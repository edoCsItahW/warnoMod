// Copyright (c) 2024. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file debuger.h
 * @author edocsitahw
 * @version 1.1
 * @date 2024/10/10 下午4:42
 * @brief 调试器API头文件
 * @copyright CC BY-NC-SA 2024. All rights reserved.
 * */

/** @mainpage 调试器API文档
 *
 * @tableofcontents
 *
 * @section intro_sec 简介
 * 该文档介绍了调试器API的使用方法和接口定义.
 *
 * @section product_sec 产品介绍
 * 该调试器是基于C++17-C++23标准编写的模板类,用于帮助开发者调试代码.
 *
 * @section install_sec 安装说明
 *
 * @subsection step1 获取
 * 项目地址: https://github.com/edoCsItahW/C/tree/main/cSeries/CPP/debuger
 *
 * @subsection step2 使用
 *
 * @subsubsection step2.1 直接使用
 * 将src目录下的 @c debuger.h 和 @c 链接文件 复制到项目目录下,并包含头文件即可使用.
 * @note
 * 该库提供了静态链接库和动态链接库,请根据实际情况选择(但目前仅提供MSVC和MINGW编译器的链接库).
 * 1. 静态链接库
 *    - MSVC:
 *        复制 @c debuger_st_MSVC.lib 到项目目录下,并在链接器选项中添加
 *    - MINGW:
 *        复制 @c libdebuger_st_MINGW.a 到项目目录下,并在链接器选项中添加
 * 2. 动态链接库
 *    - MSVC:
 *        复制 @c debuger_dy_MSVC.dll 到项目目录下,并在链接器选项中添加
 *    - MINGW:
 *        复制 @c libdebuger_dy_MINGW.dll.a 到项目目录下,并在链接器选项中添加
 *
 * @section file_sec 文件列表
 * - @ref debuger.h
 * - @ref debuger.cpp
 * @section ref_sec 参考资料
 * - 无
 * @section license_sec 版权信息
 * - 版权所有 © 2024 Xiao Songtao.
 * - 保留一切权利。
 * - 联系作者：2207150234@st.sziit.edu.cn
 * */

#ifndef TEST_DEBUGER_H
#define TEST_DEBUGER_H
#pragma once

#include <functional>
#include <iostream>
#include <source_location>
#include <utility>

#ifdef DLL_BUILD
    #define EXPORT __declspec(dllexport)
#else
    #define EXPORT __declspec(dllimport)
#endif

/** @namespace debuger
 * @brief 调试器命名空间
 * @details 该命名空间包含调试器模板类Debuger
 * @see debuger::Debuger
 * */
namespace dbg {
    template<typename>
    struct funcType;

    template<typename R, typename... Args>
    struct funcType<R (*)(Args...)> {
        using type = R;

        template<std::size_t Index>
        using argType = typename std::tuple_element_t<Index, std::tuple<Args...>>;

        static const std::size_t arity = sizeof...(Args);
    };

    template<typename C, typename R, typename... Args>
    struct funcType<R (C::*)(Args...) const> {
        using type = R;

        template<std::size_t Index>
        using argType = typename std::tuple_element_t<Index, std::tuple<Args...>>;

        static const std::size_t arity = sizeof...(Args);
    };

    template<typename F>
    struct funcType : public funcType<decltype(&F::operator())> {};

    /** @var First
     * @brief 调试器截获的错误是否为程序第一个错误
     * @details 该变量用于记录调试器截获的错误是否为程序第一个错误,默认为false.
     * @note 仅供 @ref debuger::Debuger 类使用.
     * @see debuger::Debuger
     * */
    extern bool First;

    extern bool DEBUG;

