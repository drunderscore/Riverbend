#include "XNB.h"
#include <sstream>

namespace LibXNA
{
    Result<XNB> XNB::parse(std::istream& native_stream)
    {
        constexpr static char XNB_MAGIC[] = "XNB";
        XNB xnb;

        Stream stream = native_stream;
        stream.read(xnb.m_header);

        if(memcmp(XNB_MAGIC, xnb.m_header.m_magic, 3) != 0)
            return "Invalid XNB magic";

        // TODO: Support compressed XNB format
        if(xnb.is_compressed())
            stream.read(xnb.m_decompressed_size);
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

    Result<XNB::TypeReader> XNB::TypeReader::parse(Stream& stream)
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
