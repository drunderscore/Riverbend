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
                u32 buffer_size = stream.read<u32>();
                std::vector<u8> data;
                for(auto i = 0u; i < buffer_size; i++)
                    data.push_back(stream.read<u8>());
                m_data[i] = data;
            }
        }

        u32 levels() { return m_levels; }
        bool has_mipmaps() { return m_levels > 1; }
        const std::map<u32, std::vector<u8>>& data() { return m_data; }

    protected:
        u32 m_levels;
        std::map<u32, std::vector<u8>> m_data;

        const char* type_name() const override
        {
            return "Microsoft.Xna.Framework.Content.Texture2DReader";
        }
    };
}
