#include "Application.h"
#include "SDL.h"
#include "GL/gl3w.h"
#include "loguru/loguru.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include <fstream>
#include "LibXNA/TypeReaders/Texture2DReader.h"
#include "LibXNA/Stream.h"
#include <filesystem>
#include "Result.h"
#include <sstream>

u32 Application::start()
{
#if __APPLE__
    // GL 3.2 Core + GLSL 150
    const char* glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    CHECK_F((m_window = SDL_CreateWindow("Riverbend", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720,
                                 SDL_WindowFlags::SDL_WINDOW_OPENGL | SDL_WindowFlags::SDL_WINDOW_RESIZABLE |
                                 SDL_WindowFlags::SDL_WINDOW_ALLOW_HIGHDPI )) != nullptr, "Unable to create SDL window");

    CHECK_F((m_gl_context = SDL_GL_CreateContext(m_window)) != nullptr, "Unable to create GL context");
    SDL_GL_MakeCurrent(m_window, m_gl_context);
    SDL_GL_SetSwapInterval(0);

    CHECK_F(gl3wInit() == 0, "Unable to initialize GL3W");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(m_window, m_gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

#ifdef _DEBUG
    load_file("test.xnb");
#endif

    while(!m_closing)
    {
        poll();
        draw();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(m_gl_context);
    SDL_DestroyWindow(m_window);

    return 0;
}

void Application::poll()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        ImGui_ImplSDL2_ProcessEvent(&e);
        if (e.type == SDL_QUIT)
            m_closing = true;
    }
}

void Application::load_file(std::filesystem::path path)
{
    std::ifstream file_stream(path);
    if(!file_stream.good())
    {
        show_dialog("Could not read file", "The file could not be read.");
        return;
    }

    auto xnb = LibXNA::XNB::parse(file_stream);
    if(!xnb)
    {
        std::stringstream error_text;
        error_text << "The XNB failed to parse:\n" << xnb.error().c_str();
        show_dialog("Could not parse XNB", error_text.str());
        return;
    }

    if(xnb->is_compressed())
        show_dialog("XNB is compressed", "This XNB file is compressed. You can view basic information, but there is currently no support for viewing this data.");

    m_loaded_xnb = std::make_shared<LibXNA::XNB>(*xnb);
}

void Application::draw()
{
    auto io = ImGui::GetIO();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(m_window);
    ImGui::NewFrame();

#ifdef _DEBUG
    ImGui::ShowDemoWindow();
#endif

    draw_viewport();
    draw_action_bar();
    draw_properties();
    m_file_picker.draw();

    for(auto it = m_dialogs.begin(); it != m_dialogs.end();)
    {
        if(!it->is_open())
        {
            it->set_open(true);
            ImGui::OpenPopup(it->name().c_str());
        }

        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.7f, 0.1f, 0.1f, 1.0f));
        ImGui::SetNextWindowSizeConstraints(ImVec2(250, 250), ImVec2(250, -1));
        if(ImGui::BeginPopupModal(it->name().c_str(), nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize))
        {
            ImGui::TextWrapped("%s", it->text().c_str());
            ImGui::Separator();
            if(ImGui::Button("Okay!", ImVec2(ImGui::GetContentRegionAvailWidth(), 0)))
            {
                it = m_dialogs.erase(it);
                ImGui::CloseCurrentPopup();
            }
            else
                ++it;
            ImGui::EndPopup();
        }
        ImGui::PopStyleColor();
    }

    ImGui::Render();

    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(m_window);
}

void Application::draw_viewport()
{

}

void Application::draw_action_bar()
{
    if(ImGui::BeginMainMenuBar())
    {
        if(ImGui::BeginMenu("File"))
        {
            if(ImGui::MenuItem("Open"))
            {
                m_file_picker.open([&](auto path)
                {
                    load_file(path);
                    m_file_picker.close();
                });
            }

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void Application::draw_properties()
{
    if(auto xnb = loaded_xnb())
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

            ImGui::Separator();

            if(auto reader = xnb->primary_object())
            {
                ImGui::Text("[hover for reader]");
                if(ImGui::IsItemHovered())
                    ImGui::SetTooltip("%s", reader->reader().c_str());
            }
            else
            {
                ImGui::Text("No primary object");
            }
        }
        ImGui::End();
    }
}
