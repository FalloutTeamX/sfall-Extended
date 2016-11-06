/*
* sfall
* Copyright (C) 2008-2016 The sfall team
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "..\..\FalloutEngine\Structs.h"

// TODO: replace with enum class
enum SfallDataType {
	DATATYPE_NONE = 0,
	DATATYPE_INT,
	DATATYPE_FLOAT,
	DATATYPE_STR
};

class ScriptValue {
public:
	ScriptValue(SfallDataType type, unsigned long value);

	ScriptValue();

	ScriptValue(const char* strval);

	ScriptValue(int val);

	ScriptValue(float strval);

	ScriptValue(TGameObj* obj);

	bool isInt() const;

	bool isFloat() const;

	bool isString() const;

	unsigned long rawValue() const;

	int asInt() const;

	float asFloat() const;

	const char* asString() const;

	TGameObj* asObject() const;

	SfallDataType type() const;

private:
	union Value {
		unsigned long dw;
		int i;
		float f;
		const char* str;
		TGameObj* gObj;
	} _val;

	SfallDataType _type; // TODO: replace with enum class
};

