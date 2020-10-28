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
