/*
 *    sfall
 *    Copyright (C) 2008 - 2021 Timeslip and sfall team
 *
 */

#pragma once

namespace game
{

class Skills
{
public:
	static void init();

	static int __stdcall trait_adjust_skill(DWORD skillID);

};

}