// Copyright (c) 2024. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file utils.h
 * @author edocsitahw
 * @version 1.1
 * @date 2024/11/25 下午3:57
 * @brief
 * */
#ifndef NDF_UTILS_H
#define NDF_UTILS_H
#pragma once

#include <cstddef>
#include <type_traits>

template<auto Value>
constexpr auto enumToStr();

template<typename T, size_t N = 0>
constexpr auto enumMax();

template<typename T>
    requires std::is_enum_v<T>
constexpr auto enumToStr(T value);

#include "utils.hpp"

#endif  // NDF_UTILS_H
