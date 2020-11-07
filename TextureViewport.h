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
    bool m_show_outline = false;
    u32 m_outline_color = 0xFFFF00FF;
};
