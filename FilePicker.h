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

#pragma once
#include <functional>
#include <vector>
#include <string>
#include <filesystem>
#include "imgui/imgui.h"

class FilePicker
{
public:
    FilePicker(std::string name);
    void open(std::function<void(std::filesystem::path)>);
    void close();
    void draw();

private:
    void update_listing(std::filesystem::path path, bool update_input = true);

    std::string m_name;
    std::function<void(std::filesystem::path)> m_callback;
    std::vector<std::string> m_roots;
    std::vector<std::filesystem::directory_entry> m_listed_paths;
    char m_path_input[512] = "";
    ImGuiTextFilter m_filter;
    bool m_open = false;
    bool m_should_open = false;
};
