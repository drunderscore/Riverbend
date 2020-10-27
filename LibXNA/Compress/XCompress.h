#pragma once
#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#else
#error Must define platform-specific xcompress32 API!
#endif // _WIN32 ||- _WIN64
#include "Types.h"

namespace LibXNA
{
    enum class XMEMCODEC_TYPE : int
    {
        XMEMCODEC_DEFAULT = 0,
        XMEMCODEC_LZX = 1
    };

    class XCompress
    {
    public:

        static bool load();
        static bool has_loaded_library() { return m_has_loaded_library; }
        typedef void* Context;
        typedef int (__stdcall *XMemCreateDecompressionContext_fn)(XMEMCODEC_TYPE codec, void* params, int flags, Context);
        typedef int (__stdcall *XMemResetDecompressionContext_fn)(Context);
        typedef int (__stdcall *XMemDecompress_fn)(Context, u8* dest, int* dest_size, u8* src, int src_size);
        typedef void (__stdcall *XMemDestroyDecompressionContext_fn)(Context);

        typedef int (__stdcall *XMemCreateCompressionContext_fn)(XMEMCODEC_TYPE codec, void* params, int flags, Context);
        typedef int (__stdcall *XMemResetCompressionContext_fn)(Context);
        typedef int (__stdcall *XMemCompress_fn)(Context, u8* dest, int* dest_size, u8* src, int src_size);
        typedef void (__stdcall *XMemDestroyCompressionContext_fn)(Context);

        static int create_decompression_context(XMEMCODEC_TYPE codec, void* params, int flags, Context ctx)
        {
            return m_create_decompression_context(codec, params, flags, ctx);
        }

        static int reset_decompression_context(Context ctx)
        {
            return m_reset_decompression_context(ctx);
        }

        static int decompress(Context ctx, u8* dest, int* dest_size, u8* src, int src_size)
        {
            return m_decompress(ctx, dest, dest_size, src, src_size);
        }

        static void destroy_decompression_context(Context ctx)
        {
            m_destroy_decompression_context(ctx);
        }

        static int create_compression_context(XMEMCODEC_TYPE codec, void* params, int flags, Context ctx)
        {
            return m_create_compression_context(codec, params, flags, ctx);
        }

        static int reset_compression_context(Context ctx)
        {
            return m_reset_compression_context(ctx);
        }

        static int compress(Context ctx, u8* dest, int* dest_size, u8* src, int src_size)
        {
            return m_compress(ctx, dest, dest_size, src, src_size);
        }

        static void destroy_compression_context(Context ctx)
        {
            m_destroy_compression_context(ctx);
        }

    private:
        static HMODULE m_library;
        static bool m_has_loaded_library;
        static XMemCreateDecompressionContext_fn m_create_decompression_context;
        static XMemResetDecompressionContext_fn m_reset_decompression_context;
        static XMemDecompress_fn m_decompress;
        static XMemDestroyDecompressionContext_fn m_destroy_decompression_context;

        static XMemCreateCompressionContext_fn m_create_compression_context;
        static XMemResetCompressionContext_fn m_reset_compression_context;
        static XMemCompress_fn m_compress;
        static XMemDestroyCompressionContext_fn m_destroy_compression_context;
    };
}
