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
