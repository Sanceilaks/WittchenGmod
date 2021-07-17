#include "aimbot.h"

#include "spreads.h"

#include "../../game_sdk/entities/c_base_weapon.h"

#include "../../settings/settings.h"

#include "../../utils/game_utils.h"

#include "../../globals.h"

struct shoot_pos_t {
	c_vector position;
	q_angle angle;
	float fov;
};

struct target_t {
	c_base_player* ply;
	shoot_pos_t shoot_pos;
	int idx;
};

int last_target_id = -1;
float last_target_time = -1.f;

bool can_do_damage(c_base_player* player) {
	
}

bool on_bone_not_exist(shoot_pos_t& sp, c_base_player* player) {
	auto position = (player->get_collidable_ptr()->mins() + player->get_collidable_ptr()->maxs()) / 2.f;
	if (!position.is_valid())
		return false;
	q_angle real_angle;
	interfaces::engine->get_view_angles(real_angle);
	
	sp.position = position;
	sp.fov = game_utils::get_fov(real_angle, game_utils::calc_angle(get_local_player()->get_eye_pos(), position));
	auto angle = math::get_angle(get_local_player()->get_eye_pos(), position);
	if (!angle.is_valid())
		return false;
	sp.angle = angle;

	return true;
}

bool get_shoot_pos(shoot_pos_t& sp, c_base_player* player) {
	uint32_t bone_idx = -1;
	float best_fov = FLT_MAX;

	int bones = settings::get_int("aimbot_bones");

	if (bones == (int)e_bones::none) return on_bone_not_exist(sp, player);

	q_angle real_angle;
	interfaces::engine->get_view_angles(real_angle);
	
	for (const auto& i : entity_bones_by_class) {
		if ((i.first == "head" && !(bones & (int)e_bones::head)) || (i.first == "arm" && !(bones & (int)e_bones::arm))
			|| (i.first == "foot" && !(bones & (int)e_bones::foot)) || (i.first == "pelvis" && !(bones & (int)e_bones::pelvis)) || (i.first == "body" && !(bones & (int)e_bones::body)))
			continue;

		for (const auto& j : i.second) {
			auto idx = player->get_bone_by_name(j);
			if (idx <= 0) continue;
			c_vector position = player->get_bone(idx);
			if (!position.is_valid()) continue;
			if (const auto fov = game_utils::get_fov(real_angle, game_utils::calc_angle(get_local_player()->get_eye_pos(), position)); fov < best_fov)
				best_fov = fov, bone_idx = idx;
		}
	}

	if (!bone_idx) return on_bone_not_exist(sp, player);

	const auto bone_position = player->get_bone(bone_idx);
	sp.position = bone_position;
	sp.angle = math::get_angle(get_local_player()->get_eye_pos(), sp.position);
	sp.fov = game_utils::get_fov(real_angle, game_utils::calc_angle(get_local_player()->get_eye_pos(), bone_position));

	return true;
}

bool get_target(target_t& target)
{
	auto invalidate_target = [](const target_t& t)
	{
		return (t.ply != nullptr && t.shoot_pos.angle.x != 0 && t.shoot_pos.angle.y != 0 && t.shoot_pos.fov >= 0.f); //fov maybe FLT_MAX and 0
	};

	if (!get_local_player())
		return false;

	target_t tmp{ nullptr, {}};
	tmp.shoot_pos.fov = FLT_MAX;
	
	for (auto i : game_utils::get_valid_players(false)) {
		auto player = get_player_by_index(i);

		shoot_pos_t tmp_shoot_pos;
		
		if (!get_shoot_pos(tmp_shoot_pos, player))
			continue;

		auto name = player->get_name();
		
		if (tmp_shoot_pos.fov < tmp.shoot_pos.fov && tmp_shoot_pos.fov <= (float)settings::get_int("aimbot_fov")) {
			tmp.ply = player;
			tmp.idx = i;
			tmp.shoot_pos = tmp_shoot_pos;
		}
	}

	if (!invalidate_target(tmp))
		return false;

	target = tmp;
	return true;
}

void aimbot::run_aimbot(c_user_cmd& cmd) {
	if (!settings::get_bool("aimbot"))
		return;
	
	if (!cmd.is_in_fire())
		return;
	
	target_t target;
	
	if (!get_target(target))
		return;

	last_target_id = target.idx;
	last_target_time = interfaces::engine->get_time_stamp_from_start();

	cmd.viewangles = target.shoot_pos.angle;

	interfaces::engine->set_view_angles(cmd.viewangles);
}

void aimbot::norecoil(c_user_cmd& cmd) {
	if (!settings::get_bool("norecoil"))
		return;
	
	if (cmd.buttons & IN_ATTACK)
		cmd.viewangles -= get_local_player()->get_view_punch_angles();
}

void aimbot::nospread(c_user_cmd& cmd) {
	if (!settings::get_bool("nospread"))
		return;
	
	auto wep = get_local_player()->get_active_weapon();

	if (cmd.buttons & IN_ATTACK) {
		if (wep->get_weapon_base().find("bobs_gun") != std::string::npos)
			spreads::base_nospread(cmd);
	}
}
