#pragma once

#include "c_base_entity.h"

class c_base_player : public c_base_entity {
public:
	NETVAR("DT_BasePlayer", "m_fFlags", get_flags, int);
	NETVAR("DT_BasePlayer", "m_vecViewOffset[0]", get_view_offset, c_vector);
	NETVAR("DT_BasePlayer", "m_hActiveWeapon", active_weapon_handle, uintptr_t);
	NETVAR("DT_BasePlayer", "m_vecVelocity[0]", get_velocity, c_vector);
	NETVAR("DT_BasePlayer", "hMyWeapons", get_weapons, uintptr_t);
	NETVAR("DT_BasePlayer", "m_bloodColor", get_blood_color, c_vector);
	NETVAR("DT_BasePlayer", "m_Local", get_local, uintptr_t);
	NETVAR("DT_BasePlayer", "m_iAmmo", get_ammo, int);
	NETVAR("DT_BasePlayer", "m_fOnTarget", get_on_target, int);
	NETVAR("DT_BasePlayer", "m_nTickBase", get_tick_base, int);
	NETVAR("DT_BasePlayer", "m_nNextThinkTick", get_next_think_tick, float);
	NETVAR("DT_BasePlayer", "m_flFriction", get_friction, float);
	NETVAR("DT_BasePlayer", "flDeathTime", get_death_time, float);
	NETVAR("DT_BasePlayer", "deadflag", get_dead_flag, int);
	NETVAR("DT_BasePlayer", "m_iDefaultFOV", get_default_fov, int);
	NETVAR("DT_BasePlayer", "m_hVehicle", get_vehicle_handle, uintptr_t);
	NETVAR("DT_BasePlayer", "m_iObserverMode", get_observer_mode, int);
	NETVAR("DT_BasePlayer", "m_hObserverTarget", get_observer_target_handle, uintptr_t);
	NETVAR("DT_PlayerResource", "m_iPing", get_ping, int);

	std::string get_name() const
	{
		player_info_s info;
		interfaces::engine->get_player_info(get_index(), &info);
		return info.name;
	}

	/*bool is_admin()
	{
		auto str = get_user_group();
		return str.find("admin") != std::string::npos || str.find("owner") != std::string::npos
			|| str.find("king") != std::string::npos || str.find("moder") != std::string::npos || str.find("root") != std::string::npos;
	}*/

	/*std::string get_team_name()
	{
		auto glua = interfaces::lua_shared->get_interface((int)e_special::glob);
		if (!glua)
			return {};
		c_lua_auto_pop p(glua);

		glua->push_special((int)e_special::glob);
		glua->get_field(-1, "team");
		glua->get_field(-1, "GetName");
		glua->push_number(this->get_team_num());
		glua->call(1, 1);

		return glua->get_string();
	}

	std::string get_user_group()
	{
		auto lua = interfaces::lua_shared->get_interface((int)e_special::glob);
		if (!lua)
			return {};
		c_lua_auto_pop p(lua);

		push_entity();
		lua->get_field(-1, "GetUserGroup");
		lua->push(-2);
		lua->call(1, 1);
		return lua->get_string();
	}*/
};

class c_local_player : public c_base_player {
public:
};

__forceinline c_local_player* get_local_player()
{
	return static_cast<c_local_player*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
}


__forceinline c_base_player* get_player_by_index(const uint32_t i)
{
	return reinterpret_cast<c_base_player*>(interfaces::entity_list->get_client_entity(i));
}