cmake_minimum_required(VERSION 3.5)

project(Riverbend)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(LibXNA)

set(IMGUI_FILES
    imgui/imconfig.h
    imgui/imgui.cpp
    imgui/imgui.h
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_impl_opengl3.cpp
    imgui/imgui_impl_opengl3.h
    imgui/imgui_impl_sdl.cpp
    imgui/imgui_impl_sdl.h
    imgui/imgui_internal.h
    imgui/imgui_widgets.cpp
    imgui/imstb_rectpack.h
    imgui/imstb_textedit.h
    imgui/imstb_truetype.h
    )

add_executable(Riverbend
    Main.cpp
    Application.cpp
    Application.h
    NativeMessageBox.h
    NativeMessageBox.cpp
    loguru/loguru.cpp
    GL/gl3w.c
    GL/gl3w.h
    GL/glcorearb.h
    Texture.cpp
    Texture.h
    FilePicker.h
    FilePicker.cpp
    Viewport.h
    Viewport.cpp
    TextureViewport.h
    TextureViewport.cpp
    ${IMGUI_FILES}
    )

target_include_directories(Riverbend PUBLIC .)
target_include_directories(Riverbend PUBLIC "SDL/win/include")
target_include_directories(Riverbend PUBLIC "LibXNA")
target_link_directories(Riverbend PUBLIC "SDL/win/lib/x64")
target_link_directories(Riverbend PUBLIC "build/LibXNA")
target_link_libraries(Riverbend PUBLIC "SDL2" "OpenGL32" "LibXNA")
target_compile_definitions(Riverbend PUBLIC SDL_MAIN_HANDLED WIN32_LEAN_AND_MEAN IMGUI_IMPL_OPENGL_LOADER_CUSTOM="GL/gl3w.h")
