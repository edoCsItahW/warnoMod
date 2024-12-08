// Copyright (c) 2024. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file confunc.h
 * @author edocsitahw
 * @version 1.1
 * @date 2024/10/10 下午10:00
 * @brief
 * @copyright CC BY-NC-SA
 * */

#ifndef DEBUGER_CONFUNC_H
#define DEBUGER_CONFUNC_H
#pragma once

#include <string>
#include <variant>

namespace os {
    using AnyStr = std::variant<std::string, std::wstring>;

    enum class Type {
        UTF8,
        GBK,
        ANSI,
        ASCII
    };

    std::string completePath(const std::string& _str);

    AnyStr readFile(const std::string& _filePath, Type _type = Type::UTF8, std::string cwd = "");
}

#endif