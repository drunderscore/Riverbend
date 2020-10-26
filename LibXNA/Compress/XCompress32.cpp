// See XCompress32.h for more information on why this is disabled.
#if 0
#include "XCompress32.h"

namespace LibXNA
{
    bool XCompress32::m_has_loaded_library = false;
    HMODULE XCompress32::m_library = nullptr;
    XCompress32::XMemCreateDecompressionContext_fn XCompress32::m_create_decompression_context = nullptr;
    XCompress32::XMemResetDecompressionContext_fn XCompress32::m_reset_decompression_context = nullptr;
    XCompress32::XMemDecompress_fn XCompress32::m_decompress = nullptr;
    XCompress32::XMemDestroyDecompressionContext_fn XCompress32::m_destroy_decompression_context = nullptr;


    bool XCompress32::load()
    {
        constexpr const char* LIBRARY_NAME = "xcompress32.dll";

        if(m_has_loaded_library)
            return true;

        if(!(m_library = LoadLibrary(LIBRARY_NAME)))
            return false;
        m_has_loaded_library = true;

        m_create_decompression_context = reinterpret_cast<XMemCreateDecompressionContext_fn>(GetProcAddress(m_library, "XMemCreateDecompressionContext"));
        m_reset_decompression_context = reinterpret_cast<XMemResetDecompressionContext_fn>(GetProcAddress(m_library, "XMemResetDecompressionContext"));
        m_decompress = reinterpret_cast<XMemDecompress_fn>(GetProcAddress(m_library, "XMemDecompress"));
        m_destroy_decompression_context = reinterpret_cast<XMemDestroyDecompressionContext_fn>(GetProcAddress(m_library, "XMemDestroyDecompressionContext"));

        return m_create_decompression_context && m_reset_decompression_context && m_decompress && m_destroy_decompression_context;
    }
}
#endif // 0
