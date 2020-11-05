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
