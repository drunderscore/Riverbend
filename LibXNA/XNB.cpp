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

namespace LibXNA
{
    Result<XNB> XNB::parse(LibFruit::Stream& stream)
    {
        constexpr static char XNB_MAGIC[] = "XNB";
        XNB xnb;

        stream.read(xnb.m_header);

        if(memcmp(XNB_MAGIC, xnb.m_header.m_magic, 3) != 0)
            return "Invalid XNB magic";

        // TODO: Support compressed XNB format
        if(xnb.is_compressed())
            xnb.m_decompressed_size = stream.read<u32>();
        else
        {
            auto type_reader_count = stream.read_7bit_encoded_int();
            for(int i = 0; i < type_reader_count; i++)
            {
                if(auto reader = TypeReader::parse(stream))
                    xnb.m_type_readers.push_back(*reader);
            }

            xnb.m_shared_resource_count = stream.read_7bit_encoded_int();
            xnb.m_primary_object_typeid = stream.read_7bit_encoded_int();
        }

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
        auto type_name = FullyQualifiedType::parse(stream.read_string());
        auto version = stream.read<int>();
        // TODO: result stack (put this result inside another result)
        if(!type_name)
            return type_name.error();

        TypeReader ret(*type_name);
        ret.m_version = version;
        return ret;
    }
}
