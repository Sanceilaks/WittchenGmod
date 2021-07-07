#include "game_utils.h"

bool local_player_utils::is_voice_recording() {
	using fn = bool(__thiscall*)();
	static fn func;
	if (!func) {
		func = (fn)memory_utils::pattern_scanner("engine.dll", "80 3D ? ? ? ? ? 74 0C 80 3D ? ? ? ? ?");
	}

	return func();
}
