#pragma once
#include "imgui/imgui.h"
#include <functional>
#include <filesystem>

namespace ImGuiExtras
{
    void BeginFilePickerModal(const char* name, std::function<void(const char* selected_path)> callback);
}
