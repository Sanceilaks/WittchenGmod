#include "main_window.h"

#include <imgui/im_tools.h>
#include "../widgets/widgets.h"

#include <fontawesomium.h>

#include "../../../globals.h"
#include "../../../settings/settings.h"

using namespace ImGui;

class c_wittchen_tab_bar {
	int current_idx;
	ImVec2 overlay_pos;
	ImRect tabsrect;

public:
	bool begin();
	void end();

	bool tab(const std::string& name);
};

bool c_wittchen_tab_bar::begin() {
	return false;
}

void c_wittchen_tab_bar::end() {
	
}

bool TabButton(const std::string& name, int& cur_idx) {
	return false;
}

void main_window::draw_main_window() {
	SetNextWindowSize({ 600, 400 }, ImGuiCond_Once);
	Begin(u8"Тута типа меню будет окда (мне лень идите нахуй)##MAINWINDOW", 0);

	PushFont(render_system::fonts::menu_font[2]);
	if (BeginTabBar("TMP_MAINMENUTABAR")) {
		if (BeginTabItem(ICON_FA_CROSSHAIRS " AIM##TABITEM")) {
			PushFont(render_system::fonts::menu_font[1]);
			
			WittchenCheckbox("Aimbot##AIMBOT", "aimbot");
			WittchenCheckbox("Autofire##AIMBOT", "aimbot_autofire");
			WittchenCheckbox("Silent##AIMBOT", "aimbot_silent");
			WittchenSlider("Fov##AIMBOTFOV", "aimbot_fov", {0.f, 180.f});
			SameLine();
			WittchenCheckbox("Draw##AIMBOTDOV", "aimbot_fov_draw");
			
			WittchenCheckbox("Nospread##AIMBOT", "nospread");
			WittchenCheckbox("Norecoil##AIMBOT", "norecoil");
			WittchenCheckbox("Force accuracy fire##AIMBOT", "faf");
			
			Button("Bones##AIMBONES");
			if (BeginPopupContextItem()) {
				auto& bones = settings::get_int("aimbot_bones");
				for (auto i = 1; i <= (int)e_bones::last; ++i) {
					std::string str = to_string((e_bones)i);
					if (str.empty()) continue;
					if (Selectable(std::string(str + "##AIMBONE").c_str(), (bones & i), ImGuiSelectableFlags_DontClosePopups)) {
						if (!(bones & i))
							bones |= i;
						else
							bones &= ~i;
					}
					if (bones & i) SetItemDefaultFocus();
				}
				EndPopup();
			}

			WittchenCheckbox("Fake lags##HVH", "fake_lags");
			WittchenSlider("Amount##HVH_FL", "fake_lags_amount", { 0, 21 });
			
			PopFont();
			EndTabItem();
		}
		if (BeginTabItem(ICON_FA_EYE " ESP##TABITEM")) {
			PushFont(render_system::fonts::menu_font[1]);
			
			Wittchen::DrawEspEditor();
			WittchenSlider("Custom viewmodel fov", "custom_viewmodel_fov", {0, 360});
			WittchenSlider("Custom fov", "custom_fov", { 0, 360 });
			WittchenSlider("Aspect ratio", "custom_aspect_ratio", { 0, 355 });
			
			WittchenCheckbox("Draw aimbot target", "aimbot_draw_target");

			WittchenCheckbox("Third person", "third_person");
			WittchenSlider("Third person distance", "third_person_distance", {50, 1000});
			
			PopFont();
			EndTabItem();
		}
		if (BeginTabItem(ICON_FA_WHEELCHAIR " MISC##TABITEM")) {
			PushFont(render_system::fonts::menu_font[1]);
			
			WittchenCheckbox("Bunny Hop", "bhop");
			WittchenCheckbox("Fix movement", "fixmovement");
			
			PopFont();
			EndTabItem();
		}
		
		EndTabBar();
	}
	PopFont();
	
	
	
	
	End();
}
