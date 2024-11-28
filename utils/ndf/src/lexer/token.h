// Copyright (c) 2024. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file token.h
 * @author edocsitahw
 * @version 1.1
 * @date 2024/11/25 下午3:52
 * @brief
 * */
#ifndef NDF_TOKEN_H
#define NDF_TOKEN_H
#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <vector>
#include <stack>
#include <iostream>  // MSVC构建时需要,其余时候可以移除

enum class TokenType {
    NUMBER,  ///< 数字,1级
    INT,     ///< 整数,2级
    FLOAT,   ///< 浮点数,2级

    STRING,  ///< 字符串,1级

    PATH,  ///< 路径,1级

    LITERAL,     ///< 字面量,1级
    IDENTIFIER,  ///< 标识符,2级

    KW_IS,        ///< is,2级
    KW_MAP,       ///< MAP,2级
    KW_DIV,       ///< div,2级
    KW_TEMPLATE,  ///< template,2级
    KW_EXPORT,    ///< export,2级
    KW_UNNAMED,   ///< unnamed,2级
    KW_NIL,       ///< nil,2级
    KW_PRIVATE,   ///< private,2级
    KW_BOOLEN,    ///< boolen类型(true/false),2级

    GUID,  ///< guid类型,2级

    OPERATOR,  ///< 操作符,0级

    ADD,     ///< +,1级
    SUB,     ///< -,1级
    MUL,     ///< *,1级
    DIV,     ///< /,1级
    MOD,     ///< %,1级
    ASSIGN,  ///< =,1级
    LT,      ///< <,1级
    GT,      ///< >,1级
    LE,      ///< <=,2级
    GE,      ///< >=,2级
    EQ,      ///< ==,2级
    NE,      ///< !=,2级
    OR,      ///< |,1级
    NOT,     ///< !,1级

    LPAREN,     ///< (,1级
    RPAREN,     ///< ),1级
    LBRACE,     ///< {,1级
    RBRACE,     ///< },1级
    LBRACKET,   ///< [,1级
    RBRACKET,   ///< ],1级
    COMMA,      ///< ,1级
    SEMICOLON,  ///< ;,1级
    COLON,      ///< :,1级
    DOT,        ///< .,1级
    DOLLAR,     ///< $,1级
    QUESTION,   ///< ?,1级
    TILDE,      ///< ~,1级

    COMMENT,  ///< 注释,1级

    NEWLINE,  ///< 换行符,0级
    EOF_,     ///< 结束符,0级
    ERROR_,   ///< 错误符,0级
    UNKNOWN   ///< 未知符,0级
};

class Pos {
private:
    size_t _col   = 0;
    size_t _line  = 1;
    size_t _value = 0;

public:
    Pos() = default;
    Pos(size_t line, size_t col, size_t value);
    [[nodiscard]] int col() const;
    [[nodiscard]] int line() const;
    size_t operator()() const;
    Pos newLine();
    Pos& operator++();
    Pos operator++(int);
    Pos& operator--();
    Pos operator--(int);
    Pos& operator+=(int other);
    Pos& operator-=(int other);
    template<typename T>
    bool operator<(const T& other) const;
    template<typename T>
    bool operator<=(const T& other) const;
    template<typename T>
    bool operator>(const T& other) const;
    template<typename T>
    bool operator>=(const T& other) const;
    template<typename T>
    bool operator==(const T& other) const;
    template<typename T>
    bool operator!=(const T& other) const;
    size_t operator+(int other) const;
    size_t operator-(int other) const;
};

//extern const std::stack<

struct Token {
    TokenType type;
    Pos pos;
    std::string value{};
    TokenType super = TokenType::UNKNOWN;  ///< superordinate,标识符的上级类型
//    bool isClosure = false;  ///< 是否是(或{或[的闭合符号
    [[nodiscard]] std::string toString() const;
    friend std::ostream& operator<<(std::ostream& os, const Token& token);
};

using TokenPtr  = std::shared_ptr<Token>;
using TokenPtrs = std::vector<TokenPtr>;

#include "token.hpp"

#endif  // NDF_TOKEN_H
