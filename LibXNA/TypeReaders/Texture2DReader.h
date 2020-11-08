// Copyright James Puleo 2020
// Copyright LibXNA 2020

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
#include "TextureReader.h"
#include <map>
#include <vector>

namespace LibXNA
{
    class Texture2DReader : public TextureReader
    {
    public:
        Texture2DReader(LibFruit::Stream& stream) : TextureReader(stream)
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
