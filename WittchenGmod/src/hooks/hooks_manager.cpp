#include "hooks_manager.h"

#include <memory>

#include <minpp/c_min_hook.h>

#include "imgui/imgui_impl_dx9.h"

#include <Windows.h>

#include <fmt/format.h>

inline unsigned int get_virtual(void* _class, const unsigned int index) { return static_cast<unsigned int>((*static_cast<int**>(_class))[index]); }
std::shared_ptr<min_hook_pp::c_min_hook> minpp = nullptr;

#define CREATE_HOOK(_class, index, detour, original) create_hook(reinterpret_cast<void*>(get_virtual(_class, index)), \
	detour, reinterpret_cast<void**>(&original));

struct WndprocHook
{
	static LRESULT STDMETHODCALLTYPE hookedWndproc(HWND window, UINT message_type, WPARAM w_param, LPARAM l_param);
	static inline WNDPROC originalWndproc = nullptr;

}; //extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void hooks_manager::init() {
	minpp = std::make_shared<min_hook_pp::c_min_hook>();



	auto* const game_hwnd = FindWindowW(0, L"Garry's Mod (x64)");
	WndprocHook::originalWndproc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(
		game_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndprocHook::hookedWndproc)));
	
	minpp->enable_hook();
}

void hooks_manager::shutdown() {
	minpp->unhook();
	minpp.reset();

	auto* const game_hwnd = FindWindowW(0, L"Garry's Mod (x64)");
	if (game_hwnd)
		SetWindowLongPtr(game_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndprocHook::originalWndproc));
}

void hooks_manager::create_hook(void* target, void* detour, void** original) {
	const auto result = minpp->create_hook(target, detour, original);
	if (!result)
		throw std::exception(fmt::format("Failed to create hook. Target: {}, detour: {}, original: {}",
			reinterpret_cast<uintptr_t>(target), reinterpret_cast<uintptr_t>(detour),
			reinterpret_cast<uintptr_t>(*original)).c_str());
}



LRESULT STDMETHODCALLTYPE WndprocHook::hookedWndproc(HWND window, UINT message_type, WPARAM w_param, LPARAM l_param)
{

	//ImGui_ImplWin32_WndProcHandler(window, message_type, w_param, l_param);
	//if (ImGui_ImplWin32_WndProcHandler(window, message_type, w_param, l_param))
	//{
		//interfaces::surface->unlock_cursor();
		//return true;
	//}

	return CallWindowProc(originalWndproc, window, message_type, w_param, l_param);
}