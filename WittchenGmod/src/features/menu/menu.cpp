#include "menu.h"

#include <imgui/imgui.h>

bool is_menu_open;

void menu::initialize_menu() {
	
}

void menu::draw_menu() {
	if (is_menu_open)
	{
		//interfaces::input_system->enable_input(true);

		//if (!interfaces::surface->is_cursor_visible())
			ImGui::GetIO().MouseDrawCursor = true;

		//imgui_overlay::draw();

		//bg_window::draw();
		//main_window::draw();
	}
	else
	{
		ImGui::GetIO().MouseDrawCursor = false;
	}
}

bool menu::menu_is_open() {
	return is_menu_open;
}

void menu::set_open_state(bool state) {
	is_menu_open = state;
}

void menu::toggle_menu() {
	is_menu_open = !is_menu_open;
}

