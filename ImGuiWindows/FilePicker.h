#pragma once
#include <functional>
#include <vector>
#include <string>
#include <filesystem>
#include "imgui/imgui.h"
#include "Application.h"

class FilePicker
{
public:
    static void open(std::function<void(std::filesystem::path)>);
    static void close();
    static void draw(Application& app);

private:
    FilePicker() {}
    static void update_listing(std::filesystem::path path, bool update_input = true);

    static std::function<void(std::filesystem::path)> m_callback;
    static std::vector<std::string> m_roots;
    static std::vector<std::filesystem::directory_entry> m_listed_paths;
    static char m_path_input[512];
    static ImGuiTextFilter m_filter;
    static bool m_open;
    static bool m_should_open;
};
