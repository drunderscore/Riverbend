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

#pragma once
#include "imgui/imgui.h"
#include "Application.h"
#include "SDL.h"
#include "Texture.h"
#include "LibXNA/XNB.h"
#include "LibXNA/TypeReaders/Reader.h"

class Viewport
{
public:
    Viewport(Application& app) : m_app(app) {}

    virtual void draw();
    virtual void poll(SDL_Event&);

protected:
    virtual void draw_viewport();
    virtual void draw_properties();
    template<typename T>
    void insert_key_value(const char* key, const char* specifier, T val, const char* tooltip = nullptr)
    {
        auto hovered = false;
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("%s", key);
        hovered |= ImGui::IsItemHovered();
        ImGui::TableNextColumn();
        ImGui::Text(specifier, val);
        hovered |= ImGui::IsItemHovered();

        if(hovered && tooltip)
            ImGui::SetTooltip("%s", tooltip);
    }
    virtual void draw_action_bar();

    virtual void draw_texture(Texture&, ImVec2& pos);
    ImVec2 m_offset;
    float m_scale = 1.0f;
    Application& m_app;

private:
    bool m_dragging = false;
};
