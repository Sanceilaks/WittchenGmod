#pragma once

#include <string>
#include <imgui/im_tools.h>

#include "../../game_sdk/entities/c_base_entity.h"

namespace esp {
	struct esp_text_t {
		std::string text;
		ImVec2 position;
		int flags;
	};
	
	class c_esp_box {
		//ImVec2
	public:
		ImVec2 min, max;
		
		void get_absolute_position(const ImVec2& r);

		static bool calc_box(c_base_entity* ent, c_esp_box& box);
	};

	
	void draw_esp();
}
