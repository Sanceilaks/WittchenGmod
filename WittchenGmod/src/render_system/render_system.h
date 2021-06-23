#pragma once

#include <d3d9.h>

namespace render_system {
	void Init();
	IDirect3DDevice9* GetDevice();
}
