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

#include "XNB.h"
#include <sstream>
#include "Compress/Context.h"

namespace LibXNA
{
    Result<XNB> XNB::parse(LibFruit::Stream& stream)
    {
        constexpr static const char XNB_MAGIC[] = "XNB";
        XNB xnb;

        stream.read(xnb.m_header);

        if(memcmp(XNB_MAGIC, xnb.m_header.m_magic, 3) != 0)
            return "Invalid XNB magic";

        // TODO: This method has a lot of hoops to jump through...
        // Try to condense it more, remove this stupid read_data
        std::vector<u8> read_data;

        if(xnb.is_compressed())
        {
            auto decompressed_size = stream.read<u32>();;
            xnb.m_decompressed_size = decompressed_size;
            DecompressionContext ctx;
            if(!ctx.is_valid())
                return "Unable to create decompression context";

            read_data.resize(decompressed_size);
            raw_ptr dest_size = decompressed_size;
            // The compressed header is 14 bytes long
            if(ctx.perform(read_data.data(), &dest_size, stream.current(), xnb.m_header.m_total_size - 14))
                return "Unable to decompress data";
        }
        else
        {
            // The decompressed header is 10 bytes long
            read_data.resize(xnb.m_header.m_total_size - 10);
            auto start = stream.current();
            std::copy(start, start + read_data.size(), read_data.begin());
        }

        LibFruit::Stream data_stream(read_data);

        auto type_reader_count = data_stream.read_7bit_encoded_int();
        for(int i = 0; i < type_reader_count; i++)
        {
            if(auto reader = TypeReader::parse(data_stream))
                xnb.m_type_readers.push_back(*reader);
        }

        xnb.m_shared_resource_count = data_stream.read_7bit_encoded_int();
        xnb.m_primary_object_typeid = data_stream.read_7bit_encoded_int();
        xnb.m_data.assign(read_data.begin() + (data_stream.current() - data_stream.get()), read_data.end());

        return xnb;
    }

    const std::optional<XNB::TypeReader> XNB::primary_object() const
    {
        if(m_primary_object_typeid == 0)
            return {};

        return m_type_readers[m_primary_object_typeid - 1];
    }

    Result<XNB::TypeReader> XNB::TypeReader::parse(LibFruit::Stream& stream)
    {
        auto type_name = FullyQualifiedType::parse(stream.read_csharp_string());
        auto version = stream.read<int>();
        // TODO: result stack (put this result inside another result)
        if(!type_name)
            return type_name.error();

        TypeReader ret(*type_name);
        ret.m_version = version;
        return ret;
    }
}
