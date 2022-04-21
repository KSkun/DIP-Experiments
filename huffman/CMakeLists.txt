cmake_minimum_required(VERSION 3.22)
project(huffman)

set(CMAKE_CXX_STANDARD 20)

set(Qt5_DIR $ENV{QT_HOME}/lib/cmake/Qt5)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

add_executable(huffman
        src/main.cpp
        )

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt5::Core Qt5::Widgets)
