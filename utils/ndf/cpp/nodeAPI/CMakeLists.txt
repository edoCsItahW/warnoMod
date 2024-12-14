cmake_minimum_required(VERSION 3.28.1)

project(ndfNodeAPI VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (MSVC)
    add_compile_options(/wd4819)
    add_compile_options(/wd4715)
endif ()

include_directories(${CMAKE_JS_INC} ${CMAKE_SOURCE_DIR}/../include)
#file(GLOB SOURCE_FILES "*.cpp")
#file(GLOB SOURCE_FILES "./py/*.cpp")


add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${CMAKE_JS_SRC}
        ../src/utils.cpp
        ../src/lexer/lexer.cpp
        ../src/lexer/token.cpp
        ../src/lexer/tokenProc.cpp
        ../src/parser/parser.cpp
        ../src/parser/ast.cpp
        build.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB}
        ${CMAKE_SOURCE_DIR}/../lib/python311.lib
        ${CMAKE_SOURCE_DIR}/../lib/node.lib
)

include(../cmake/pybind11.cmake)
include(../cmake/node.cmake)

execute_process(
        COMMAND node -p "require('node-addon-api').include"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE NODE_ADDON_API_DIR
)

# 指定生成的.node文件输出目录
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/node)

string(REPLACE "\n" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
string(REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})


target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})


add_definitions(-DNAPI_VERSION=4)
