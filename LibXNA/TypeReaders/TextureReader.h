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
