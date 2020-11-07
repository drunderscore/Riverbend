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

#include "TextureViewport.h"
#include "TypeReaders/Texture2DReader.h"
#include "loguru/loguru.hpp"

TextureViewport::TextureViewport(Application& app) : Viewport(app)
{
    auto texture_reader = dynamic_cast<LibXNA::Texture2DReader*>(app.loaded_reader().get());
    CHECK_F(texture_reader != nullptr, "Couldn't get Texture2DReader from application");

    auto type = app.loaded_xnb()->header().m_target_platform == LibXNA::TargetPlatform::Xbox ? Texture::Type::UnsignedInt8 : Texture::Type::UnsignedInt8Reverse;
    for(auto &kv : texture_reader->data())
    {
        u32 width_scale = kv.first == 0 ? 1 : std::pow(2, kv.first);
        u32 height_scale = kv.first == 0 ? 1 : std::pow(2, kv.first);
        m_textures.emplace(kv.first, std::make_shared<Texture>(kv.second.data(), texture_reader->width() / width_scale, texture_reader->height() / height_scale,
                                                               Texture::Format::RGBA, type));
        m_rendered_levels.emplace(kv.first, kv.first == 0);
    }
}

void TextureViewport::draw_viewport()
{
    Viewport::draw_viewport();
    auto pos = ImVec2(0, 0);
    for(auto i = 0u; i < m_textures.size(); i++)
    {
        if(m_rendered_levels.at(i))
        {
            auto tex = m_textures.at(i);
            draw_texture(*tex, pos);
            if(m_show_outline)
            {
                auto size = ImVec2(tex->width(), tex->height());
                draw_rect(pos, size, m_outline_color, 1.0f);
            }
        }
    }
}

void TextureViewport::draw_properties()
{
    Viewport::draw_properties();
    auto reader = reinterpret_cast<LibXNA::Texture2DReader*>(m_app.loaded_reader().get());
    insert_key_value("Width", "%d", reader->width());
    insert_key_value("Height", "%d", reader->height());
    insert_key_value("Levels", "%d", reader->levels());
    insert_key_value("Surface (?)", "%s", LibXNA::get_surface_format_name(reader->format()), LibXNA::get_surface_format_description(reader->format()));
}

void TextureViewport::draw_action_bar()
{
    Viewport::draw_action_bar();

    if(ImGui::BeginMenu("View"))
    {
        if(ImGui::BeginMenu("Levels"))
        {
            for(auto& should_render : m_rendered_levels)
                ImGui::Checkbox(std::to_string(should_render.first).c_str(), &should_render.second);

            ImGui::EndMenu();
        }

        ImGui::MenuItem("Show Outline", nullptr, &m_show_outline);
        ImGui::EndMenu();
    }
}
