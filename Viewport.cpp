// Copyright James Puleo 2020
// Copyright Riverbend 2020

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#define NOMINMAX
#define IMGUI_DEFINE_MATH_OPERATORS
#include "Viewport.h"
#include "imgui/imgui.h"
#include <sstream>
#include "imgui/imgui_internal.h"

void Viewport::draw()
{
    if(ImGui::Begin("Properties", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        if(ImGui::BeginTable("PropertiesTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingPolicyFixedX))
        {
            draw_properties();
            ImGui::EndTable();
        }
    }
    ImGui::End();

    if(ImGui::BeginMainMenuBar())
    {
        draw_action_bar();
        ImGui::EndMainMenuBar();
    }

    if(m_app.loaded_reader())
        draw_viewport();
}

void Viewport::draw_viewport() {}

void Viewport::draw_action_bar()
{
    if(ImGui::BeginMenu("View"))
    {
        ImGui::SetNextItemWidth(150.0f);
        ImGui::SliderFloat("Scale", &m_scale, 0.25f, 20.0f);
        if(ImGui::MenuItem("Re-center"))
            m_offset = ImVec2();

        ImGui::EndMenu();
    }
}

void Viewport::draw_properties()
{
    auto xnb = m_app.loaded_xnb();
    insert_key_value("Target Platform", "%s", LibXNA::get_target_platform_name(xnb->header().m_target_platform));
    insert_key_value("Format Version", "%d", xnb->header().m_format_version);

    std::stringstream flags_text;
    if(xnb->is_hi_def())
        flags_text << "Hi-Def";
    if(xnb->is_compressed())
    {
        if(xnb->is_hi_def())
            flags_text << ", ";
        flags_text << "Compressed";
    }
    insert_key_value("Flags", "%s", flags_text.str().c_str());

    insert_key_value("Total Size", "%s", xnb->total_size().most_fitting().c_str());
    if(auto decompressed_size = xnb->decompressed_size())
        insert_key_value("Decompressed Size", "%s", decompressed_size->most_fitting().c_str());

    // TODO: include fully qualified
    if(auto reader = xnb->primary_object())
        insert_key_value("Type Reader", "%s", reader->reader().class_name().c_str());

}

void Viewport::poll(SDL_Event& e)
{
    if(ImGui::GetCurrentContext()->HoveredWindow)
        return;

    if((e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) && e.button.button == SDL_BUTTON_LEFT)
        m_dragging = e.type == SDL_MOUSEBUTTONDOWN;

    if(m_dragging && e.type == SDL_MOUSEMOTION)
    {
        m_offset.x += e.motion.xrel;
        m_offset.y += e.motion.yrel;
    }

    if(e.type == SDL_MOUSEWHEEL)
        m_scale = std::max(0.25f, m_scale + (e.wheel.y * 0.125f));
}

void Viewport::draw_texture(Texture& t, ImVec2& pos)
{
    auto min = pos + m_offset;
    auto max = (pos + ImVec2(t.width(), t.height()) * m_scale) + m_offset;
    ImGui::GetBackgroundDrawList()->AddImage(reinterpret_cast<void*>(t.id()), min, max);
}

void Viewport::draw_rect(ImVec2& pos, ImVec2& size, u32 col, float thick)
{
    ImGui::GetBackgroundDrawList()->AddRect(pos + m_offset, (pos + size * m_scale) + m_offset, col, 0.0f, ImDrawCornerFlags_All, thick);
}
