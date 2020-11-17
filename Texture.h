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
#include "GL/gl3w.h"
#include <LibFruit/Types.h>

class Texture
{
public:
    enum class Format : u32
    {
        RGBA = GL_RGBA,
        BGRA = GL_BGRA,
        RGB = GL_RGB,
        BGR = GL_BGR,
    };

    enum class Type : u32
    {
        UnsignedByte = GL_UNSIGNED_BYTE,
        Byte = GL_BYTE,
        UnsignedInt = GL_UNSIGNED_INT,
        UnsignedInt8Reverse = GL_UNSIGNED_INT_8_8_8_8_REV,
        UnsignedInt8 = GL_UNSIGNED_INT_8_8_8_8,
        Int = GL_INT
    };

    Texture(const u8* data, u32 width, u32 height, Format fmt = Format::RGBA, Type t = Type::UnsignedByte, u32 count = 1);
    ~Texture();

    bool operator==(const Texture &b) const
    {
        return m_id == b.m_id;
    }

    bool operator!=(const Texture &b) const
    {
        return !(m_id == b.m_id);
    }

    bool operator>(const Texture &b) const
    {
        return m_id > b.m_id;
    }

    bool operator<(const Texture &b) const
    {
        return b.m_id > m_id;
    }

    u32 width() { return m_width; }
    u32 height() { return m_height; }
    GLuint id() { return m_id; }
private:
    GLuint m_id;
    u32 m_width, m_height;
    u32 m_number_of_textures;
};
