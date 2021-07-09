#pragma once

#include <imgui/im_tools.h>

#include "../../esp/esp.h"

namespace ImGui {
	IMGUI_API bool WittchenCheckbox(const std::string& name, const std::string& var);
	IMGUI_API void BeginApplyAlpha(float alpha);
	IMGUI_API void EndApplyAlpha();

}

namespace Wittchen {
	struct WitthcenEspStyleEditor {

	};

	WitthcenEspStyleEditor* GetWittchenEspStyleEditor();
	
	void InitializeEspStyleEditor();
	
	void ApplyStyleToBox(esp::c_esp_box& box);
}
