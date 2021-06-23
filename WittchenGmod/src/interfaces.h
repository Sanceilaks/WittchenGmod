#pragma once

#include "game_sdk/interfaces/engine.h"
#include "game_sdk/interfaces/client.h"
#include "game_sdk/interfaces/cliententitylist.h"

namespace interfaces {
	inline CEngineClient* Engine;
	inline CClientEntityList* EntityList;
	inline CHLClient* Client;
	
	void InitInterfaces();
}
