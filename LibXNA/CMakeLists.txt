cmake_minimum_required(VERSION 3.5)

project(LibXNA)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(LibXNA STATIC
    XNB.h
    XNB.cpp

    Compress/XCompress.h
    Compress/XCompress.cpp
    Compress/Context.h
    Compress/Context.cpp

    TypeReaders/Reader.h
    TypeReaders/Texture2DReader.h
    TypeReaders/SurfaceFormat.h
    TypeReaders/SurfaceFormat.cpp
    TypeReaders/ModelReader.h
    TypeReaders/ModelReader.cpp

    FullyQualifiedType.h
    FullyQualifiedType.cpp
    )

# This is for LibFruit inclusion
target_include_directories(LibXNA SYSTEM PUBLIC ../)

target_include_directories(LibXNA PUBLIC .)
target_link_directories(LibXNA PUBLIC ../build/LibFruit)
target_link_libraries(LibXNA PUBLIC LibFruit)
