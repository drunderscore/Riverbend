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
#include <LibFruit/Graphics/MessageBox.h>
#include "SDL.h"
#include <LibXNA/Compress/XCompress.h>
#include <Windows.h>
#include <ShObjIdl.h>
#include <filesystem>
#undef MessageBox

int main()
{
#ifndef _DEBUG
    try
    {
#endif // _DEBUG
        CHECK_MSG(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) == 0, "SDL failed to initialize: %s", SDL_GetError());
        CHECK_MSG(SUCCEEDED(CoInitializeEx(nullptr, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE)), "COM failed to initialize");

        if(!LibXNA::XCompress::load())
        {
            MessageBox::open("XCompress failed to initialize", "Ensure you have xcompress64.dll (or xcompress32.dll) next to Riverbend.exe",
                             MessageBox::Type::Error, MessageBox::Button::OK);
                    return 1;
        }

        Application app;

        auto ret = app.start();
        CoUninitialize();

        return ret;
#ifndef _DEBUG
    }
    catch(const std::exception& e)
    {
        CHECK_MSG(false, "Unhandled exception: %s", e.what());
    }
#endif // _DEBUG
}
