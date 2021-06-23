#pragma once

#include <d3d9.h>

namespace render_system {
	void init();
	IDirect3DDevice9* get_device();

	void on_end_scene(LPDIRECT3DDEVICE9 device, uintptr_t return_address);
}
