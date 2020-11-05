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
#include <cstring>
#include <functional>

namespace LibXNA
{
    class Parser
    {
    public:
        Parser(const char*);
        char consume();
        char backpeek();
        char peek();
        char consume_until(char, std::function<void(char)> = nullptr);
        char consume_while(char, std::function<void(char)> = nullptr);
        void reset();
        u32 index() { return m_index; }
        bool finished() { return m_index >= strlen(m_characters); }

    private:
        u32 m_index = 0;
        const char* m_characters;
    };
}
