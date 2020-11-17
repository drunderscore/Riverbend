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
#include <LibFruit/Types.h>
#include <memory>
#include <string>
#include <vector>
#include <filesystem>
#include <LibXNA/XNB.h>
#include "FilePicker.h"
#include <LibXNA/TypeReaders/Reader.h>

class Viewport;

struct SDL_Window;
typedef void *SDL_GLContext;

class Application
{
public:
    class Dialog
    {
    public:
        Dialog(std::string name, std::string text) : m_name(name), m_text(text) {}
        std::string& name() { return m_name; }
        std::string& text() { return m_text; }
        bool is_open() { return m_open; }
        void set_open(bool val) { m_open = val; }

    private:
        bool m_open = false;
        std::string m_name;
        std::string m_text;
    };

    Application() {}
    u32 start();
    void show_dialog(std::string name, std::string text) { m_dialogs.push_back(Dialog(name, text)); }
    void load_file(std::filesystem::path);
    const std::shared_ptr<LibXNA::XNB> loaded_xnb() const { return m_loaded_xnb; }
    const std::shared_ptr<LibXNA::Reader> loaded_reader() const { return m_loaded_reader; }

private:
    void poll();
    void draw();
    void draw_action_bar(bool post = false);

    SDL_Window *m_window = nullptr;
    SDL_GLContext m_gl_context = nullptr;
    bool m_closing = false;
    std::vector<Dialog> m_dialogs;
    std::shared_ptr<LibXNA::XNB> m_loaded_xnb;
    std::shared_ptr<LibXNA::Reader> m_loaded_reader;
    std::shared_ptr<Viewport> m_viewport;
};
