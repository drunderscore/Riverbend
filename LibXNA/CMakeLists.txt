cmake_minimum_required(VERSION 3.5)

project(LibXNA)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(LibXNA STATIC
    XNB.h
    XNB.cpp
    Types.h
    Stream.h
    Result.h

    Compress/XCompress.h
    Compress/XCompress.cpp
    Compress/Context.h

    TypeReaders/Reader.h
    TypeReaders/Reader.cpp
    TypeReaders/Texture2DReader.h
    TypeReaders/Texture2DReader.cpp
    )

target_include_directories(LibXNA PUBLIC .)
