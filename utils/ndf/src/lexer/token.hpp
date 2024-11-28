// Copyright (c) 2024. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file token.hpp
 * @author edocsitahw
 * @version 1.1
 * @date 2024/11/25 下午3:58
 * @brief
 * */
#ifndef NDF_TOKEN_HPP
#define NDF_TOKEN_HPP
#pragma once

template<typename T>
bool Pos::operator<(const T &other) const {
    if constexpr (std::is_same_v<T, Pos>) return _value < other.operator()();
    else if (std::is_same_v<T, int>) return static_cast<int>(_value) < other;
    else if (std::is_same_v<T, size_t>) return _value < other;
}

template<typename T>
bool Pos::operator<=(const T &other) const {
    if constexpr (std::is_same_v<T, Pos>) return _value <= other.operator()();
    else if (std::is_same_v<T, int>) return static_cast<int>(_value) <= other;
    else if (std::is_same_v<T, size_t>) return _value <= other;
}

template<typename T>
bool Pos::operator>(const T &other) const {
    if constexpr (std::is_same_v<T, Pos>) return _value > other.operator()();
    else if (std::is_same_v<T, int>) return static_cast<int>(_value) > other;
    else if (std::is_same_v<T, size_t>) return _value > other;
}

template<typename T>
bool Pos::operator>=(const T &other) const {
    if constexpr (std::is_same_v<T, Pos>) return _value >= other.operator()();
    else if (std::is_same_v<T, int>) return static_cast<int>(_value) >= other;
    else if (std::is_same_v<T, size_t>) return _value >= other;
}

template<typename T>
bool Pos::operator==(const T &other) const {
    if constexpr (std::is_same_v<T, Pos>) return _value == other.operator()();
    else if (std::is_same_v<T, int>) return static_cast<int>(_value) == other;
    else if (std::is_same_v<T, size_t>) return _value == other;
}

template<typename T>
bool Pos::operator!=(const T &other) const {
    if constexpr (std::is_same_v<T, Pos>) return _value != other.operator()();
    else if (std::is_same_v<T, int>) return static_cast<int>(_value) != other;
    else if (std::is_same_v<T, size_t>) return _value != other;
}

#endif  // NDF_TOKEN_HPP
