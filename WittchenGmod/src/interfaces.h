#pragma once

#include "game_sdk/interfaces/engine.h"
#include "game_sdk/interfaces/client.h"
#include "game_sdk/interfaces/cliententitylist.h"

namespace interfaces {
	inline c_engine_client* engine;
	inline c_client_entity_list* entity_list;
	inline chl_client* client;
	
	void init_interfaces();
}
