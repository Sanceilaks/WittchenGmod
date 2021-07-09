#pragma once
#include "color.h"
#include "../../utils/netvars.h"
#include "../../interfaces.h"

#include "../../utils/memoryutils.h"

#include "i_client_entity.h"

class c_collidable
{
public:
	virtual void unknown_0() = 0;
	virtual c_vector& mins() = 0;
	virtual c_vector& maxs() = 0;
};

class c_base_entity : public i_client_entity
{
public:
	NETVAR("DT_BaseEntity", "m_vecOrigin", get_origin, c_vector);
	NETVAR("DT_GMOD_Player", "m_angEyeAngles[0]", get_angles, c_vector);
	NETVAR("DT_BaseEntity", "m_nModelIndex", get_model_index, int);
	NETVAR("DT_BaseEntity", "m_iTeamNum", get_team_num, int);
	NETVAR("DT_BasePlayer", "m_vecViewOffset[0]", get_view_offset, c_vector);
	NETVAR("DT_BaseEntity", "m_flAnimTime", get_anim_time, float);
	NETVAR("DT_BaseEntity", "m_flSimulationTime", get_simulation_time, float);
	NETVAR("DT_BaseEntity", "m_angRotation", get_rotation, c_vector);
	NETVAR("DT_BaseEntity", "m_hOwnerEntity", get_owner_entity_handle, uintptr_t);
	NETVAR("DT_BaseEntity", "m_iHealth", get_health, int);
	NETVAR("DT_BaseEntity", "m_iMaxHealth", get_max_health, int);

	c_vector get_eye_pos() { return this->get_origin() + this->get_view_offset(); }

	c_collidable* get_collidable_ptr()
	{
		using original_fn = c_collidable * (__thiscall*)(void*);
		return (*(original_fn**)this)[3](this);
	}

	bool is_alive()
	{
		using original_fn = bool(__fastcall*)(void*);
		return (*(original_fn**)this)[129](this);
	}

	bool is_player()
	{
		using original_fn = bool(__fastcall*)(void*);
		return (*(original_fn**)this)[130](this);
	}

	void push_entity() //https://imgur.com/bK6cNdu
	{
		using original_fn = void(__fastcall*)(void*);
		return (*(original_fn**)this)[172](this);
	}

	bool is_use_lua()
	{
		using fn = bool(__fastcall*)(void*);
		return (*(fn**)this)[170](this);
	}

	const char* get_lua_script_name() //B8 AA 2A 4D 00 C3	client.dll
	{
		using fn = const char* (__fastcall*)(void*);
		return (*(fn**)this)[183](this);
	}

	//E8 ? ? ? ? F3 0F 11 BD ? ? ? ?
	void set_abs_origin(const c_vector& position)
	{
		using fn = void(__fastcall*)(void*, const c_vector& origin);
		static fn orig_fn;
		if (!orig_fn)
			orig_fn = reinterpret_cast<fn>(memory_utils::relative_to_absolute((uintptr_t)memory_utils::pattern_scanner("client.dll",
				"E8 ? ? ? ? F3 0F 11 BD ? ? ? ?"), 0x1, 5));
		orig_fn(this, position);
	}


	void set_abs_angles(const c_vector& ang)
	{
		using fn = void(__fastcall*)(void*, const c_vector& origin);
		static fn orig_fn;
		if (!orig_fn)
			orig_fn = reinterpret_cast<fn>(memory_utils::relative_to_absolute((uintptr_t)memory_utils::pattern_scanner("client.dll",
				"E8 ?? ?? ?? ?? 48 8D 57 70"), 0x1, 5));
		orig_fn(this, ang);
	}

	bool is_equal(c_base_entity* ent) const {
		return ent->get_index() == get_index();
	}

	float get_health_procentage() {
		return get_health() / (get_max_health() / 100.f);
	}
};

__forceinline c_base_entity* get_entity_by_index(const int i)
{
	return static_cast<c_base_entity*>(interfaces::entity_list->get_client_entity(i));
}