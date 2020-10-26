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
