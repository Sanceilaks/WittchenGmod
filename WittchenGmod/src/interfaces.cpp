#include "interfaces.h"

#include "utils/memoryutils.h"

void interfaces::init_interfaces() {
	engine = memory_utils::capture_interface<c_engine_client>("engine.dll", "VEngineClient015");
	client = memory_utils::capture_interface<chl_client>("client.dll", "VClient017");
	entity_list = memory_utils::capture_interface<c_client_entity_list>("client.dll", "VClientEntityList003");
	surface = memory_utils::capture_interface<i_surface>("vguimatsurface.dll", "VGUI_Surface030");
	
	//client mode pattern: CHLClient__HudProcessInput + offset to mov [reg], g_ClientMode. Sizeof mov instruction - 3. 
	client_mode = memory_utils::get_vmt<i_client_mode>((uintptr_t)client, 10, 0x0);

	
}
