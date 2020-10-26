#pragma once
#include <string>

class NativeMessageBox final
{
public:
    enum class Type
    {
        Info,
        Error
    };

    enum class Button
    {
        OK,
        OKCancel,
        YesNo,
    };

    static void open(std::string title, std::string text, Type, Button);
    
private:
    NativeMessageBox() {}
};
