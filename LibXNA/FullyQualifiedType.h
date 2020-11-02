#pragma once
#include "Types.h"
#include "Result.h"
#include <string>
#include <vector>

namespace LibXNA
{
    class FullyQualifiedType
    {
    public:
        static Result<FullyQualifiedType> parse(std::string type);

        // TODO: rename this to something better.
        const std::vector<std::string>& namespaze() { return m_namespace; }
        std::string& class_name() { return m_class_name; }
    private:
        FullyQualifiedType() {}

        std::vector<std::string> m_namespace;
        std::string m_class_name;
    };
}
