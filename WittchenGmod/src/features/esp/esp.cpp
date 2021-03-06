#include "esp.h"

#include "../../game_sdk/entities/c_base_player.h"
#include <algorithm>

#include <lemi_utils.h>

#include "../../utils/game_utils.h"

#include "../../render_system/render_system.h"

#include "../menu/widgets/widgets.h"

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

void format_esp_map_for_players(std::unordered_map<uint64_t, esp::esp_text_t>& t, c_base_player* ply) {
	for (auto& i : t) {
		i.second.color = i.second.is_auto_color ? Wittchen::WitthcenEspStyleEditor::GetAutoColor(i.second.text, ply) : i.second.color;
		i.second.text = format_text_for_player(i.second.text, ply);
	}
}

__forceinline float calc_font_size(const esp::c_esp_box& box) {
	return 13.f;
}

void esp::c_esp_box::get_absolute_position(const ImVec2& r) {
	
}

ImVec2 esp::c_esp_box::get_screen_position(const ImVec2& pos) const {
	return {min.x + pos.x, min.y + pos.y};
}

bool esp::c_esp_box::calc_box(c_base_entity* ent, c_esp_box& box) {
	using namespace game_utils;
	c_vector flb, brt, blb, frt, frb, brb, blt, flt;

	const auto& origin = ent->get_render_origin();
	const auto min = ent->get_collidable_ptr()->mins() + origin;
	const auto max = ent->get_collidable_ptr()->maxs() + origin;

	c_vector points[] = {
		c_vector(min.x, min.y, min.z),
		c_vector(min.x, max.y, min.z),
		c_vector(max.x, max.y, min.z),
		c_vector(max.x, min.y, min.z),
		c_vector(max.x, max.y, max.z),
		c_vector(min.x, max.y, max.z),
		c_vector(min.x, min.y, max.z),
		c_vector(max.x, min.y, max.z)
	};

	if (!world_to_screen(points[3], flb) || !world_to_screen(points[5], brt)
		|| !world_to_screen(points[0], blb) || !world_to_screen(points[4], frt)
		|| !world_to_screen(points[2], frb) || !world_to_screen(points[1], brb)
		|| !world_to_screen(points[6], blt) || !world_to_screen(points[7], flt))
		return false;

	c_vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	auto left = flb.x;
	auto top = flb.y;
	auto right = flb.x;
	auto bottom = flb.y;

	if (left < 0 || top < 0 || right < 0 || bottom < 0)
		return false;

	for (int i = 1; i < 8; i++) {
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}

	box.min = { left, top };
	box.max = {right, bottom};

	return true;
}

ImVec2 esp::c_esp_box::calc_text_position(const c_esp_box& box, esp_text_t& text, std::array<ImVec2, 4>& last_positions) {
	const ImVec2 box_size = { box.max.x - box.min.x, box.max.y - box.min.y };
	
	if (text.relative_position == (int)esp::e_esp_text_position::top) {
		auto& last_position = last_positions[(int)esp::e_esp_text_position::top];
		auto font_size = (text.size == -1 || text.size == 0 ) ? calc_font_size(box) : text.size;
		auto text_size = render_system::fonts::nunito_font[2]->CalcTextSizeA(font_size, FLT_MAX, 0.f, text.text.c_str());
		ImVec2 base_position = { box_size.x / 2.f, -(text_size.y / 2.f) };

		ImVec2 position = { base_position.x, last_position.y != -1.f ? last_position.y - text_size.y : base_position.y };

		//only centered text
		text.flags = !(text.flags & directx_render::font_centered) ? text.flags |= directx_render::font_centered : text.flags;

		last_position = position;
		return position;
	}

	if (text.relative_position == (int)esp::e_esp_text_position::right) {
		auto& last_position = last_positions[(int)esp::e_esp_text_position::right];
		auto font_size = (text.size == -1 || text.size == 0) ? calc_font_size(box) : text.size;
		auto text_size = render_system::fonts::nunito_font[2]->CalcTextSizeA(font_size, FLT_MAX, 0.f, text.text.c_str());
		ImVec2 base_position = { box_size.x + text_size.y / 2.f, 0 };
		ImVec2 position = { base_position.x, last_position.y != -1.f ? last_position.y + text_size.y : base_position.y };

		last_position = position;
		return position;
	}

	if (text.relative_position == (int)esp::e_esp_text_position::left) {
		auto& last_position = last_positions[(int)esp::e_esp_text_position::left];
		auto font_size = (text.size == -1 || text.size == 0) ? calc_font_size(box) : text.size;
		auto text_size = render_system::fonts::nunito_font[2]->CalcTextSizeA(font_size, FLT_MAX, 0.f, text.text.c_str());
		ImVec2 base_position = { -text_size.x - (text_size.y / 2.f), 0 };
		ImVec2 position = { base_position.x, last_position.y != -1.f ? last_position.y + text_size.y : base_position.y };

		last_position = position;
		return position;
	}

	if (text.relative_position == (int)esp::e_esp_text_position::down) {
		auto& last_position = last_positions[(int)esp::e_esp_text_position::down];
		auto font_size = (text.size == -1 || text.size == 0) ? calc_font_size(box) : text.size;
		auto text_size = render_system::fonts::nunito_font[2]->CalcTextSizeA(font_size, FLT_MAX, 0.f, text.text.c_str());
		ImVec2 base_position = { box_size.x / 2.f, box_size.y + (text_size.y / 2.f) };

		ImVec2 position = { base_position.x, last_position.y != -1.f ? last_position.y + text_size.y : base_position.y };

		//only centered text
		text.flags = !(text.flags & directx_render::font_centered) ? text.flags |= directx_render::font_centered : text.flags;

		last_position = position;
		return position;
	}

	return { 0, 0 };
}

uint64_t esp::c_esp_box::generate_id() {
	static uint64_t last;
	last++;
	return last;
}

void render_strings_for_players(esp::c_esp_box& box, c_base_player* player) {
	//format strings first
	format_esp_map_for_players(box.text_storage.strings, player);

	//constants
	box.text_storage.last_positions.fill({ -1, -1 });
	
	//render strings
	{
		for (auto& i : box.text_storage.strings) {
			auto position = esp::c_esp_box::calc_text_position(box, i.second, box.text_storage.last_positions);
			const auto font_size = (i.second.size == -1 || i.second.size == 0) ? calc_font_size(box) : i.second.size;
			directx_render::text(render_system::fonts::nunito_font[2], i.second.text, box.get_screen_position(position), font_size, i.second.color, i.second.flags);
		}
	}
}

void esp::draw_esp() {
	
	//at this time only players
	for (auto i : game_utils::get_valid_players(true)) {
		auto p = get_player_by_index(i);
		
		c_esp_box box;
		if (!c_esp_box::calc_box(p, box))
			continue;
		Wittchen::ApplyStyleToBox(box);

		render_strings_for_players(box, p);

		switch ((box_type)box.type) {
		case box_type::filled:
			directx_render::bordered_rect(box.min, box.max, box.color, box.rounding);
			break;
		case box_type::border:
			directx_render::bordered_rect({ box.min.x - 1.f, box.min.y - 1.f }, { box.max.x + 1.f, box.max.y + 1.f }, box.border_color, box.rounding);
			directx_render::bordered_rect({ box.min.x + 1.f, box.min.y + 1.f }, { box.max.x - 1.f, box.max.y - 1.f }, box.border_color, box.rounding);
			directx_render::bordered_rect(box.min, box.max, box.color, box.rounding);
			break;
		case box_type::corner:
			directx_render::corner_box(box.min, box.max, box.color);
		}
	}
}
