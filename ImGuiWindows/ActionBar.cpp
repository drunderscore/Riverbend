#include "ActionBar.h"
#include "imgui/imgui.h"
#include <filesystem>
#include "loguru/loguru.hpp"
#include "ImGuiWindows/FilePicker.h"

void ActionBar::draw(Application& app)
{
    if(ImGui::BeginMainMenuBar())
    {
        if(ImGui::BeginMenu("File"))
        {
            if(ImGui::MenuItem("Open"))
            {
                FilePicker::open([&](auto path)
                {
                    app.load_file(path);
                    FilePicker::close();
                });
            }

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}
