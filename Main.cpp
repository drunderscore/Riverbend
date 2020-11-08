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

#include <iostream>
#include <sstream>
#include "Application.h"
#include "loguru/loguru.hpp"
#include <LibFruit/Graphics/MessageBox.h>
#include "SDL.h"
#include "LibXNA/Compress/XCompress.h"
#include <Windows.h>
#include <ShObjIdl.h>
#undef MessageBox

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
#ifndef _DEBUG
    try
    {
#endif // _DEBUG
        loguru::g_preamble_uptime = false;
        loguru::g_preamble_thread = false;
        loguru::add_file("riverbend.log", loguru::FileMode::Append, loguru::Verbosity_INFO);

        loguru::set_fatal_handler([](const loguru::Message& msg)
        {
            MessageBox::open("Fatal Error", msg.message, MessageBox::Type::Error, MessageBox::Button::OK);
        });

        CHECK_F(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) == 0, "SDL failed to initialize: %s", SDL_GetError());
        CHECK_F(SUCCEEDED(CoInitializeEx(nullptr, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE)), "COM failed to initialize");

        Application app;

        auto ret = app.start();
        CoUninitialize();

        return ret;
#ifndef _DEBUG
    }
    catch(const std::exception& e)
    {
        ABORT_F("Unhandled exception: %s", e.what());
    }
#endif // _DEBUG
}
