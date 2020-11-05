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
#include <istream>
#include <string>
#include "Types.h"

namespace LibXNA
{
    class Stream
    {
    public:
        Stream(u8* data, u32 size, bool copy = true) : m_size(size), m_our_data(copy)
        {
            if(copy)
            {
                m_data = new u8[size];
                std::copy(data, data + size, m_data);
            }
            else
            {
                m_data = data;
            }
        }

        Stream(std::istream& src)
        {
            m_our_data = true;
            src.seekg(0, std::ios::end);
            m_size = src.tellg();
            src.seekg(0, std::ios::beg);
            m_data = new u8[m_size];
            src.read(reinterpret_cast<char*>(m_data), m_size);
        }

        ~Stream()
        {
            if(m_our_data)
                delete[] m_data;
        }

        template<typename T>
        T read()
        {
            // FIXME: Is this the best you got?
            auto val = *reinterpret_cast<T*>(&m_data[m_index]);
            m_index += sizeof(T);
            return val;
        }

        template<typename T>
        void read(T& val)
        {
            std::copy(m_data + m_index, m_data + m_index + sizeof(T), reinterpret_cast<u8*>(&val));
            m_index += sizeof(T);
        }

        template<typename T>
        void read(T* val)
        {
            std::copy(m_data + m_index, m_data + m_index + sizeof(T), reinterpret_cast<u8*>(val));
            m_index += sizeof(T);
        }

        template<typename T>
        void read(T* val, u32 size)
        {
            std::copy(m_data + m_index, m_data + m_index + size, reinterpret_cast<u8*>(val));
            m_index += size;
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

    public:
        u32 m_index = 0;
        u8* m_data;
        std::streamsize m_size;
        bool m_our_data;
    };
}
