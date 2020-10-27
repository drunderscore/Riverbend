#pragma once
#include <istream>
#include <string>
#include "Types.h"

namespace LibXNA
{
    class Stream
    {
    public:
        Stream(std::istream& stream) : m_base(stream) {}
        template<typename T>
        T read()
        {
            T value;
            m_base.read(reinterpret_cast<char*>(&value), sizeof(T));
            return value;
        }

        template<typename T>
        void read(T& val)
        {
            m_base.read(reinterpret_cast<char*>(&val), sizeof(T));
        }

        template<typename T>
        void read(T* val)
        {
            m_base.read(reinterpret_cast<char*>(val), sizeof(T));
        }

        std::string read_string()
        {
            auto len = read_7bit_encoded_int();
            std::string str;
            for(int i = 0; i < len; i++)
                str += read<char>();
            return str;
        }

        template<typename TInt = int>
        TInt read_7bit_encoded_int()
        {
            TInt result = 0;

            for(auto i = 0u; i < sizeof(TInt); i++)
            {
                auto val = read<u8>();
                result |= (val & 127) << (7 * i);
                if((val & 128) == 0)
                    break;
            }
            return result;
        }

    private:
        std::istream& m_base;
    };
}
