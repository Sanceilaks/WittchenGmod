#include "widgets.h"

#include "../../../settings/settings.h"
#include "../menu.h"

#include "../../esp/esp.h"

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

void MergeTextStorage(std::vector<Wittchen::WitthcenEspStyleEditor::TextStorage>& w, std::vector<esp::esp_text_t>& e) {
	e.clear();
	for (auto& i : w) {
		e.emplace_back(esp::esp_text_t{ i.text, i.flags, i.size, i.color, i.is_auto_color});
	}
}

static Wittchen::WitthcenEspStyleEditor g_style_editor;

void Wittchen::WitthcenEspStyleEditor::Strings::Clear() {
	top.clear();
	right.clear();
	left.clear();
	bottom.clear();
}

c_color Wittchen::WitthcenEspStyleEditor::GetAutoColor(const std::string& name, c_base_entity* entity) {
	return colors::white_color;
}

c_color Wittchen::WitthcenEspStyleEditor::GetAutoColor(const std::string& name, c_base_player* player) {
	if (name == "%health") {
		float g = 255 * (player->get_health_procentage() / 100.f);
		return { 255 - g, g, 0 };
	}
	//TODO: IMPL OTHER

	return colors::white_color;
}

Wittchen::WitthcenEspStyleEditor* Wittchen::GetWittchenEspStyleEditor() {
	return &g_style_editor;
}

void Wittchen::InitializeEspStyleEditor() {
}

void Wittchen::ApplyStyleToBox(esp::c_esp_box& box) {
	std::unique_lock l(g_style_editor.esp_editor_mutex);
	
	//box style
	box.color = colors::red_color;
	box.rounding = 0.f;

	g_style_editor.strings.top.emplace_back(WitthcenEspStyleEditor::TextStorage{ "%name", 0, 13.f, colors::white_color});
	g_style_editor.strings.bottom.emplace_back(WitthcenEspStyleEditor::TextStorage{ "%name", 0, 13.f, colors::white_color});
	g_style_editor.strings.left.emplace_back(WitthcenEspStyleEditor::TextStorage{ "%name", 0, 13.f, colors::white_color});
	g_style_editor.strings.right.emplace_back(WitthcenEspStyleEditor::TextStorage{ "%name", 0, 13.f, colors::white_color});

	g_style_editor.strings.top.emplace_back(WitthcenEspStyleEditor::TextStorage{ "%health", 0, 13.f, colors::white_color, true});
	g_style_editor.strings.bottom.emplace_back(WitthcenEspStyleEditor::TextStorage{ "%health", 0, 13.f, colors::white_color , true });
	g_style_editor.strings.left.emplace_back(WitthcenEspStyleEditor::TextStorage{ "%health", 0, 13.f, colors::white_color , true });
	g_style_editor.strings.right.emplace_back(WitthcenEspStyleEditor::TextStorage{ "%health", 0, 13.f, colors::white_color , true });
	
	//text applying
	MergeTextStorage(g_style_editor.strings.right, box.strings.right.second);
	MergeTextStorage(g_style_editor.strings.left, box.strings.left.second);
	MergeTextStorage(g_style_editor.strings.top, box.strings.top.second);
	MergeTextStorage(g_style_editor.strings.bottom, box.strings.down.second);

	//clear text storage for next frame
	g_style_editor.strings.Clear();
}
