#pragma once

#include "../game_sdk/entities/c_base_entity.h"

#include "memoryutils.h"

class c_local_player : public c_base_entity {
public:

	static bool is_voice_recording() {
		using fn = bool(__thiscall*)();
		static fn func;
		if (!func) {
			func = (fn)memory_utils::pattern_scanner("engine.dll", "80 3D ? ? ? ? ? 74 0C 80 3D ? ? ? ? ?");
		}
		
		return func();
	}
};