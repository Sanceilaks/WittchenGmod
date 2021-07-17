#include "main_window.h"

#include <imgui/im_tools.h>
#include "../widgets/widgets.h"

#include <fontawesomium.h>

using namespace ImGui;

void main_window::draw_main_window() {
	SetNextWindowSize({ 600, 400 }, ImGuiCond_Once);
	Begin(u8"Тута типа меню будет окда (мне лень идите нахуй)##MAINWINDOW", 0);

	PushFont(render_system::fonts::menu_font[2]);
	if (BeginTabBar("TMP_MAINMENUTABAR")) {
		if (BeginTabItem(ICON_FA_CROSSHAIRS " AIM##TABITEM")) {
			PushFont(render_system::fonts::menu_font[1]);
			
			WittchenCheckbox("Aimbot##AIMBOT", "aimbot");
			WittchenCheckbox("Autofire##AIMBOT", "aimbot_autofire");
			WittchenSlider("Fov##AIMBOTFOV", "aimbot_fov", {0.f, 180.f});
			
			WittchenCheckbox("Nospread##AIMBOT", "nospread");
			WittchenCheckbox("Norecoil##AIMBOT", "norecoil");
			
			PopFont();
			EndTabItem();
		}
		if (BeginTabItem(ICON_FA_EYE " ESP##TABITEM")) {
			PushFont(render_system::fonts::menu_font[1]);
			
			Wittchen::DrawEspEditor();

			PopFont();
			EndTabItem();
		}
		if (BeginTabItem(ICON_FA_WHEELCHAIR " MISC##TABITEM")) {
			PushFont(render_system::fonts::menu_font[1]);
			
			WittchenCheckbox("Bunny Hop", "bhop");
			
			PopFont();
			EndTabItem();
		}
		
		EndTabBar();
	}
	PopFont();
	
	
	
	
	End();
}
