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
#include <codecvt>
#include <fstream>
#include <iostream>
#include <locale>

std::string wstr2str(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(wstr);
}


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


int main() {
    auto text = std::get<std::string>(os::readFile(R"(..\..\..\..\GameData\Generated\Gameplay\Decks\Divisions.ndf)", os::Type::UTF8));

    auto tokens = TokenProcessor::process(Lexer::tokenize(text));

    auto parser = Parser(tokens);

    auto result = parser.parse();

    writeToFile(result->toJson(), R"(..\..\ast.json)");

    return 0;
}
