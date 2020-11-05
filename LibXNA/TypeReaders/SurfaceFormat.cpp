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

#include "SurfaceFormat.h"

namespace LibXNA
{
    static const char* surface_format_names[] =
    {
        "Color",
        "Bgr565",
        "Bgra5551",
        "Bgra4444",
        "Dxt1",
        "Dxt3",
        "Dxt5",
        "NormalizedByte2",
        "NormalizedByte4",
        "Rgba1010102",
        "Rg32",
        "Rgba64",
        "Alpha8",
        "Single",
        "Vector2",
        "Vector4",
        "HalfSingle",
        "HalfVector2",
        "HalfVector4",
        "HdrBlendable"
    };

    const char* get_surface_format_name(SurfaceFormat fmt)
    {
        return surface_format_names[static_cast<int>(fmt)];
    }

    // Most of these descriptions are taken from
    // https://docs.microsoft.com/en-us/windows/win32/direct3d9/d3dformat
    const char* get_surface_format_description(SurfaceFormat fmt)
    {
        switch(fmt)
        {
            case SurfaceFormat::Color:
                return "32-bit ARGB pixel format with alpha, using 8 bits per channel";
            case SurfaceFormat::Dxt1:
            case SurfaceFormat::Dxt3:
            case SurfaceFormat::Dxt5:
                return "Direct X (DXTn) compressed texture";
            default:
                return get_surface_format_name(fmt);
        }
    }
}
