#include "interfaces.h"

#include "utils/memoryutils.h"

void interfaces::init_interfaces() {
	engine = memory_utils::capture_interface<c_engine_client>("engine.dll", "VEngineClient015");
	client = memory_utils::capture_interface<chl_client>("client.dll", "VClient017");
	entity_list = memory_utils::capture_interface<c_client_entity_list>("client.dll", "VClientEntityList003");
	surface = memory_utils::capture_interface<i_surface>("vguimatsurface.dll", "VGUI_Surface030");
	lua_shared = memory_utils::capture_interface<c_lua_shared>("lua_shared.dll", "LUASHARED003");
	panel = memory_utils::capture_interface<i_panel>("vgui2.dll", "VGUI_Panel009");
	debug_overlay = memory_utils::capture_interface<i_debug_overlay>("engine.dll", "VDebugOverlay003");
	material_system = memory_utils::capture_interface<i_material_system>("materialsystem.dll", "VMaterialSystem080");
	
	//client mode pattern: CHLClient__HudProcessInput + offset to mov [reg], g_ClientMode. Sizeof mov instruction - 3. 
	client_mode = memory_utils::get_vmt<i_client_mode>((uintptr_t)client, 10, 0x0);

	mat_render_context = material_system->get_render_context();
	
}
