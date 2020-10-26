#include "ImGuiExtras.h"
#include <Windows.h>
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

namespace ImGuiExtras
{
    void BeginFilePickerModal(const char* name, std::function<void(const char* selected_path)> callback)
    {
        if(ImGui::BeginPopupModal(name))
        {
            auto roots = list_root_paths();
            if(ImGui::BeginChild("Roots", ImVec2(150, 0), true))
            {
                if(roots.size() == 0)
                    ImGui::Text("No root paths found");
                else
                {
                    for(auto& r : roots)
                        ImGui::Selectable(r.c_str());
                }

                ImGui::EndChild();
            }
            ImGui::SameLine();

            if(ImGui::BeginChild("Listing", ImVec2(0, 0), true))
            {

            }

            ImGui::EndPopup();
        }
    }
}
