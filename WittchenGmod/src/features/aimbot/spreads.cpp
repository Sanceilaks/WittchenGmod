#include "spreads.h"

#include "../../game_sdk/entities/c_base_weapon.h"

#include "../../utils/md5_check_sum.h"

void spreads::base_nospread(c_user_cmd& cmd) {
	auto wep = get_local_player()->get_active_weapon();
	if (!wep)
		return;
	
	auto spread_cone = wep->get_bullet_spread();

	if (((std::string)(wep->get_lua_script_name())).find("m9k") != std::string::npos) {
		auto in = interfaces::lua_shared->get_lua_interface((int)e_interface_type::client);
		c_lua_auto_pop p(in);
		auto s = wep->get_primary_value("Spread");
		spread_cone = {s};
	}
	
	if (spread_cone == q_angle(-1.f))
		return;

	const auto spread = ((spread_cone.x + spread_cone.y + spread_cone.z) / 3.f);

	float random[2];
	if (cmd.command_number == 0)
		return;
	auto seed = md5::md5_pseudo_random(cmd.command_number) & 0xFF;

	interfaces::random_stream->set_seed(seed);

	random[0] = interfaces::random_stream->random_float(-0.5f, 0.5f)
		+ interfaces::random_stream->random_float(-0.5f, 0.5f);

	random[1] = interfaces::random_stream->random_float(-0.5f, 0.5f)
		+ interfaces::random_stream->random_float(-0.5f, 0.5f);

	auto dir = (c_vector(1.0f, 0.0f, 0.0f) + (c_vector(0.0f, -1.0f, 0.0f) * spread * random[0]) + (c_vector(0.0f, 0.0f, 1.0f) * spread * random[1]));

	q_angle out = math::get_angle(q_angle(0.f, 0.f, 0.f), dir);
	//if (!settings::states["aim_bot::no_recoil"])
		//out += get_local_player()->get_view_punch_angles();
	//else
		//out -= get_local_player()->get_view_punch_angles();

	out = math::fix_angles(out);

	if (out.is_valid())
		cmd.viewangles -= out;
}
