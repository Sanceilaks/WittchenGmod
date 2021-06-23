#pragma once

class c_view_setup;

class chl_client {
public:
	virtual void init(void) = 0;
	virtual void post_init(void) = 0;
	virtual void shutdown(void) = 0;
	virtual void unk1(void) = 0;
	virtual void reply_post_init(void) = 0;
	virtual void level_init_pre_entity() = 0;
	virtual void level_init_post_entity() = 0;
	virtual void level_shutdown() = 0;
	virtual void* get_all_classes() = 0;

	bool get_player_view(c_view_setup* setup) {
		using fn = bool(__fastcall*)(void*, c_view_setup* out);
		return (*(fn**)this)[59](this, setup);
	}
};
