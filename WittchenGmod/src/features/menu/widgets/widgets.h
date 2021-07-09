#pragma once

#include <mutex>
#include <imgui/im_tools.h>

#include "../../esp/esp.h"
#include "../../../game_sdk/entities/c_base_player.h"

namespace ImGui {
	IMGUI_API bool WittchenCheckbox(const std::string& name, const std::string& var);
	IMGUI_API void BeginApplyAlpha(float alpha);
	IMGUI_API void EndApplyAlpha();

}

namespace Wittchen {
	struct WitthcenEspStyleEditor {
		struct TextStorage {
			std::string text;
			int flags;
			float size;
			c_color color;
			bool is_auto_color = false;
		};
		struct Strings {
			std::vector<TextStorage> top;
			std::vector<TextStorage> bottom;
			std::vector<TextStorage> right;
			std::vector<TextStorage> left;

			void Clear();
		};

		Strings strings;
		std::mutex esp_editor_mutex;
		
		WitthcenEspStyleEditor() = default;

		static c_color GetAutoColor(const std::string& name, c_base_entity* entity);
		static c_color GetAutoColor(const std::string& name, c_base_player* player);
	};

	WitthcenEspStyleEditor* GetWittchenEspStyleEditor();
	
	void InitializeEspStyleEditor();
	
	void ApplyStyleToBox(esp::c_esp_box& box);

	void DrawEspPreview();
}
