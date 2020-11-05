#pragma once
#include "Types.h"

namespace LibXNA
{
    enum class SurfaceFormat : u32
    {
        Color,
        Bgr565,
        Bgra5551,
        Bgra4444,
        Dxt1,
        Dxt3,
        Dxt5,
        NormalizedByte2,
        NormalizedByte4,
        Rgba1010102,
        Rg32,
        Rgba64,
        Alpha8,
        Single,
        Vector2,
        Vector4,
        HalfSingle,
        HalfVector2,
        HalfVector4,
        HdrBlendable
    };

    // Direct translation from the above enum
    const char* get_surface_format_name(SurfaceFormat fmt);

    // Describes how the memory would represent the data in this format
    const char* get_surface_format_description(SurfaceFormat fmt);
}
