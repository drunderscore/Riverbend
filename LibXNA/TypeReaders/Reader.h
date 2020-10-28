#pragma once
#include "Stream.h"
#include "Types.h"

namespace LibXNA
{
    class Reader
    {
    public:
        virtual ~Reader() {}
    protected:
        virtual const char* type_name() const = 0;
    };
}
