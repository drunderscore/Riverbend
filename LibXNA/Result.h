#pragma once
#include <optional>
#include <string>

namespace LibXNA
{
    template<typename T>
    class Result : public std::optional<T>
    {
    public:
        Result(T val) : std::optional<T>(val) {}
        Result(std::string error) : m_error(error) {}
        Result(const char* error) : m_error(error) {}

        std::string& error(){ return m_error; }

    private:
        std::string m_error;
    };
}
