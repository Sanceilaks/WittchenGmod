#include "render_system.h"

IDirect3DDevice9* gameDevice;

IDirect3DDevice9* getDeviceFromGame() {
	auto* const game_hwnd = FindWindowW(L"Valve001", nullptr);
	auto* d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d9) return nullptr;

	D3DPRESENT_PARAMETERS d3dpp{};
	d3dpp.hDeviceWindow = game_hwnd;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = TRUE;

	IDirect3DDevice9* device9 = nullptr;
	if (FAILED(d3d9->CreateDevice(0, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &device9)))
	{
		d3d9->Release();
		return nullptr;
	}

	return device9;
}

//I`m gay
void render_system::Init() {
	gameDevice = getDeviceFromGame();
	
}

IDirect3DDevice9* render_system::GetDevice() {
	
}
