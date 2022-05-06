cmake_minimum_required(VERSION 3.22)
project(node-canny)

set(CMAKE_CXX_STANDARD 17)

# work around for including node-api headers
if (NOT DEFINED CMAKE_JS_VERSION)
    # only work with windows & specified node version
    # not a good idea
    set(CMAKE_JS_INC $ENV{USERPROFILE}/.cmake-js/node-x64/v16.15.0/include/node)
endif ()

add_library(${PROJECT_NAME} SHARED
        ${CMAKE_JS_SRC}
        hello.cpp
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
