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

#include "FullyQualifiedType.h"
#include <LibFruit/Data/Parser.h>
#include <sstream>

namespace LibXNA
{
    Result<FullyQualifiedType> FullyQualifiedType::parse(std::string type)
    {
        LibFruit::Parser p(type.c_str());
        std::stringstream buffer;
        FullyQualifiedType fully_qualified;

        while(!p.finished())
        {
            auto c = p.consume();
            if(c == '\\')
            {
                auto escape = p.consume();

                if(escape == ',' || escape == '+' || escape == '&' || escape == '*'
                        || escape == '[' || escape == ']' || escape == '.' || escape == '\\')
                    buffer << escape;
                else
                    return "Invalid escape character";
            }
            else if(c == '.')
            {
                fully_qualified.m_namespace.push_back(buffer.str());
                buffer.str(std::string());
                buffer.clear();
            }
            else if(c == ',')
            {
                fully_qualified.m_class_name = buffer.str();
                // TODO: Finish parsing key-values
                return fully_qualified;
            }
            else
            {
                buffer << c;
            }
        }

        // if there are no key-values, then it won't end in a comma, so just append and return early
        fully_qualified.m_class_name = buffer.str();
        return fully_qualified;
    }

    std::string FullyQualifiedType::type_name()
    {
        std::stringstream buffer;
        for(auto& str : m_namespace)
            buffer << str << '.';
        buffer << m_class_name;

        return buffer.str();
    }
}
