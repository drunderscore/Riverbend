#pragma once
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
