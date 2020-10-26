#include "FilePicker.h"
#include <filesystem>
#include <Windows.h>
#include "Types.h"
#include <sstream>
#include "loguru/loguru.hpp"

std::vector<std::string> list_root_paths();
char FilePicker::m_path_input[512] = "";
std::vector<std::string> FilePicker::m_roots = list_root_paths();

std::function<void(std::filesystem::path)> FilePicker::m_callback;
std::vector<std::filesystem::directory_entry> FilePicker::m_listed_paths;
ImGuiTextFilter FilePicker::m_filter;
bool FilePicker::m_open = false;
bool FilePicker::m_should_open = false;

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

void FilePicker::draw(Application&)
{
    constexpr const char* FILE_PICKER_NAME = "Select a file";

    if(m_should_open)
    {
        m_should_open = false;
        ImGui::OpenPopup(FILE_PICKER_NAME);
        m_open = true;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(450, 250));
    if(ImGui::BeginPopupModal(FILE_PICKER_NAME, &m_open))
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
