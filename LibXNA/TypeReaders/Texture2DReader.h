#pragma once
#include "TextureReader.h"
#include <map>
#include <vector>

namespace LibXNA
{
    class Texture2DReader : public TextureReader
    {
    public:
        Texture2DReader(Stream& stream) : TextureReader(stream)
        {
            stream.read(m_levels);
            for(auto i = 0u; i < m_levels; i++)
            {
                auto buffer_size = stream.read<u32>();
                std::vector<u8> data(buffer_size);
                stream.read(&data[0], buffer_size);
                m_data.emplace(i, data);
            }
        }

        u32 levels() { return m_levels; }
        bool has_mipmaps() { return m_levels > 1; }
        const std::map<u32, std::vector<u8>>& data() { return m_data; }

        static const char* type_name()
        {
            return "Microsoft.Xna.Framework.Content.Texture2DReader";
        }

    protected:
        u32 m_levels;
        std::map<u32, std::vector<u8>> m_data;
    };
}
