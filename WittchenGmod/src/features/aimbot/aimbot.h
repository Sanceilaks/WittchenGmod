#pragma once

#include "../../game_sdk/entities/c_base_player.h"
#include "../../game_sdk/misc/usercmd.h"

namespace aimbot {
	c_vector get_aimbot_target();

	void run_aimbot(c_user_cmd& cmd);
	
	void norecoil(c_user_cmd& cmd);
	void nospread(c_user_cmd& cmd);

	bool start_prediction(c_user_cmd& cmd);
	void end_prediction();
}
