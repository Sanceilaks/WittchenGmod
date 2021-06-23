#include "render_system.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx9.h>

#include "imgui/imgui_freetype.h"

IDirect3DDevice9* game_device;
bool initialized = false;

void render_system::init() {
    ImGui::CreateContext();

    auto* const game_hwnd = FindWindowW(0, L"Garry's Mod (x64)");
    if (game_hwnd)
    {
        ImGui_ImplWin32_Init(game_hwnd);
        ImGui_ImplDX9_Init(game_device);

        ImGui::GetIO().IniFilename = nullptr;

        //ImGuiFreeType::BuildFontAtlas(ImGui::GetIO().Fonts);
    }

	initialized = true;
}

IDirect3DDevice9* render_system::get_device() {
	return game_device;
}

void render_system::on_end_scene(LPDIRECT3DDEVICE9 device, uintptr_t return_address) {
	if (!game_device) game_device = device;
	if (!initialized) {
		init();
	}

    IDirect3DStateBlock9* pixel_state = NULL;
    IDirect3DVertexDeclaration9* vertex_declaration;
    IDirect3DVertexShader9* vertex_shader;
    device->CreateStateBlock(D3DSBT_PIXELSTATE, &pixel_state);
    device->GetVertexDeclaration(&vertex_declaration);
    device->GetVertexShader(&vertex_shader);
    device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);

    DWORD state1;
    DWORD state2;
    DWORD state3;
    device->GetRenderState(D3DRS_COLORWRITEENABLE, &state1);
    device->GetRenderState(D3DRS_COLORWRITEENABLE, &state2);
    device->GetRenderState(D3DRS_SRGBWRITEENABLE, &state3);

    device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
    device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
    device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);


    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    //menu::draw();

    //auto* list = ImGui::GetBackgroundDrawList();
    //directx_render::add_temp_to_draw_list(list);

    ImGui::GetBackgroundDrawList()->AddRectFilledMultiColor({0, 0}, {100, 100}, ImColor(255, 0, 0),
                                                            ImColor(0, 255, 255), ImColor(0, 255, 0),
                                                            ImColor(255, 255, 255));
	
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    device->SetRenderState(D3DRS_COLORWRITEENABLE, state1);
    device->SetRenderState(D3DRS_COLORWRITEENABLE, state2);
    device->SetRenderState(D3DRS_SRGBWRITEENABLE, state3);

    pixel_state->Apply();
    pixel_state->Release();
    device->SetVertexDeclaration(vertex_declaration);
    device->SetVertexShader(vertex_shader);
}