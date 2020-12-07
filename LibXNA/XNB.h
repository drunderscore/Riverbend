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
#include <optional>
#include <istream>
#include <LibFruit/Fruit.h>
#include <LibFruit/Data/Stream.h>
#include <LibFruit/Data/DataSize.h>
#include <memory>
#include <vector>
#include "FullyQualifiedType.h"

namespace LibXNA
{
    enum class TargetPlatform : char
    {
        Windows = 'w',
        Phone = 'm',
        Xbox = 'x'
    };

    static const char* get_target_platform_name(TargetPlatform plat)
    {
        switch(plat)
        {
            case TargetPlatform::Windows:
                return "Windows";
            case TargetPlatform::Phone:
                return "Phone";
            case TargetPlatform::Xbox:
                return "Xbox";
            default:
                return "???";
        }
    }

    class XNB
    {
    public:
        enum class Flags : u8
        {
            HiDef = 1 << 0,
            Compressed = 1 << 7
        };

#pragma pack(push, 1)
        struct Header
        {
        public:
            char m_magic[3];
            TargetPlatform m_target_platform;
            u8 m_format_version;
            Flags m_flags;
            u32 m_total_size;
            Header() = default;
        };
#pragma pack(pop)

        class TypeReader
        {
        public:
            static Result<TypeReader> parse(LibFruit::Stream&);
            FullyQualifiedType& reader() { return m_reader; }
            int version(){ return m_version; }
        private:
            TypeReader(FullyQualifiedType reader) : m_reader(reader) {}
            FullyQualifiedType m_reader;
            int m_version;
        };

        static Result<XNB> parse(LibFruit::Stream&);
        const std::vector<TypeReader> type_readers() const { return m_type_readers; }
        const Header& header() const { return m_header; }
        std::optional<LibFruit::DataSize> decompressed_size() { return m_decompressed_size; }
        const std::optional<TypeReader> primary_object() const;
        LibFruit::DataSize total_size() { return m_header.m_total_size; }
        const std::vector<u8>& data() const { return m_data; }

        bool is_compressed()
        {
            return (static_cast<int>(m_header.m_flags) & static_cast<int>(Flags::Compressed)) != 0;
        }

        bool is_hi_def()
        {
            return (static_cast<int>(m_header.m_flags) & static_cast<int>(Flags::HiDef)) != 0;
        }

    private:
        Header m_header;
        std::optional<LibFruit::DataSize> m_decompressed_size;
        std::vector<TypeReader> m_type_readers;
        u32 m_shared_resource_count = 0;
        u32 m_primary_object_typeid = 0;
        // This is always decompressed data, starting at the primary asset data.
        std::vector<u8> m_data;
    };
}
