/*
 *    sfall
 *    Copyright (C) 2008 - 2020  The sfall team
 *
 */

#include "..\..\main.h"
#include "..\..\FalloutEngine\Fallout2.h"
#include "..\..\Translate.h"

#include "CombatBlock.h"

namespace sfall
{

static bool combatDisabled;

static void __stdcall CombatBlocked() {
	fo::func::display_print(Translate::CombatBlockMessage().c_str());
}

static void __declspec(naked) intface_use_item_hook() {
	static const DWORD BlockCombatHook1Ret1 = 0x45F6AF;
	static const DWORD BlockCombatHook1Ret2 = 0x45F6D7;
	__asm {
		cmp  combatDisabled, 0;
		jne  block;
		jmp  BlockCombatHook1Ret1;
block:
		call CombatBlocked;
		jmp  BlockCombatHook1Ret2;
	}
}

static void __declspec(naked) game_handle_input_hook() {
	__asm {
		mov  eax, dword ptr ds:[FO_VAR_intfaceEnabled];
		test eax, eax;
		jz   end;
		cmp  combatDisabled, 0; // eax = 1
		je   end; // no blocked
		push edx;
		call CombatBlocked;
		pop  edx;
		xor  eax, eax;
end:
		retn;
	}
}

static void __declspec(naked) ai_can_use_weapon_hack() {
	using namespace fo;
	using namespace Fields;
	__asm {
		mov  ebp, ebx;
		test dword ptr [esi + miscFlags], CantUse;
		jnz  cantUse;
		mov  eax, [edi + damageFlags];
		retn;
cantUse:
		mov  al, 0xFF;
		retn;
	}
}

static void __declspec(naked) can_use_weapon_hook() {
	static const DWORD cant_use_weapon_Ret = 0x477F9F;
	using namespace fo;
	using namespace Fields;
	__asm {
		call fo::funcoffs::item_get_type_;
		cmp  eax, item_type_weapon;
		je   checkFlag;
		retn; // eax - type
checkFlag:
		test dword ptr [edx + miscFlags], CantUse;
		jnz  cantUse;
		retn; // eax - type
cantUse:
		add esp, 4;
		jmp cant_use_weapon_Ret;
	}
}

// Note: in ai_try_attack_ the attacker will not be able to change unusable weapon, as it happens with crippled hands
static void __declspec(naked) combat_check_bad_shot_hack() {
	static const DWORD combat_check_bad_shot_Ret = 0x42673A;
	using namespace fo;
	using namespace Fields;
	__asm {
		test dword ptr [ecx + miscFlags], CantUse;
		jnz  cantUse;
		mov  eax, [esi + damageFlags];
		test al, DAM_CRIP_ARM_LEFT;
		retn;
cantUse:
		mov eax, 4; // result same as TargetDead
		add esp, 4;
		jmp combat_check_bad_shot_Ret;
	}
}

void __stdcall CombatBlock::SetBlockCombat(long toggle) {
	combatDisabled = toggle != 0;
}

void CombatBlock::init() {
	HookCall(0x45F626, intface_use_item_hook); // jnz hook
	HookCall(0x4432A6, game_handle_input_hook);

	// Adds the sfall flag MiscFlags::CantUse to set the state of the weapon to unusable in combat
	HookCall(0x477F4C, can_use_weapon_hook);
	MakeCall(0x4298F4, ai_can_use_weapon_hack);
	MakeCall(0x426669, combat_check_bad_shot_hack);
}

}
