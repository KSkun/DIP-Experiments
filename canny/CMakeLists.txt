cmake_minimum_required(VERSION 3.22)
project(nodecanny)

set(CMAKE_CXX_STANDARD 17)

# work around for including node-api headers
if (NOT DEFINED CMAKE_JS_VERSION)
    # only work with windows & specified node version
    # not a good idea
    set(CMAKE_JS_INC $ENV{USERPROFILE}/.cmake-js/node-x64/v16.15.0/include/node)
endif ()

add_library(${PROJECT_NAME} SHARED
        ${CMAKE_JS_SRC}
        cpp/napi.cpp
        cpp/image.h
        cpp/image.cpp
        )

include_directories(${CMAKE_JS_INC})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})

add_definitions(-DNAPI_VERSION=3)

execute_process(COMMAND node -p "require('node-addon-api').include"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE NODE_ADDON_API_DIR
        )
string(REPLACE "\n" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
string(REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${NODE_ADDON_API_DIR})

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} opencv/cmake)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
        )
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_INSTALL_PATH}/bin/tbb.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
        )
