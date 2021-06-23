#include "interfaces.h"

#include "utils/memoryutils.h"

void interfaces::init_interfaces() {
	engine = memory_utils::capture_interface<c_engine_client>("engine.dll", "VEngineClient015");
	client = memory_utils::capture_interface<chl_client>("client.dll", "VClient017");
	entity_list = memory_utils::capture_interface<c_client_entity_list>("client.dll", "VClientEntityList003");

	
}
