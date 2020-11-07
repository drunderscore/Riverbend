// Copyright James Puleo 2020
// Copyright Riverbend 2020

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

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING 1337 // TODO: Not have this
#include "FilePicker.h"
#include "loguru/loguru.hpp"
#include <Windows.h>
#include <ShObjIdl.h>
#include <thread>
#include <codecvt>
#include <locale>

IFileOpenDialog* FilePicker::m_active = nullptr;

void FilePicker::reset()
{
    m_active->Release();
    m_active = nullptr;
}

std::optional<std::wstring> FilePicker::open()
{
    if(is_open())
        return {};

    CHECK_F(SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&m_active))), "Failed to create file open dialog");

    m_active->Show(nullptr);
    IShellItem* item;
    m_active->GetResult(&item);

    if(!item)
    {
        reset();
        return {};
    }

    PWSTR path;
    item->GetDisplayName(SIGDN_FILESYSPATH, &path);
    std::wstring path_str = path;
    CoTaskMemFree(path);

    reset();
    return path_str;
}

bool FilePicker::is_open()
{
    return m_active != nullptr;
}
