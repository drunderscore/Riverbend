#pragma once
#include "Stream.h"
#include "Types.h"

namespace LibXNA
{
    class Reader
    {
    public:
        Reader(Stream& stream);
        virtual ~Reader();
    protected:
        u32 m_typeid;
        virtual const char* type_name() const = 0;
    };
}
