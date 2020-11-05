#pragma once
#include <optional>
#include <istream>
#include "Types.h"
#include <memory>
#include <vector>
#include "Stream.h"
#include "Result.h"
#include "FullyQualifiedType.h"
#include "DataSize.h"

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
            static Result<TypeReader> parse(Stream&);
            FullyQualifiedType& reader() { return m_reader; }
            int version(){ return m_version; }
        private:
            TypeReader(FullyQualifiedType reader) : m_reader(reader) {}
            FullyQualifiedType m_reader;
            int m_version;
        };

        static Result<XNB> parse(Stream&);
        const std::vector<TypeReader> type_readers() const { return m_type_readers; }
        const Header& header() const { return m_header; }
        std::optional<DataSize> decompressed_size() { return m_decompressed_size; }
        const std::optional<TypeReader> primary_object() const;

        DataSize total_size()
        {
            return m_header.m_total_size;
        }

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
        std::optional<DataSize> m_decompressed_size;
        std::vector<TypeReader> m_type_readers;
        u32 m_shared_resource_count = 0;
        u32 m_primary_object_typeid = 0;
    };
}
