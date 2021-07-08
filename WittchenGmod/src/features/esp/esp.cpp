#include "esp.h"

#include "../../game_sdk/entities/c_base_player.h"
#include <algorithm>

#include <lemi_utils.h>

/// Formatter usage
/// Player:
///		%name - player name
///		%health - player health in procesnt`s
///		%armour - player armour in procesnt`s //not impl
///		%weapon - player weapon
///		%group - player group
///		%team - player team
///		%id - steam id
///		%rpname - rpname of player
///		%ammo - current weapon ammo
///
///	Entity:
///		%name - entity name
///		%health - player health in procesnt`s

std::string format_text_for_player(const std::string& str, c_base_player* ply) {
	auto s = str;
	s = replace_all(s, "%name", ply->get_name());
	s = replace_all(s, "%health", std::to_string(ply->get_health()));

	return s;
}

void esp::draw_esp() {
	
}
