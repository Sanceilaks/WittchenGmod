#include "input_system.h"
#include <imgui/imgui.h>

#include <map>
#include <string>
#include <vector>
#include <array>

#include <mutex>

#include <Windows.h>

std::array<bool, 256> keys;
std::array<uint32_t, 256> frames;
uint32_t current_frame;

std::mutex input_mutex;

void input_system::initialize_input_system() {

}

void input_system::add_bind(int64_t id, uint32_t key, bool* var, bind_system::bind_type type) {
	bind_system::bool_binds[id].push_back({(int)type, key, var, false});
}

bool input_system::is_key_pressed(int key, bool null_is_true)
{
	std::unique_lock l(input_mutex);

	if (key >= 256)
		return false;

	if (null_is_true)
		return keys[key] || key == 0;
	return keys[key];
}

bool input_system::is_key_just_pressed(int key, int delay, bool null_is_true)
{
	std::unique_lock l(input_mutex);

	if (key >= 256)
		return false;

	if (null_is_true)
		return (keys[key] && frames[key] + delay / 2 >= current_frame) || key == 0;
	return keys[key] && frames[key] + delay / 2 >= current_frame;
}

bool resolve_type_as_bind(bind_system::bind_type type, uint32_t key) {
	switch (type) {
	case bind_system::bind_type::none:
		return false;
	case bind_system::bind_type::active:
		return true;
	case bind_system::bind_type::pressed:
		return input_system::is_key_pressed(key);
	case bind_system::bind_type::toggle:
		return input_system::is_key_just_pressed(key, 1);
	default:
			return false;
	}
	
}

void input_system::process_binds() {
	//std::unique_lock l(input_mutex);
	
	for (auto& i : bind_system::bool_binds)
		for (auto& j : i.second)
			*j.value = resolve_type_as_bind((bind_system::bind_type)j.type, j.key);
}

void input_system::on_windpoc(int msg, int wparam, int lparam)
{
	std::unique_lock l(input_mutex);

	current_frame++;

	if (msg == WM_SYSKEYDOWN || msg == WM_KEYDOWN) {
		if (wparam < 256) {
			keys[wparam] = true;
			frames[wparam] = current_frame;
		}
	}
	else if (msg == WM_SYSKEYUP || msg == WM_KEYUP) {
		if (wparam < 256) {
			keys[wparam] = false;
			frames[wparam] = current_frame;
		}
	}

	if (msg == WM_XBUTTONDOWN || msg == WM_XBUTTONDBLCLK || msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK || msg == WM_RBUTTONDOWN || msg == WM_RBUTTONDBLCLK)
	{
		int button = 0;
		if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK) button = 1;
		if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONDBLCLK) button = 2;
		if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONDBLCLK) button = 3;
		if (msg == WM_XBUTTONDOWN || msg == WM_XBUTTONDBLCLK) button = (HIWORD(wparam) == XBUTTON1) ? 5 : 4;
		keys[button] = true;
		frames[button] = current_frame;
	}
	else if (msg == WM_XBUTTONUP || msg == WM_LBUTTONUP || msg == WM_RBUTTONUP) {
		int button = 0;
		if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK) button = 1;
		if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONDBLCLK) button = 2;
		if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONDBLCLK) button = 3;
		if (msg == WM_XBUTTONDOWN || msg == WM_XBUTTONDBLCLK) button = (HIWORD(wparam) == XBUTTON1) ? 5 : 4;
		keys[button] = false;
		frames[button] = current_frame;
	}
}
