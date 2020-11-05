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

#include "FilePicker.h"
#include <filesystem>
#include <Windows.h>
#include "Types.h"
#include <sstream>
#include "loguru/loguru.hpp"

std::vector<std::string> list_root_paths()
{
#ifdef _WIN32
    char drive_strings[256];
    std::vector<std::string> ret;
    std::string current_drive;
    bool saw_null = false;
    GetLogicalDriveStrings(sizeof(drive_strings), drive_strings);
    for(auto i = 0u; i < sizeof(drive_strings); i++)
    {
        auto c = drive_strings[i];
        if(c == '\0')
        {
            if(saw_null)
                break;
            saw_null = true;
            ret.push_back(current_drive);
            current_drive.clear();
        }
        else
        {
            saw_null = false;
            current_drive += c;
        }
    }

    return ret;
#else
#error Must define platform-specific root path listing!
#endif // _WIN32
}

FilePicker::FilePicker(std::string name) : m_name(name)
{
    m_roots = list_root_paths();
}

void FilePicker::open(std::function<void(std::filesystem::path)> cb)
{
    CHECK_F(!m_open, "File picker already opened");
    m_callback = cb;
    m_filter.Clear();
    m_should_open = true;
}

void FilePicker::close()
{
    m_open = false;
}

void FilePicker::update_listing(std::filesystem::path path, bool update_input)
{
    u32 count = 0;

    try
    {
        for(auto& de : std::filesystem::directory_iterator(path))
        {
            if(count == 0)
                m_listed_paths.clear();
            m_listed_paths.push_back(de);
            count++;
        }
    }
    // TODO: display to user nicely?
    catch(std::filesystem::filesystem_error ignored) {}

    if(update_input)
        strcpy_s(m_path_input, path.string().c_str());
}

void FilePicker::draw()
{
    if(m_should_open)
    {
        m_should_open = false;
        ImGui::OpenPopup(m_name.c_str());
        m_open = true;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(450, 250));
    if(ImGui::BeginPopupModal(m_name.c_str(), &m_open))
    {
        ImGui::PopStyleVar();
        if(ImGui::InputText("Path", m_path_input, sizeof(m_path_input), ImGuiInputTextFlags_EnterReturnsTrue))
        {
            auto path = std::filesystem::path(std::string(m_path_input));
            if(std::filesystem::is_directory(path))
                update_listing(path, false);
        }

        ImGui::SameLine();
        m_filter.Draw();

        auto& roots = m_roots;
        if(ImGui::BeginChild("Roots", ImVec2(150, 0), true))
        {
            if(roots.size() == 0)
                ImGui::Text("No root paths found");
            else
            {
                for(auto& r : roots)
                {
                    if(ImGui::Selectable(r.c_str()))
                        update_listing(r);
                }
            }
        }

        ImGui::EndChild();
        ImGui::SameLine();

        if(ImGui::BeginChild("Listing", ImVec2(0, 0), true))
        {
            for(auto& p : m_listed_paths)
            {
                auto path = p.path();
                auto name = path.filename().string();
                if(m_filter.PassFilter(name.c_str()) && ImGui::Selectable(name.c_str()))
                {
                    if(p.is_directory())
                    {
                        update_listing(path);
                        m_filter.Clear();
                    }
                    else
                        m_callback(p);
                }
            }
        }

        ImGui::EndChild();
        ImGui::EndPopup();
    }
    else
    {
        ImGui::PopStyleVar();
    }
}
