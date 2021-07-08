#pragma once

#include "../game_sdk/entities/c_base_entity.h"

#include "memoryutils.h"

namespace local_player_utils {
	bool is_voice_recording();
	
}

namespace game_utils {
	bool screen_transform(const c_vector& in, c_vector& out);
	bool world_to_screen(const c_vector& in, c_vector& out);

	std::vector<int> get_valid_players(bool dormant = false);
}
