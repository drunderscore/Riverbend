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

#include "Application.h"
#include <LibFruit/Fruit.h>
#include "SDL.h"
#include "GL/gl3w.h"
#include "loguru/loguru.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include <fstream>
#include "LibXNA/TypeReaders/Texture2DReader.h"
#include <filesystem>
#include "Result.h"
#include <sstream>
#include "Viewport.h"
#include "TextureViewport.h"
#include "Build.h"
#include <optional>

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

    CHECK_F((m_window = SDL_CreateWindow(get_build_title(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720,
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
    load_file("test_mipped.xnb");
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
        if(m_viewport)
            m_viewport->poll(e);

        if (e.type == SDL_QUIT)
            m_closing = true;
    }
}

void Application::load_file(std::filesystem::path path)
{
    std::ifstream file_stream(path, std::ios::binary);
    LibFruit::Stream stream(file_stream);

    if(!file_stream.good())
    {
        show_dialog("Could not read file", "The file could not be read.");
        return;
    }

    auto xnb = LibXNA::XNB::parse(stream);
    if(!xnb)
    {
        std::stringstream error_text;
        error_text << "The XNB failed to parse:\n" << xnb.error().c_str();
        show_dialog("Could not parse XNB", error_text.str());
        return;
    }

    m_loaded_xnb = std::make_shared<LibXNA::XNB>(*xnb);

    auto created_viewport = false;
    if(xnb->is_compressed())
        show_dialog("XNB is compressed", "This XNB file is compressed. You can view basic information, but there is currently no support for viewing this data.");
    else
    {
        if(auto primary = xnb->primary_object())
        {
            // TODO: Decide what reader to load inside of LibXNA
            // TODO: Load off render thread, and display loading
            if(primary->reader().type_name() == LibXNA::Texture2DReader::type_name())
            {
                m_loaded_reader = std::make_shared<LibXNA::Texture2DReader>(stream);
                created_viewport = true;
                m_viewport = std::make_shared<TextureViewport>(*this);
            }
            else
            {
                show_dialog("Unknown primary object type name", "This type name is unimplemented or unknown, so it cannot be displayed.");
            }
        }
        else
        {
            show_dialog("No primary object", "This XNB does not have any primary object, so it cannot be displayed.");
        }
    }

    if(!created_viewport)
        m_viewport = std::make_shared<Viewport>(*this);
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

    draw_action_bar();
    if(m_viewport)
        m_viewport->draw();
    draw_action_bar(true);

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

void Application::draw_action_bar(bool post)
{
    if(ImGui::BeginMainMenuBar())
    {
        if(post)
        {
            if(ImGui::BeginMenu("About"))
            {
                ImGui::Text("Licensed under GNU GPL Version 3");
                ImGui::Text("Written and open sourced by James Puleo");

                if(ImGui::MenuItem("Copy Information"))
                    ImGui::SetClipboardText(get_build_title());
                ImGui::EndMenu();
            }
        }
        else
        {
            if(ImGui::BeginMenu("File"))
            {
                if(ImGui::MenuItem("Open"))
                {
                    // TODO: we're on the wrong thread!
                    if(auto path = FilePicker::open())
                        load_file(*path);
                }

                ImGui::EndMenu();
            }
        }
        ImGui::EndMainMenuBar();
    }
}
