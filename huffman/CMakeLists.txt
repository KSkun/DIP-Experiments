cmake_minimum_required(VERSION 3.22)
project(huffman)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_PREFIX_PATH $ENV{QT_HOME}/lib/cmake)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

add_executable(huffman
        resource/resources.qrc

        src/main.cpp
        src/mainwindow.cpp
        src/mainwindow.h
        src/mainwindow.ui
        )

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets)

add_custom_command(TARGET huffman POST_BUILD COMMAND
        $ENV{QT_HOME}/bin/windeployqt.exe ${CMAKE_CURRENT_BINARY_DIR}/huffman.exe
        )
