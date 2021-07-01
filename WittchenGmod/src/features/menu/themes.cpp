#include "themes.h"

#include <imgui/im_tools.h>

using namespace ImGui;

void menu_themes::standard_theme() {
	auto& io = GetIO;
	auto& style = GetStyle();

	style.WindowPadding = { 0, 0 };
	style.FrameRounding = 8.f;
	style.GrabMinSize = 10.f;
	style.ItemSpacing = { 20.f, 3.f };
	style.ItemInnerSpacing = { 8.f, 0.f };

	style.FrameBorderSize = 0.f;
	style.PopupBorderSize = 0.f;
	style.WindowBorderSize = 0.f;

	style.SliderThickness = 0.3f;
}
