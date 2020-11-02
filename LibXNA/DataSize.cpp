#include "DataSize.h"
#include <cmath>
#include <sstream>

namespace LibXNA
{
    // Instead of kB / MB and so on, ISO/IEC 80000 calls these
    // types 'kibi' and 'mebi' and so on. This is to avoid confusion
    // with SI types.
    const char* DataSize::m_type_suffix[] = {"B", "KiB", "MiB", "GiB", "TiB", "PiB"};

    inline double DataSize::convert(Type t)
    {
        if(t == Type::Byte)
            return m_size;

        auto exp = static_cast<u32>(t);
        auto val = std::powl(1024, exp);
        return m_size / val;
    }

    inline std::string DataSize::display(Type t, bool include_suffix)
    {
        if(!include_suffix)
            return std::to_string(convert(t));

        std::stringstream stream;
        stream << convert(t) << ' ' << m_type_suffix[static_cast<u32>(t)];
        return stream.str();
    }

    std::string DataSize::display_most_fitting()
    {
        if(m_size < 1024)
            return std::to_string(m_size);

        auto exp = 1;

        while(m_size > std::powl(1024, exp))
            exp++;

        exp--;

        return display(static_cast<Type>(exp));
    }
}
