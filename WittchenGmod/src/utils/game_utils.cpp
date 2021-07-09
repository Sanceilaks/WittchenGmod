#include "game_utils.h"

#include <iostream>

#include "../game_sdk/entities/c_base_player.h"

bool local_player_utils::is_voice_recording() {
	using fn = bool(__thiscall*)();
	static fn func;
	if (!func) {
		func = (fn)memory_utils::pattern_scanner("engine.dll", "80 3D ? ? ? ? ? 74 0C 80 3D ? ? ? ? ?");
	}

	return func();
}

bool game_utils::screen_transform(const c_vector& in, c_vector& out) {
	auto exception_filter = [](int code, PEXCEPTION_POINTERS ex)
	{
		std::cout << code << std::endl;
		return EXCEPTION_EXECUTE_HANDLER;
	};

	__try
	{
		if (!interfaces::engine->is_in_game())
			return false;

		const D3DMATRIX& world_matrix = interfaces::engine->get_world_to_screen_matrix();

		const auto w = world_matrix.m[3][0] * in.x + world_matrix.m[3][1] * in.y + world_matrix.m[3][2] * in.z + world_matrix.m[3][3];
		if (w < 0.001f)
		{
			out.x *= 100000;
			out.y *= 100000;
			return false;
		}

		out.x = world_matrix.m[0][0] * in.x + world_matrix.m[0][1] * in.y + world_matrix.m[0][2] * in.z + world_matrix.m[0][3];
		out.y = world_matrix.m[1][0] * in.x + world_matrix.m[1][1] * in.y + world_matrix.m[1][2] * in.z + world_matrix.m[1][3];
		out.z = 0.0f;

		out.x /= w;
		out.y /= w;

		return true;
	}
	__except (exception_filter(GetExceptionCode(), GetExceptionInformation()))
	{
		out.x *= 100000;
		out.y *= 100000;
		return false;
	}
}

bool game_utils::world_to_screen(const c_vector& in, c_vector& out) {
	/*if (!screen_transform(in, out))
		return false;

	int w, h;
	interfaces::engine->get_screen_size(w, h);

	out.x = (w / 2.0f) + (out.x * w) / 2.0f;
	out.y = (h / 2.0f) - (out.y * h) / 2.0f;

	return true;*/
	return !interfaces::debug_overlay->screen_position(in, out);
}

std::vector<int> game_utils::get_valid_players(bool dormant) {
	if (!interfaces::engine->is_in_game())
		return {};

	auto local_player = get_local_player();
	if (!local_player)
		return {};
	
	std::vector<int> c;
	for (auto i = 0; i < interfaces::entity_list->get_highest_entity_index(); ++i) {
		auto ent = get_entity_by_index(i);
		if (ent && ent->is_player() && ent->is_alive() /*&& (!dormant ? !ent->is_dormant() : true)*/ && !local_player->is_equal(ent)) c.push_back(i);
	}
	return c;
}
