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

#include "XCompress.h"

namespace LibXNA
{
    bool XCompress::m_has_loaded_library = false;
    HMODULE XCompress::m_library = nullptr;
    XCompress::XMemCreateDecompressionContext_fn XCompress::m_create_decompression_context = nullptr;
    XCompress::XMemResetDecompressionContext_fn XCompress::m_reset_decompression_context = nullptr;
    XCompress::XMemDecompress_fn XCompress::m_decompress = nullptr;
    XCompress::XMemDestroyDecompressionContext_fn XCompress::m_destroy_decompression_context = nullptr;

    XCompress::XMemCreateCompressionContext_fn XCompress::m_create_compression_context = nullptr;
    XCompress::XMemResetCompressionContext_fn XCompress::m_reset_compression_context = nullptr;
    XCompress::XMemCompress_fn XCompress::m_compress = nullptr;
    XCompress::XMemDestroyCompressionContext_fn XCompress::m_destroy_compression_context = nullptr;

    bool XCompress::load()
    {
#ifdef _WIN64
        constexpr const char* LIBRARY_NAME = "xcompress64.dll";
#else
        constexpr const char* LIBRARY_NAME = "xcompress32.dll";
#endif // _WIN64

        if(m_has_loaded_library)
            return true;

        if(!(m_library = LoadLibrary(LIBRARY_NAME)))
            return false;
        m_has_loaded_library = true;

        m_create_decompression_context = reinterpret_cast<XMemCreateDecompressionContext_fn>(GetProcAddress(m_library, "XMemCreateDecompressionContext"));
        m_reset_decompression_context = reinterpret_cast<XMemResetDecompressionContext_fn>(GetProcAddress(m_library, "XMemResetDecompressionContext"));
        m_decompress = reinterpret_cast<XMemDecompress_fn>(GetProcAddress(m_library, "XMemDecompress"));
        m_destroy_decompression_context = reinterpret_cast<XMemDestroyDecompressionContext_fn>(GetProcAddress(m_library, "XMemDestroyDecompressionContext"));

        m_create_compression_context = reinterpret_cast<XMemCreateCompressionContext_fn>(GetProcAddress(m_library, "XMemCreateCompressionContext"));
        m_reset_compression_context = reinterpret_cast<XMemResetCompressionContext_fn>(GetProcAddress(m_library, "XMemResetCompressionContext"));
        m_compress = reinterpret_cast<XMemCompress_fn>(GetProcAddress(m_library, "XMemCompress"));
        m_destroy_compression_context = reinterpret_cast<XMemDestroyCompressionContext_fn>(GetProcAddress(m_library, "XMemDestroyCompressionContext"));

        return m_create_decompression_context && m_reset_decompression_context && m_decompress && m_destroy_decompression_context
                && m_create_compression_context && m_reset_compression_context && m_compress && m_destroy_compression_context;
    }
}
