#pragma once
#include "Types.h"
#include "Texture.h"
#include <memory>
#include <string>
#include <vector>
#include <filesystem>
#include "LibXNA/XNB.h"

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

    u32 start();
    void show_dialog(std::string name, std::string text) { m_dialogs.push_back(Dialog(name, text)); }
    void load_file(std::filesystem::path);
    std::optional<LibXNA::XNB> loaded_xnb() { return m_loaded_xnb; }

private:
    void poll();
    void render();

    SDL_Window *m_window = nullptr;
    SDL_GLContext m_gl_context = nullptr;
    bool m_closing = false;
    std::vector<Dialog> m_dialogs;
    std::optional<LibXNA::XNB> m_loaded_xnb;
};
