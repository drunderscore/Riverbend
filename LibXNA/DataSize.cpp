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
