#pragma once

#include <string>
#include <imgui/im_tools.h>

namespace esp {
	struct esp_text_t {
		std::string text;
		ImVec2 position;
		int flags;
	};
	
	void draw_esp();
}
