#include "PropertiesPanel.h"
#include "imgui/imgui.h"
#include <sstream>

void PropertiesPanel::draw(Application &app)
{
    if(auto xnb = app.loaded_xnb())
    {
        if(ImGui::Begin("Properties", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            if(ImGui::BeginTable("PropertiesTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingPolicyFixedX))
            {
                auto insert_key_value = [](const char* key, const char* specifier, auto val)
                {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", key);
                    ImGui::TableNextColumn();
                    ImGui::Text(specifier, val);
                };

                insert_key_value("Target Platform", "%s", LibXNA::get_target_platform_name(xnb->header().m_target_platform));
                insert_key_value("Format Version", "%d", xnb->header().m_format_version);

                std::stringstream flags_text;
                if(xnb->is_hi_def())
                    flags_text << "Hi-Def";
                if(xnb->is_compressed())
                {
                    if(xnb->is_hi_def())
                        flags_text << ", ";
                    flags_text << "Compressed";
                }
                insert_key_value("Flags", "%s", flags_text.str().c_str());

                insert_key_value("Total Size", "%d", xnb->header().m_total_size);
                if(auto decompressed_size = xnb->decompressed_size())
                    insert_key_value("Decompressed Size", "%d", *decompressed_size);

                ImGui::EndTable();
            }

            if(!xnb->is_compressed())
            {
                ImGui::Separator();

            }
        }
        ImGui::End();
    }
}
