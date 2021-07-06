#include "widgets.h"

#include "../../../settings/settings.h"

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
		animations[id] = ImMin(animations[id] + io.DeltaTime * 4.f, 1.f);
	} else {
		animations[id] = ImMax(animations[id] - io.DeltaTime * 4.f, 0.f);
	}

	return ret;
}
