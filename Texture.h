#pragma once
#include "GL/gl3w.h"
#include "Types.h"

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
