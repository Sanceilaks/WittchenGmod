#include "hooks_manager.h"

#include <d3d9.h>
#include <memory>

#include <minpp/c_min_hook.h>
#include <kiero/kiero.h>

#include "imgui/imgui_impl_dx9.h"

#include <Windows.h>

#include <fmt/format.h>
#include "../render_system/render_system.h"

#include "../features/menu/menu.h"

#include "../utils/hackutils.h"

#include "../interfaces.h"

#include "../game_sdk/misc/usercmd.h"
#include "../utils/input_system.h"


std::shared_ptr<min_hook_pp::c_min_hook> minpp = nullptr;

uintptr_t get_virtual(PVOID** c, int idx) {
	return (uintptr_t)(*c)[idx];
}

#define CREATE_HOOK(_class, index, detour, original) create_hook((void*)get_virtual((PVOID**)_class, index), \
	detour, reinterpret_cast<void**>(&original));

struct end_scene_hook {
	static inline constexpr uint32_t idx = 42;

	using fn = long(__stdcall*)(IDirect3DDevice9*);
	static inline fn original = nullptr;
	static long __stdcall hook(IDirect3DDevice9* device);
};

struct reset_hook {
	static inline constexpr unsigned int idx = 16;

	using fn = long(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
	static long __stdcall hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_parameters);
	static inline fn original;
};

struct create_move_hook
{
	static inline constexpr uint32_t idx = 21;

	using fn = bool(__fastcall*)(i_client_mode*, float, c_user_cmd*);
	static inline fn original = nullptr;
	static bool __fastcall hook(i_client_mode* self, float frame_time, c_user_cmd* cmd);
};

struct lock_cursor_hook {
	static inline constexpr uint32_t idx = 66;

	using fn = void(__stdcall*)(i_surface*);
	static inline fn original = nullptr;
	static void __stdcall hook(i_surface* self);
};

struct wndproc_hook
{
	static LRESULT STDMETHODCALLTYPE hooked_wndproc(HWND window, UINT message_type, WPARAM w_param, LPARAM l_param);
	static inline WNDPROC original_wndproc = nullptr;

}; extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


void hook_dx() {
	if (init(kiero::RenderType::D3D9) == kiero::Status::Success) {
		kiero::bind(end_scene_hook::idx, (void**)&end_scene_hook::original, end_scene_hook::hook);
		kiero::bind(reset_hook::idx, (void**)&reset_hook::original, reset_hook::hook);
	}
}

void hooks_manager::init() {
	minpp = std::make_shared<min_hook_pp::c_min_hook>();

	hook_dx();

	CREATE_HOOK(interfaces::client_mode, create_move_hook::idx, create_move_hook::hook, create_move_hook::original);
	CREATE_HOOK(interfaces::surface, lock_cursor_hook::idx, lock_cursor_hook::hook, lock_cursor_hook::original);
	
	auto* const game_hwnd = FindWindowW(0, L"Garry's Mod (x64)");
	wndproc_hook::original_wndproc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(
		game_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(wndproc_hook::hooked_wndproc)));
	
	minpp->enable_hook();
}

void hooks_manager::shutdown() {
	minpp->unhook();
	minpp.reset();

	auto* const game_hwnd = FindWindowW(0, L"Garry's Mod (x64)");
	if (game_hwnd)
		SetWindowLongPtr(game_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(wndproc_hook::original_wndproc));
}

void hooks_manager::create_hook(void* target, void* detour, void** original) {
	const auto result = minpp->create_hook(target, detour, original);
	if (!result)
		throw std::exception(fmt::format("Failed to create hook. Target: {}, detour: {}, original: {}",
			reinterpret_cast<uintptr_t>(target), reinterpret_cast<uintptr_t>(detour),
			reinterpret_cast<uintptr_t>(*original)).c_str());
}


long end_scene_hook::hook(IDirect3DDevice9* device) {
	auto ret = original(device);
	render_system::on_end_scene(device, (uintptr_t)_ReturnAddress());
	return ret;
}

long reset_hook::hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_parameters) {
	const auto ret = original(device, present_parameters);
	ImGui_ImplDX9_InvalidateDeviceObjects();

	if (ret > 0)
		ImGui_ImplDX9_CreateDeviceObjects();

	return ret;
}

class cply {
public:
	int get_flags() {
		return *(int*)((uintptr_t)this + 0x440);
	}
};

bool create_move_hook::hook(i_client_mode* self, float frame_time, c_user_cmd* cmd) {
	if (!cmd || !cmd->command_number)
		return original(self, frame_time, cmd);
	
	cply* lp = (cply*)interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player());
	
	if (cmd->buttons & IN_JUMP && lp && !(lp->get_flags() & (1 << 0))) {
		cmd->buttons &= ~IN_JUMP;
	}
	
	return original(interfaces::client_mode, frame_time, cmd);
}

void lock_cursor_hook::hook(i_surface* self) {
	if (menu::menu_is_open()) {
		return self->unlock_cursor();
	}
	original(self);
}

LRESULT STDMETHODCALLTYPE wndproc_hook::hooked_wndproc(HWND window, UINT message_type, WPARAM w_param, LPARAM l_param)
{
	if (message_type == WM_CLOSE) {
		hack_utils::unload_hack();
		return true;
	}

	auto mk = VK_INSERT;
	/*if (settings::binds["other::menu_key"] > 0)
		mk = settings::binds["other::menu_key"];*/

	if (message_type == WM_KEYDOWN)
		if (w_param == mk)
			menu::toggle_menu();
	
	ImGui_ImplWin32_WndProcHandler(window, message_type, w_param, l_param);
	if (ImGui_ImplWin32_WndProcHandler(window, message_type, w_param, l_param) && menu::menu_is_open())
	{
		input_system::process_binds();
		return true;
	}

	input_system::process_binds();
	return CallWindowProc(original_wndproc, window, message_type, w_param, l_param);
}
