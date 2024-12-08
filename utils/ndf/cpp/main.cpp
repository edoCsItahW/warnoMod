// Copyright (c) 2024. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file main.cpp
 * @author edocsitahw
 * @version 1.1
 * @date 2024\11\24 上午1:33
 * @brief
 * */

#include "confunc.h"
#include "src\lexer\tokenProc.h"
#include "src\parser\parser.h"
#include <fstream>
#include <iostream>
#include <filesystem>


int writeToFile(const std::string& text, const std::string& path) {
    std::ofstream out(path);

    if (!out.is_open()) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return -1;
    }

    out << text;

    out.close();

    return 0;
}


int main(int argc, char* argv[]) {

    // -t=--target 目标文件路径(必选参数)
    // -o=--output 输出文件路径(可选参数)
    // -d=--debug 调试模式(可选参数)

//    std::string target;
//    std::string output;
//    bool debug = false;
//
//    for (int i = 1; i < argc; i++) {
//        std::string arg = argv[i];
//        if (arg == "-t" || arg == "--target")
//            target = argv[++i];
//        else if (arg == "-o" || arg == "--output")
//            output = argv[++i];
//        else if (arg == "-d" || arg == "--debug")
//            debug = true;
//    }
//
//    if (target.empty())
//        throw ArgumentError("Target file is not specified.");

    auto text = std::get<std::string>(os::readFile(R"(.\GameData\Generated\Gameplay\Decks\DivisionRules.ndf)", os::Type::UTF8, R"(E:\codeSpace\codeSet\ndf\warnoMod)"));


    auto tokens = TokenProcessor::process(Lexer::tokenize(text));
//    for (auto& token : tokens) std::cout << token->toString() << std::endl;
    auto parser = Parser(tokens);

    auto result = parser.parse();

//    writeToFile(result->toJson(), R"(..\..\ast.json)");
    writeToFile(result->toJson(), R"(..\..\DivisionRules.json)");

    return 0;
}
