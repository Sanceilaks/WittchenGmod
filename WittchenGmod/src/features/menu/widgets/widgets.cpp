#include "widgets.h"

#include "../../../settings/settings.h"
#include "../menu.h"

using namespace ImGui;

bool ImGui::WittchenCheckbox(const std::string& name, const std::string& var) {
	static float animations[100000000];
	
	auto& v = settings::get_bool(var);
	auto id = GenerateAnimationId();
	auto io = GetIO();

	auto bc = GetStyleColorVec4(ImGuiCol_Border);
	auto bcs = GetStyleColorVec4(ImGuiCol_BorderShadow);
	bc.w = animations[id];
	bcs.w = 0.f;
	
	PushStyleColor(ImGuiCol_Border, bc);
	PushStyleColor(ImGuiCol_BorderShadow, bcs);
	PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.f);
	auto ret = Checkbox(name.c_str(), &v);
	PopStyleVar();
	PopStyleColor(2);
	
	if (IsItemHovered()) {
		animations[id] = ImMin(animations[id] + io.DeltaTime * ELEMENT_FADE_ANIMATION_SPEED, 1.f);
	} else {
		animations[id] = ImMax(animations[id] - io.DeltaTime * ELEMENT_FADE_ANIMATION_SPEED, 0.f);
	}

	return ret;
}

void ImGui::BeginApplyAlpha(float alpha) {
	alpha = menu::menu_alpha == 1.f ? alpha : menu::menu_alpha;
	PushStyleVar(ImGuiStyleVar_Alpha, alpha);
}

void ImGui::EndApplyAlpha() {
	PopStyleVar();
}
