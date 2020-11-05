#include "FullyQualifiedType.h"
#include "Parser.h"
#include <sstream>

namespace LibXNA
{
    Result<FullyQualifiedType> FullyQualifiedType::parse(std::string type)
    {
        Parser p(type.c_str());
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
