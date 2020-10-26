#include "Reader.h"

namespace LibXNA
{
    Reader::Reader(Stream& stream)
    {
        m_typeid = stream.read_7bit_encoded_int();
    }
}
