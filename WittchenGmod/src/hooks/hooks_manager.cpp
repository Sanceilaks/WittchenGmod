#include "hooks_manager.h"

#include <d3d9.h>
#include <intrin.h>
#include <iostream>
#include <memory>

#include <minpp/c_min_hook.h>
#include <kiero/kiero.h>

#include "imgui/imgui_impl_dx9.h"

#include <Windows.h>

#include "../render_system/render_system.h"

#include "../features/menu/menu.h"

#include "../utils/hackutils.h"

#include "../interfaces.h"

#include "../game_sdk/misc/usercmd.h"
#include "../utils/input_system.h"

#include "../utils/memoryutils.h"
#include "../utils/game_utils.h"
#include "../features/lua_futures/lua_futures.h"
#include "../features/esp/esp.h"
#include "../settings/settings.h"

#include "../game_sdk/entities/c_base_weapon.h"
#include "../features/aimbot/spreads.h"
#include "../features/aimbot/aimbot.h"
#include "../game_sdk/misc/viewsetup.h"
#include "../globals.h"

std::shared_ptr<min_hook_pp::c_min_hook> minpp = nullptr;
uintptr_t cl_move = 0;

#define RENDER_CAPTURE_PATTERN "40 55 41 57 48 8D 6C 24 ?"
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

struct present_hook { 
	static inline constexpr uint32_t idx = 17;

	using fn = long(__stdcall*)(IDirect3DDevice9*, RECT*, RECT*, HWND, RGNDATA*);
	static inline fn original = nullptr;
	static long __stdcall hook(IDirect3DDevice9* device, RECT* src_rect, RECT* dest_rect, HWND dest_wnd_override,
		RGNDATA* dirty_region);
};

struct create_move_hook
{
	static inline constexpr uint32_t idx = 21;

	using fn = bool(__fastcall*)(i_client_mode*, float, c_user_cmd*);
	static inline fn original = nullptr;
	static bool __fastcall hook(i_client_mode* self, float frame_time, c_user_cmd* cmd);
};

struct cl_move_hook
{
	using fn = void(__fastcall*)(float, bool);
	static inline fn original = nullptr;
	static void __fastcall hook(float aes, bool final_tick);
};

struct lock_cursor_hook {
	static inline constexpr uint32_t idx = 66;

	using fn = void(__stdcall*)(i_surface*);
	static inline fn original = nullptr;
	static void __stdcall hook(i_surface* self);
};

struct run_string_ex {
	static inline constexpr uint32_t idx = 111;

	using fn = bool(__fastcall*)(void*, const char*, const char*, const char*, bool, bool, bool, bool);
	static inline fn original = nullptr;
	static bool __fastcall hook(c_lua_interface* self, const char* filename, const char* path,
		const char* string_to_run, bool run, bool print_errors, bool dont_push_errors,
		bool no_returns);
};

struct run_command_hook {
	static inline constexpr uint32_t idx = 17;

	using fn = void(__fastcall*)(i_prediction*, c_base_entity*, c_user_cmd*, i_move_helper*);
	static inline fn original = nullptr;
	static void __fastcall hook(i_prediction* pred, c_base_entity* player, c_user_cmd* ucmd,
		i_move_helper* move_helper);
};

struct paint_traverse_hook {
	static inline constexpr uint32_t idx = 41;

	using fn = void(__fastcall*)(i_panel*, void*, bool, bool);
	static inline fn original = nullptr;
	static auto __fastcall hook(i_panel* self, void* panel, bool force_repaint, bool allow_force) -> void;
};

struct override_view_hook {
	using fn = void(__fastcall*)(i_client_mode*, c_view_setup&);
	static inline fn original = nullptr;
	static void __fastcall hook(i_client_mode*, c_view_setup&);
};

struct get_viewmodel_fov {
	using fn = float(__stdcall*)();
	static inline fn original = nullptr;
	static float __stdcall hook();
};

struct get_aspect_ration_hook {
	static inline constexpr uint32_t idx = 95;
	
	using fn = float(__fastcall*)(void*);
	static inline fn original = nullptr;
	static float __fastcall hook(void* self);
};

struct view_render_hook {
	using fn = void(__fastcall*)(void*, void*);
	static inline fn original = nullptr;
	static void __fastcall hook(chl_client* self, vrect_t* rect);
};

struct read_pixels_hook {
	static inline constexpr uint32_t idx = 11;
	
	using fn = void(__fastcall*)(void*, int, int, int, int, unsigned char*, image_format);
	static inline fn original = NULL;
	static void __fastcall hook(i_mat_render_context* self, int x, int y, int width, int height, unsigned char* data, image_format dstFormat);
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
		kiero::bind(present_hook::idx, (void**)&present_hook::original, present_hook::hook);
	}
}

