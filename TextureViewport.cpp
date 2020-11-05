#include "TextureViewport.h"
#include "TypeReaders/Texture2DReader.h"
#include "loguru/loguru.hpp"

TextureViewport::TextureViewport(Application& app) : Viewport(app)
{
    auto texture_reader = dynamic_cast<LibXNA::Texture2DReader*>(app.loaded_reader().get());
    CHECK_F(texture_reader != nullptr, "Couldn't get Texture2DReader from application");
    for(auto &kv : texture_reader->data())
    {
        u32 width_scale = kv.first == 0 ? 1 : std::pow(2, kv.first);
        u32 height_scale = kv.first == 0 ? 1 : std::pow(2, kv.first);
        m_textures.emplace(kv.first, std::make_shared<Texture>(kv.second.data(), texture_reader->width() / width_scale, texture_reader->height() / height_scale));
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
            draw_texture(*m_textures.at(i), pos);
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
        ImGui::EndMenu();
    }
}