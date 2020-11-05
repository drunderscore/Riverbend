#pragma once
#include "Viewport.h"
#include <map>

class TextureViewport : public Viewport
{
public:
    TextureViewport(Application& app);

protected:
    void draw_viewport() override;
    void draw_action_bar() override;
    void draw_properties() override;

    std::map<u32, bool> m_rendered_levels;
    std::map<u32, std::shared_ptr<Texture>> m_textures;
};
