cmake_policy(SET CMP0057 NEW)
# 3.29是你的cmake版本
cmake_minimum_required(VERSION 3.28 FATAL_ERROR)
project(NdfPyAPI VERSION 0.1.0 LANGUAGES CXX)  # 大小项目名

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(/wd4819)

include_directories(../include)

set(pybind11_DIR "E:/ProgramFiles/Anaconda/Lib/site-packages/pybind11/share/cmake/pybind11")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../../../py")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

# 3.11是你的python解释器版本
find_package(Python 3.11 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(ndfPyAPI
        build.cpp
        ../src/lexer/token.cpp
        ../src/lexer/tokenProc.cpp
        ../src/lexer/lexer.cpp
        ../src/parser/ast.cpp
        ../src/parser/parser.cpp
        ../src/utils.cpp
)  # 两个参数,小写的项目名和主cpp文件名

set_target_properties(ndfPyAPI PROPERTIES OUTPUT_NAME "ndfPyAPI" SUFFIX ".pyd")

if (MSVC)
    target_link_libraries(ndfPyAPI PRIVATE
            ${CMAKE_SOURCE_DIR}/../lib/confunc_st_MSVC.lib
            ${CMAKE_SOURCE_DIR}/../lib/Debugger_st_MSVC.lib
    )
else()
target_link_libraries(ndfPyAPI PRIVATE
            ${CMAKE_SOURCE_DIR}/../lib/libconfunc_MINGW.a
            ${CMAKE_SOURCE_DIR}/../lib/libDebugger_st_MINGW.a
    )
endif()
