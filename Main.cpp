#include <iostream>
#include <sstream>
#include "Application.h"
#include "loguru/loguru.hpp"
#include "NativeMessageBox.h"
#include "SDL.h"
#include "LibXNA/Compress/XCompress32.h"

int main()
{
    loguru::g_preamble_uptime = false;
    loguru::g_preamble_thread = false;
    loguru::add_file("riverbend.log", loguru::FileMode::Append, loguru::Verbosity_INFO);

    loguru::set_fatal_handler([](const loguru::Message& msg)
    {
        NativeMessageBox::open("Fatal Error", msg.message, NativeMessageBox::Type::Error, NativeMessageBox::Button::OK);
    });

    CHECK_F(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) == 0, "SDL failed to initialize: %s", SDL_GetError());

    Application app;

    return app.start();
}
