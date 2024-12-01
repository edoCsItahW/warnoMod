cmake_policy(SET CMP0057 NEW)
# 3.29是你的cmake版本
cmake_minimum_required(VERSION 3.29 FATAL_ERROR)
project(NdfPyAPI VERSION 0.1.0 LANGUAGES CXX)  # 大小项目名

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(/wd4819)

include_directories("E:/codeSpace/codeSet/C/cSeries/CPP/conFunc/src" "E:/codeSpace/codeSet/C/cSeries/CPP/debuger/src")

set(pybind11_DIR "E:/ProgramFiles/Anaconda/Lib/site-packages/pybind11/share/cmake/pybind11")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "E:/codeSpace/codeSet/Python/privateProject/ndf")
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

target_link_libraries(ndfPyAPI PRIVATE
#        "E:/codeSpace/codeSet/C/cSeries/CPP/conFunc/lib/libconfunc_MINGW.a"
        "E:/codeSpace/codeSet/C/cSeries/CPP/conFunc/lib/confunc_MSVC.lib"
#        "E:/codeSpace/codeSet/C/cSeries/CPP/debuger/lib/libDebugger_st_MINGW.a"
        "E:/codeSpace/codeSet/C/cSeries/CPP/debuger/lib/Debugger_st_MSVC.lib"
)

