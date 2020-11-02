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
