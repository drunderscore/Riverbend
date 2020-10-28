#pragma once
#include "Reader.h"
#include "SurfaceFormat.h"

namespace LibXNA
{
    class TextureReader : public Reader
    {
    public:
        TextureReader(Stream& stream)
        {
            stream.read(m_format);
            stream.read(m_width);
            stream.read(m_height);
        }

        u32 width() { return m_width; }
        u32 height() { return m_height; }
        SurfaceFormat format() { return m_format; }

    protected:
        SurfaceFormat m_format;
        u32 m_width, m_height;
    };
}
