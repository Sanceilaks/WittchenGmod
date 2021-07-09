#pragma once

#include <string>
#include <imgui/im_tools.h>

#include "../../game_sdk/entities/c_base_entity.h"

namespace esp {
	struct esp_text_t {
		std::string text;
		int flags;
		float size;
		c_color color;
		bool is_auto_color;
	};
	struct esp_text_storage_t {
		std::pair<ImVec2, std::vector<esp_text_t>> top;
		std::pair<ImVec2, std::vector<esp_text_t>> down;
		std::pair<ImVec2, std::vector<esp_text_t>> right;
		std::pair<ImVec2, std::vector<esp_text_t>> left;
	};
	
	class c_esp_box {
	public:
		esp_text_storage_t strings;
		ImVec2 min, max;
		float rounding;
		c_color color;
		
		void get_absolute_position(const ImVec2& r);
		ImVec2 get_screen_position(const ImVec2& pos);
		
		static bool calc_box(c_base_entity* ent, c_esp_box& box);
	};

	
	void draw_esp();
}
