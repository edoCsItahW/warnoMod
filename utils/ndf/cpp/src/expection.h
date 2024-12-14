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
 *
 * @if zh
 * @brief 异常处理类头文件
 *
 * @else
 * @brief Header file of exception handling class.
 *
 * @endif
 *
 * @copyright CC BY-NC-SA 2024. All rights reserved.
 * */
#ifndef NDF_EXPECTION_H
#define NDF_EXPECTION_H
#pragma once

#include "utils.h"
#include <format>
#include <iostream>

/**
 * @if zh
 * @defgroup Macro 宏定义
 * @brief 所有宏定义
 *
 * @else
 * @defgroup Macro Macros
 * @brief All macro definitions.
 *
 * @endif
 *
 * @{
 * */

/** @def RAISE
 *
 * @if zh
 * @brief 异常抛出宏
 * @details 该宏用于简化抛出异常，其参数为异常信息、异常类型、异常相关信息。
 * @param msg 异常信息
 * @param type 异常类型
 *
 * @else
 * @brief Macro for raising exceptions.
 * @details This macro simplifies the process of throwing exceptions by providing the exception message, type, and related information.
 * @param msg Exception message.
 * @param type Exception type.
 *
 * @endif
 * */
#define RAISE(msg, type) Expection(msg, #type, ExpKwargs{__FILE__, __LINE__, __func__})

/** @def SyntaxError
 *
 * @if zh
 * @brief 语法错误异常
 * @details 该宏用于抛出语法错误异常，抛出的异常信息形如: @e "File <文件名>, line <行号>, in <函数名>: SyntaxError: <异常信息>" 。
 * @param msg 异常信息
 *
 * # 用法
 * @code{.cpp}
 * throw SyntaxError("Invalid syntax");
 * > File <文件名>, line <行号>, in <函数名>: SyntaxError: Invalid syntax
 * @endcode
 *
 * @else
 * @brief Syntax error exception.
 * @details This macro is used to throw a syntax error exception, which will result in an exception message in the format of "File <file name>, line <line number>, in <function name>: SyntaxError: <exception message>".
 * @param msg Exception message.
 *
 * # Usage
 * @code{.cpp}
 * throw SyntaxError("Invalid syntax");
 * > File <file name>, line <line number>, in <function name>: SyntaxError: Invalid syntax
 * @endcode
 *
 * @endif
 *
 * @see RAISE
 * */
#define SyntaxError(msg) RAISE(msg, SyntaxError)

/**
 * @def TypeError_
 *
 * @if zh
 * @brief 类型错误异常，与@ref SyntaxError 类似.
 *
 * @else
 * @brief Type error exception, similar to @ref SyntaxError.
 *
 * @endif
 *
 * @see SyntaxError
 * */
#define TypeError_(msg) RAISE(msg, "TypeError")

/**
 * @def ValueError
 *
 * @if zh
 * @brief 值错误异常，与@ref SyntaxError 类似.
 *
 * @else
 * @brief Value error exception, similar to @ref SyntaxError.
 *
 * @endif
 *
 * @see SyntaxError
 * */
#define ValueError(msg) RAISE(msg, "ValueError")

/**
 * @def RuntimeError
 *
 * @if zh
 * @brief 运行时错误异常，与@ref SyntaxError 类似.
 *
 * @else
 * @brief Runtime error exception, similar to @ref SyntaxError.
 *
 * @endif
 *
 * @see SyntaxError
 * */
#define RuntimeError(msg) RAISE(msg, "RuntimeError")

/** @def ArgumentError
 *
 * @if zh
 * @brief 参数错误异常, 与@ref SyntaxError 类似.
 *
 * @else
 * @brief Argument error exception, similar to @ref SyntaxError.
 *
 * @endif
 *
 * @see SyntaxError
 * */
#define ArgumentError(msg) RAISE(msg, "ArgumentError")

/** @def KeyError
 *
 * @if zh
 * @brief 键错误异常, 与@ref SyntaxError 类似.
 *
 * @else
 * @brief Key error exception, similar to @ref SyntaxError.
 *
 * @endif
 *
 * @see SyntaxError
 * */
#define KeyError(msg) RAISE(msg, "KeyError")

/** @} */

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
    Expection(std::string msg, std::string type, const ExpKwargs& kwargs)
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
