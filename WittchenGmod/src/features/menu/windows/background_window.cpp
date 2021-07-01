#include "background_window.h"

#include <imgui/im_tools.h>

using namespace ImGui;

void background_window::draw_background_window() {
	if (BeginMainMenuBar()) {

		EndMainMenuBar();
	}

	

	Begin(u8"Измени мой стайл))))");

	ShowStyleEditor();

	End();
}
