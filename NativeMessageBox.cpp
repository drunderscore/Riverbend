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

#include "NativeMessageBox.h"
#include <Windows.h>
#include "Types.h"

void NativeMessageBox::open(std::string title, std::string text, Type type, Button buttons)
{
#ifdef _WIN32
    u32 win_type = 0;
    switch(type)
    {
        case Type::Info:
            win_type |= MB_ICONINFORMATION;
            break;
        case Type::Error:
            win_type |= MB_ICONERROR;
            break;
    }

    switch(buttons)
    {
        case Button::OK:
            win_type |= MB_OK;
            break;
        case Button::OKCancel:
            win_type |= MB_OKCANCEL;
            break;
        case Button::YesNo:
            win_type |= MB_YESNO;
            break;
    }

    MessageBox(NULL, text.c_str(), title.c_str(), win_type);
#else
#error Must define platform-specific message box API!
#endif // _WIN32
}
