cmake_minimum_required(VERSION 3.28.1)

project(NDF VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories("E:/codeSpace/codeSet/C/cSeries/CPP/conFunc/src" "E:/codeSpace/codeSet/C/cSeries/CPP/debuger/src")

add_executable(${PROJECT_NAME}
        src/lexer/token.cpp
        src/lexer/lexer.cpp
        src/lexer/tokenProc.cpp
        src/parser/parser.cpp
        src/parser/ast.cpp
        src/utils.cpp
        main.cpp
)

target_link_libraries(${PROJECT_NAME} 
        "E:/codeSpace/codeSet/C/cSeries/CPP/conFunc/lib/libconfunc_MINGW.a"
        "E:/codeSpace/codeSet/C/cSeries/CPP/debuger/lib/libDebugger_st_MINGW.a"
)
