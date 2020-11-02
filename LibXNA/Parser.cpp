#pragma once
#include "Parser.h"

namespace LibXNA
{
    Parser::Parser(const char* chars) : m_characters(chars) {}

    char Parser::consume()
    {
        return m_characters[m_index++];
    }

    char Parser::peek()
    {
        return m_characters[m_index];
    }

    char Parser::backpeek()
    {
        return m_characters[m_index - 1];
    }

    char Parser::consume_while(char c, std::function<void(char)> fn)
    {
        char consumed;
        while((consumed = consume()) == c)
        {
            if(fn)
                fn(consumed);
        }

        return consumed;
    }

    char Parser::consume_until(char c, std::function<void(char)> fn)
    {
        char consumed;
        while((consumed = consume()) != c)
        {
            if(fn)
                fn(consumed);
        }

        return consumed;
    }

    void Parser::reset()
    {
        m_index = 0;
    }
}
