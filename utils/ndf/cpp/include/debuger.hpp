// Copyright (c) 2024. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file debuger.hpp
 * @author edocsitahw
 * @version 1.1
 * @date 2024/10/10 下午4:42
 * @brief 实现模板调试器类API
 * @copyright CC BY-NC-SA
 * */

#ifndef TEST_DEBUGER_HPP
#define TEST_DEBUGER_HPP
#pragma once

#include "debuger.h"

namespace dbg {
    template<typename F>
    Debugger<F>::Debugger(F func, bool exit, std::source_location loc)
        : _func(std::move(func))
        , _exit(exit)
        , _loc(loc) {}

    template<typename F>
    template<typename... Args>
    auto Debugger<F>::operator()(Args &&...args) -> decltype(auto) {
        using R = decltype(_func(std::forward<Args>(args)...));
        try {
            return _func(std::forward<Args>(args)...);
        }
        catch (const std::exception &e) {
            if (First) {
                std::cerr << "Traceback (most recent call last):" << std::endl;
                First = false;
            }
            std::cerr << "    File " << _loc.file_name() << ", line " << _loc.line() << ", in <" << _loc.function_name() << ">"
                      << "\n\t" << e.what()<< std::endl;
            if constexpr (std::is_same_v<R, void>) {}
            else {
                if (_exit) std::terminate();
                return R{};
            }
        }
    }

    template<class C, typename F>
    Debugger<F C::*>::Debugger(F C::*func, bool exit, std::source_location loc): _func(func), _exit(exit), _loc(loc) {}

    template<class C, typename F>
    template<typename... Args>
    auto Debugger<F C::*>::operator()(C* obj, Args &&...args) -> decltype(auto) {
        using R = decltype((obj->*_func)(std::forward<Args>(args)...));
        try {
            return (obj->*_func)(std::forward<Args>(args)...);
        }
        catch (const std::exception &e) {
            if (First) {
                std::cerr << "Traceback (most recent call last):" << std::endl;
                First = false;
            }
            std::cerr << "    File " << _loc.file_name() << ", line " << _loc.line() << ", in <" << _loc.function_name() << ">"
                      << "\n\t" << e.what() << std::endl;
            if constexpr (std::is_same_v<R, void>) {}
            else {
                if (_exit) std::terminate();
                return R{};
            }
        }
    }
}  // namespace debuger

#endif  // TEST_DEBUGER_HPP
