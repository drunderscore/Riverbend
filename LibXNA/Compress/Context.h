#pragma once
#include "XCompress.h"
#include "Types.h"

namespace LibXNA
{
    class Context
    {
    public:
        virtual ~Context() {}
        virtual int reset() = 0;
        virtual int perform(u8* dest, int* dest_size, u8* src, int src_size) = 0;

        bool is_valid() { return m_ctx != nullptr; }
        int return_value() { return m_creation_return; }

    protected:
        XCompress::Context m_ctx;
        int m_creation_return;
    };

    class DecompressionContext : public Context
    {
    public:
        DecompressionContext(XMEMCODEC_TYPE codec, void* params = nullptr, int flags = 0)
        {
            m_creation_return = XCompress::create_decompression_context(codec, params, flags, &m_ctx);
        }

        int reset() override
        {
            return XCompress::reset_decompression_context(m_ctx);
        }

        int perform(u8* dest, int* dest_size, u8* src, int src_size) override
        {
            return XCompress::decompress(m_ctx, dest, dest_size, src, src_size);
        }

        ~DecompressionContext() override
        {
            if(is_valid())
                XCompress::destroy_decompression_context(m_ctx);
        }
    };

    class CompressionContext : public Context
    {
    public:
        CompressionContext(XMEMCODEC_TYPE codec, void* params = nullptr, int flags = 0)
        {
            m_creation_return = XCompress::create_compression_context(codec, params, flags, &m_ctx);
        }

        int reset() override
        {
            return XCompress::reset_compression_context(m_ctx);
        }

        int perform(u8* dest, int* dest_size, u8* src, int src_size) override
        {
            return XCompress::compress(m_ctx, dest, dest_size, src, src_size);
        }

        ~CompressionContext() override
        {
            if(is_valid())
                XCompress::destroy_compression_context(m_ctx);
        }
    };
}