    /** @class Debugger
     * @brief 调试器模板类
     * @tparam F 待调试函数类型
     * @warning 如果 @a F 为成员函数,需先将该函数使用 @a std::bind 或者匿名函数绑定到对象上,否则无法调用.
     * @todo 支持被装饰后的新函数支持loc调试参数,以确保调试信息是使用函数时,而不是装饰函数时生成的
     * @note 对于特化版本,有时编译器无法正确推导。
     * 例如:
     * @code
     * struct Test {
     *     template<typename T>
     *     void func(T _t, int _i) { _t do something }
     *
     *     template<typename T>
     *     void func(T _t, double _d) { _t do something else }
     * };
     * @endcode
     * 这时 @c Test::func<T> 被视为一个重载函数,这种情况下编译器无法确定具体函数类型,因为模板参数 @c T 可以是任意类型.
     * 并且此时编译器看到的是一个重载函数列表,而不是具体的函数,这导致 @c T 无法推导.  \n
     * **解决方案**  \n
     * 编译器无法根据调用上下文确定 @c T 的确切类型，所以需要明确指定。这通常通过强制转换来完成。  \n
     * @code
     * using FuncPtr = void (Test::*)(T, int);
     * auto fPtr = static_cast<FuncPtr>(&Test::func<T>);
     * debuger::Debugger dbg(fPtr);
     * dbg(this, 1);
     * @endcode
     * 这样就可以正确推导 @c T 的类型。
     *
     * ## 示例
     *
     * ### 普通函数
     *
     * @code
     * int add(int a, int b) { return a + b; }
     *
     * int main() {
     *     Debugger debugger(add);  // 位置参数loc默认值为std::source_location::current()
     *     debugger(1, 2);  // 调用add函数并打印调试信息
     *     return 0;
     * }
     * @endcode
     *
     * ### 成员函数
     *
     * @code
     * class A {
     * public:
     *     int add(int a, int b) { return a + b; }
     *     int refFunc(int a, int b) {
     *         auto debuger = std::bind(&A::add, this, std::placeholders::_1, std::placeholders::_2);  // 绑定成员函数
     *         return debuger(a, b);
     *
     *         // 特化版本
     *         Debugger debugger(&A::add);
     *         debugger(this, 1, 2);  // 调用A::add函数并打印调试信息
     *         return 0;
     *     }
     * };
     *
     * int main() {
     *     A a;
     *     // 使用std::bind绑定成员函数
     *     auto debuger = std::bind(&A::add, &a, std::placeholders::_1, std::placeholders::_2);  // 绑定成员函数
     *     debuger(1, 2);  // 调用A::add函数并打印调试信息
     *
     *     // 使用lambda表达式绑定成员函数
     *     auto debuger = [&a](int _a, int _b) { return a.add(_a, _b); };
     *     debuger(1, 2);  // 调用A::add函数并打印调试信息
     *
     *     // 使用其特化版本
     *     Debugger debugger(&A::add);
     *     debugger(&a, 1, 2);  // 调用A::add函数并打印调试信息
     *     return 0;
     * }
     * @endcode
     * */
    template<typename F>
    class Debugger {
    public:
        /**
         * @brief 调试器模板类构造函数
         * @details 构造函数初始化调试器参数,并设置调试器是否退出程序的默认值为false.
         * @param func 待调试函数
         * @param exit 调试器是否退出程序,默认为false
         * @param loc 调试器位置信息
         * @remark @a loc 参数默认为std::source_location::current(), @a exit 参数默认为false.
         * */
        explicit Debugger(F func, bool exit = false, std::source_location loc = std::source_location::current());

        /**
         * @brief 调试器模板类调用运算符重载
         * @details 调用调试器模板类,打印调试信息并调用待调试函数.
         * @tparam Args 待调试函数参数类型
         * @param args 待调试函数参数
         * @return 待调试函数返回值类型
         * @note 当 @a _exit 为true时,调试器将退出程序.
         * */
        template<typename... Args>
        auto operator()(Args&&... args) -> decltype(auto);
        private:

        /** @var _func
         * @brief 待调试函数
         * @details 待调试函数,由构造函数初始化.
         * @invariant 自初始化赋值后不应修改
         * @qualifier const
         * */
        const F _func;

        /** @var _exit
         * @brief 调试器是否退出程序
         * @details 调试器是否退出程序,由构造函数初始化.
         * */
        bool _exit;

        /** @var _loc
         * @brief 调试器位置信息
         * @details 调试器位置信息,由构造函数初始化.
         * */
        std::source_location _loc;
    };

    template<class C, typename F>
    class Debugger<F C::*> {
    public:
        explicit Debugger(F C::*func, bool exit = false, std::source_location loc = std::source_location::current());

        template<typename... Args>
        auto operator()(C* obj, Args&&... args) -> decltype(auto);

    private:
        F C::*_func;
        bool _exit;
        std::source_location _loc;
    };

}  // namespace debuger

#include "debuger.hpp"

#endif  // TEST_DEBUGER_H