void hooks_manager::init() {
	minpp = std::make_shared<min_hook_pp::c_min_hook>();
	cl_move = memory_utils::relative_to_absolute((uintptr_t)memory_utils::pattern_scanner("engine.dll", "E8 ? ? ? ? FF 15 ? ? ? ? F2 0F 10 0D ? ? ? ? 85 FF"), 0x1, 5);
	
	hook_dx();

	CREATE_HOOK(interfaces::client_mode, create_move_hook::idx, create_move_hook::hook, create_move_hook::original);
	CREATE_HOOK(interfaces::surface, lock_cursor_hook::idx, lock_cursor_hook::hook, lock_cursor_hook::original);
	CREATE_HOOK(interfaces::panel, paint_traverse_hook::idx, paint_traverse_hook::hook, paint_traverse_hook::original);
	CREATE_HOOK(interfaces::prediction, run_command_hook::idx, run_command_hook::hook, run_command_hook::original);
	CREATE_HOOK(interfaces::engine, get_aspect_ration_hook::idx, get_aspect_ration_hook::hook, get_aspect_ration_hook::original);
	CREATE_HOOK(interfaces::mat_render_context, read_pixels_hook::idx, read_pixels_hook::hook, read_pixels_hook::original);
	
	create_hook((void*)memory_utils::pattern_scanner("client.dll", "40 55 53 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B DA"), override_view_hook::hook, (void**)(&override_view_hook::original));
	create_hook((void*)cl_move, cl_move_hook::hook, (void**)(&cl_move_hook::original));
	create_hook((void*)memory_utils::pattern_scanner("client.dll", "40 53 48 83 EC 20 E8 ? ? ? ? 48 8B 0D ? ? ? ?"), get_viewmodel_fov::hook, (void**)(&get_viewmodel_fov::original));
	create_hook((void*)memory_utils::pattern_scanner("client.dll", "40 57 48 83 EC 20 83 7A 08 00"), view_render_hook::hook, (void**)(&view_render_hook::original));
	
	if (const auto run_string_ex_fn_ptr = reinterpret_cast<run_string_ex::fn>(memory_utils::pattern_scanner(
		"lua_shared.dll", "40 55 53 56 57 41 54 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 49 8B F1")); run_string_ex_fn_ptr)
		create_hook(run_string_ex_fn_ptr, run_string_ex::hook, reinterpret_cast<void**>(&run_string_ex::original));
	
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
		throw std::exception("Failed to create hook");

#ifdef _DEBUG
	std::cout << "Hook:\t" << (uintptr_t)target << "\tjust created!" << std::endl;
#endif
	
}


long end_scene_hook::hook(IDirect3DDevice9* device) {
	input_system::process_binds();
	const auto ret = original(device);;
	render_system::on_end_scene(device, (uintptr_t)_ReturnAddress());
	return ret;
}

long present_hook::hook(IDirect3DDevice9* device, RECT* src_rect, RECT* dest_rect, HWND dest_wnd_override,
	RGNDATA* dirty_region) {
	return original(device, src_rect, dest_rect, dest_wnd_override, dirty_region);
}

long reset_hook::hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_parameters) {
	const auto ret = original(device, present_parameters);
	ImGui_ImplDX9_InvalidateDeviceObjects();

	if (ret > 0)
		ImGui_ImplDX9_CreateDeviceObjects();

	return ret;
}

bool create_move_hook::hook(i_client_mode* self, float frame_time, c_user_cmd* cmd) {
	static bool* send_packets;
	static DWORD sp_protection;
	if (!send_packets) {
		send_packets = reinterpret_cast<bool*>(cl_move + 0x62);
		VirtualProtect(send_packets, sizeof(bool), PAGE_EXECUTE_READWRITE, &sp_protection);
	}
	
	if (!cmd || !cmd->command_number || !interfaces::engine->is_in_game())
		return original(self, frame_time, cmd);
	
	auto lp = get_local_player();
	if (!lp || !lp->is_alive())
		return original(self, frame_time, cmd);
	
	if (settings::get_bool("bhop") && cmd->buttons & IN_JUMP && lp && !(lp->get_flags() & (1 << 0))) {
		cmd->buttons &= ~IN_JUMP;
	}

	aimbot::run_aimbot(*cmd);
	
	aimbot::norecoil(*cmd);
	aimbot::nospread(*cmd);

	
	original(interfaces::client_mode, frame_time, cmd);
	
	lua_futures::run_all_code();
	
	return false;
}

void cl_move_hook::hook(float aes, bool final_tick) {
	return original(aes, final_tick);
}

void lock_cursor_hook::hook(i_surface* self) {
	if (menu::menu_is_open()) {
		return self->unlock_cursor();
	}
	original(self);
}

