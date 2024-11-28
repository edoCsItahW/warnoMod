// Copyright (c) 2024. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file expection.h
 * @author edocsitahw
 * @version 1.1
 * @date 2024/11/24 下午11:54
 * @brief
 * */
#ifndef NDF_EXPECTION_H
#define NDF_EXPECTION_H
#pragma once

#include "utils.h"
#include <format>
#include <iostream>

#define RAISE(msg, type) Expection(msg, #type, ExpKwargs{__FILE__, __LINE__, __func__})
#define SyntaxError(msg) RAISE(msg, SyntaxError)
#define TypeError(msg) RAISE(msg, "TypeError")
#define ValueError(msg) RAISE(msg, "ValueError")
#define RuntimeError(msg) RAISE(msg, "RuntimeError")

struct ExpKwargs {
    const char* file;
    int line;
    const char* func;
};

class Expection : public std::exception {
private:
    std::string _msg;
    std::string _type;
    ExpKwargs _kwargs;
    mutable std::string _what;

public:
    (Expection)(std::string msg, std::string type, const ExpKwargs& kwargs)
        : std::exception()
        , _msg(std::move(msg))
        , _type(std::move(type))
        , _kwargs(kwargs) {}

    [[nodiscard]] const char* what() const noexcept override {
        _what = std::format("File {}, line {}, in <{}>:\n{}: {}", _kwargs.file, _kwargs.line, _kwargs.func, _type, _msg);
        return _what.c_str();
    }
};

enum Warning {
    UserWarning,
    BytesWarning,
    FutureWarning,
    DeprecationWarning,
    PendingDeprecationWarning,
    SyntaxWarning,
    EncodingWarning,
    ImportWarning,
    ResourceWarning,
    RuntimeWarning,
    UnicodeWarning,
};

void warn(const std::string& msg, Warning cotegory = UserWarning) { std::cerr << enumToStr(cotegory) << ": " << msg << std::endl; }

#endif  // NDF_EXPECTION_H
