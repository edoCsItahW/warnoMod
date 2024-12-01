// Copyright (c) 2024. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file token.cpp
 * @author edocsitahw
 * @version 1.1
 * @date 2024/11/25 下午3:52
 * @brief
 * */
#include "token.h"
#include "../utils.h"
#include <format>

Pos::Pos(size_t line, size_t col, size_t value)
    : _line(line)
    , _col(col)
    , _value(value) {}

int Pos::col() const { return static_cast<int>(_col); }

int Pos::line() const { return static_cast<int>(_line); }

size_t Pos::operator()() const { return _value; }

Pos Pos::newLine() {
    auto old = *this;
    _line++;
    _col = 0;
    _value++;
    return old;
}

Pos& Pos::operator++() {
    _col++;
    _value++;
    return *this;
}

Pos Pos::operator++(int) {
    auto old = *this;
    _col++;
    _value++;
    return old;
}

Pos& Pos::operator--() {
    _col--;
    _value--;
    return *this;
}

Pos Pos::operator--(int) {
    auto old = *this;
    _col--;
    _value--;
    return old;
}

Pos& Pos::operator+=(const int other) {
    _col += other;
    _value += other;
    return *this;
}

Pos& Pos::operator-=(const int other) {
    _col -= other;
    _value -= other;
    return *this;
}

size_t Pos::operator+(const int other) const { return _value + other; }

size_t Pos::operator-(const int other) const { return _value - other; }

std::string Token::toString() const { return std::format("Token<{},{}>[{}]('{}')", pos.line(), pos.col(), enumToStr(type), value); }

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << token.toString();
    return os;
}
