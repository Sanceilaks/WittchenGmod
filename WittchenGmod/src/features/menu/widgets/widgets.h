#pragma once

#include <imgui/im_tools.h>

namespace ImGui {
	IMGUI_API bool WittchenCheckbox(const std::string& name, const std::string& var);
	IMGUI_API void BeginApplyAlpha(float alpha);
	IMGUI_API void EndApplyAlpha();
	
}
