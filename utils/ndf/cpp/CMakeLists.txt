cmake_minimum_required(VERSION 3.28.1)

project(NDF VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (MSVC)
    add_compile_options(/wd4819)
endif()

include_directories(include)

add_executable(${PROJECT_NAME}
        src/lexer/token.cpp
        src/lexer/lexer.cpp
        src/lexer/tokenProc.cpp
        src/parser/parser.cpp
        src/parser/ast.cpp
        src/utils.cpp
        main.cpp
)

if (MSVC)
target_link_libraries(${PROJECT_NAME} 
    ${CMAKE_SOURCE_DIR}/lib/confunc_st_MSVC_debug.lib
    ${CMAKE_SOURCE_DIR}/lib/debugger_st_MSVC_debug.lib
)

else()
target_link_libraries(${PROJECT_NAME}
    ${CMAKE_SOURCE_DIR}/lib/libconfunc_MINGW.a
    ${CMAKE_SOURCE_DIR}/lib/libDebugger_st_MINGW.a
)
endif()
