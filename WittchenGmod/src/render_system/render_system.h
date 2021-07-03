#pragma once

#include <array>

#include <d3d9.h>
#include <imgui/im_tools.h>

namespace render_system {
	namespace fonts {
		inline std::array<ImFont*, 3> nunito_font;

		inline ImFont* arial_font;
	}
	
	void init();
	IDirect3DDevice9* get_device();

	void on_end_scene(LPDIRECT3DDEVICE9 device, uintptr_t return_address);
}
