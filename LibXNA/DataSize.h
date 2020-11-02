#pragma once
#include "Types.h"
#include <string>

namespace LibXNA
{
    class DataSize
    {
    public:
        // Although these types follow SI naming scheme,
        // data sizes should (and are) interpreted binary wise,
        // following ISO/IEC 80000. This means that each
        // type is a multiple of 1024 (instead of 1000).
        enum class Type
        {
            Byte,
            Kilobyte,
            Megabyte,
            Gigabyte,
            Terrabyte,
            Petabyte
        };

        DataSize(u64 size) : m_size(size) {}
        u64 size() { return m_size; }

        inline double convert(Type);
        inline std::string display(Type, bool include_suffix = true);
        // Display this as the most fitting size.
        // Will pick the last size that displays a whole number.
        std::string display_most_fitting();
    private:
        static const char* m_type_suffix[];
        u64 m_size;
    };
}
