#include "interfaces.h"

#include "utils/memoryutils.h"

constexpr auto client_mode_pattern = "48 8B 0D ? ? ? ? 48 8B 01 48 FF 60 50 CC CC 48 83 EC 28";

void interfaces::init_interfaces() {
	engine = memory_utils::capture_interface<c_engine_client>("engine.dll", "VEngineClient015");
	client = memory_utils::capture_interface<chl_client>("client.dll", "VClient017");
	entity_list = memory_utils::capture_interface<c_client_entity_list>("client.dll", "VClientEntityList003");

	//client mode patter: CHLClient__HudProcessInput + sizeof mov instruction
	client_mode = memory_utils::get_vmt<i_client_mode>((uintptr_t)client, 10, 0x0);

	
}
