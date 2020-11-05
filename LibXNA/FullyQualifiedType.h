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
        std::string type_name();
    private:
        FullyQualifiedType() {}

        std::vector<std::string> m_namespace;
        std::string m_class_name;
    };
}