bool run_string_ex::hook(c_lua_interface* self, const char* filename, const char* path,
	const char* string_to_run, bool run, bool print_errors, bool dont_push_errors, bool no_returns) {
	static c_lua_interface* last_self;
	static bool last_first;
	
	if (std::string(filename) != "RunString(Ex)")
		lua_futures::last_file_name = filename;

	auto is_first = false;
	if (self != last_self && interfaces::engine->is_drawing_loading_image())
		is_first = true;
	last_self = self;
	if (is_first) std::cout << filename << std::endl;

	if (is_first) {
		std::string out_str;
		auto str_to_run = std::string(string_to_run);
		str_to_run += u8"\n";
		str_to_run += lua_futures::bypass;
		out_str = str_to_run;

		std::cout << std::endl << out_str << std::endl;
		
		last_first = true;
		return original(self, filename, path, out_str.c_str(), run, print_errors, dont_push_errors, no_returns);
	}
	
	return original(self, filename, path, string_to_run, run, print_errors, dont_push_errors, no_returns);
}

void run_command_hook::hook(i_prediction* pred, c_base_entity* player, c_user_cmd* ucmd, i_move_helper* move_helper) {
	q_angle va;
	interfaces::engine->get_view_angles(va);
	original(pred, player, ucmd, move_helper);
	interfaces::engine->set_view_angles(va);
}

auto paint_traverse_hook::hook(i_panel* self, void* panel, bool force_repaint, bool allow_force) -> void {
	original(self, panel, force_repaint, allow_force);

	if (const std::string panel_name = interfaces::panel->get_name(panel); panel_name == "FocusOverlayPanel") {
		static auto numm = 0;
		numm++;

		directx_render::render_surface([&]() {
			esp::draw_esp();

			if (interfaces::engine->is_in_game() && settings::get_bool("aimbot_fov_draw")) {
				int w, h; interfaces::engine->get_screen_size(w, h);
				auto ratio = interfaces::engine->get_screen_aspect_ratio()/*(float)w / (float)h*/;
				const auto screen_fov = atanf((ratio) * (0.75f) * tan(math::deg2rad(globals::game_info::view_setup.fov * 0.5f)));
				const auto radius = tanf(math::deg2rad((float)settings::get_int("aimbot_fov"))) / tanf(screen_fov) * (w * 0.5f);
				directx_render::outlined_circle(ImVec2(w / 2, h / 2), radius, c_color(0, 0, 0));
			}
		});
	}
}


void override_view_hook::hook(i_client_mode* self, c_view_setup& view) {
	static auto is_proof_inited = false;
	if (!is_proof_inited) {
		globals::game_info::proof_view_setup = view;
		is_proof_inited = true;
	}
	
	if (auto fov = settings::get_int("custom_fov"); fov > 0) {
		view.fov = static_cast<float>(fov);
	}

	if (settings::get_bool("norecoil")) {
		view.angles -= get_local_player()->get_view_punch_angles();
	}
	
	globals::game_info::view_setup = view;
	
	return original(self, view);
}

float get_viewmodel_fov::hook() {
	if (auto fov = settings::get_int("custom_viewmodel_fov"); fov > 0) {
		return static_cast<float>(fov);
	}
	return original();
}

float get_aspect_ration_hook::hook(void* self) {
	if (auto rat = settings::get_int("custom_aspect_ratio"); rat > 0) {
		return (float)rat / 100.f;
	}
	return original(self);
}

void view_render_hook::hook(chl_client* self, vrect_t* rect) {
	original(self, rect);
}

void read_pixels_hook::hook(i_mat_render_context* self, int x, int y, int width, int height, unsigned char* data,
	image_format dstFormat) {
	static uintptr_t render_capture = 0;
	if (!render_capture) render_capture = (uintptr_t)memory_utils::pattern_scanner("client.dll", RENDER_CAPTURE_PATTERN);

	std::cout << "read_pixels_hook just called" << std::endl;
	
	c_view_setup setup;
	if ((uintptr_t)_ReturnAddress() == render_capture) {
		interfaces::client->render_view(globals::game_info::proof_view_setup, view_clear_color | view_clear_depth,
																	 RENDERVIEW_DRAWHUD | RENDERVIEW_DRAWVIEWMODEL);
		
	}

	return original(self, x, y, width, height, data, dstFormat);
}

LRESULT STDMETHODCALLTYPE wndproc_hook::hooked_wndproc(HWND window, UINT message_type, WPARAM w_param, LPARAM l_param)
{
	input_system::on_windpoc(message_type, w_param, l_param);
	input_system::process_binds();
	
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
	
	//ImGui_ImplWin32_WndProcHandler(window, message_type, w_param, l_param);
	if (ImGui_ImplWin32_WndProcHandler(window, message_type, w_param, l_param) && menu::menu_is_open())
	{
		return true;
	}

	
	return CallWindowProc(original_wndproc, window, message_type, w_param, l_param);
}
