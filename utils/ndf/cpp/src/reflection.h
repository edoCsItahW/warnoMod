/**
 * @file reflection.h
 * @author edocsitahw
 * @version 1.1
 * @date 2024/12/12 下午8:20
 * @brief C++静态反射库
 * @copyright CC BY-NC-SA 4.0. All rights reserved.
 * */

// Ubpa Static Reflection -- 99 lines
// Repository: https://github.com/Ubpa/USRefl
// License: https://github.com/Ubpa/USRefl/blob/master/LICENSE

#ifndef TEST_REFLECTION_H
#define TEST_REFLECTION_H
#pragma once

#include <string_view>
#include <tuple>


#if __GNUC__ || __clang__
    #define TSTR(s)                                                                                                                                                                                    \
        ([] {                                                                                                                                                                                          \
            struct tmp {                                                                                                                                                                               \
                static constexpr auto get() { return std::basic_string_view{s}; }                                                                                                                      \
            };                                                                                                                                                                                         \
            return detl::TSTRH(tmp{});                                                                                                                                                                 \
        }())
#elif _MSC_VER
    /** @def TSTR
     * @brief 创建常量字符串
     * @param s 字符串内容
     * @return 常量字符串
     * */
    #define TSTR(s)                                                                                                                                                                                    \
        ([] {                                                                                                                                                                                          \
            constexpr std::basic_string_view str{s};                                                                                                                                                   \
            return detl::TStr<detl::FCStr<typename decltype(str)::value_type, str.size()>{str}>{};                                                                                                     \
        }())
#endif
namespace detl {
#if __GNUC__ || __clang__
    /** @struct TStr
     * @brief
     * */
    template<typename C, C... chars>
    struct TStr {
        using Char = C;

        template<typename T>
        static constexpr bool is(T = {}) {
            return std::is_same_v<T, TStr>;
        }

        static constexpr const Char* data() { return _data; }

        static constexpr std::size_t size() { return sizeof...(chars); }

        static constexpr std::basic_string_view<Char> view() { return _data; }

    private:
        static constexpr Char _data[]{chars..., Char(0)};
    };

    template<typename Char, typename T, std::size_t... Ns>
    constexpr auto TSTRHI(std::index_sequence<Ns...>) {
        return TStr<Char, T::get()[Ns]...>{};
    }

    template<typename T>
    constexpr auto TSTRH(T) {
        return TSTRHI<typename decltype(T::get())::value_type, T>(std::make_index_sequence<T::get().size()>{});
    }
#elif _MSC_VER
    /** @struct FCStr
     * @brief 固定长度字符串(Fixed Constant String)
     * @details 特别的包装字符串类型,用于实现字符串字面量的constexpr化,并提供相关元信息
     * @tparam C 字符类型
     * @tparam N 字符串长度
     *
     * @see TStr
     * */
    template<typename C, std::size_t N>
    struct FCStr {
        /** @typedef ValueType
         * @brief 重定义模板参数-字符串类型@c C
         * @details 为元信息接口提供统一的类型定义,将模板名称@c C 重定义为@c ValueType
         * */
        using ValueType = C;
        /** @var data
         * @brief 字符串内容
         * @remark 在struct中初始化,实际在构造函数中赋值
         * */
        ValueType data[N + 1]{};
        /** @var size
         * @brief 字符串长度
         * @details 字符串长度,用于元信息接口
         * @qualifier constexpr
         * */
        static constexpr std::size_t size = N;

        /**
         * @brief FCStr构造函数
         * @param str 字符串内容
         * @details 构造函数,将字符串内容拷贝到data中
         * */
        constexpr FCStr(std::basic_string_view<ValueType> str) {
            for (std::size_t i{0}; i < size; ++i) data[i] = str[i];
        }
    };

    /** @struct TStr
     * @brief 将字面量字符串转换为编译期常量字符串
     * @tparam str 字符串内容
     * */
    template<FCStr str>
    struct TStr {
        /** @typedef Char
         * @brief 重定义模板参数-字符串类型@c C
         * @details 为元信息接口提供统一的类型定义,将模板类型@c FCStr::ValueType 重定义为@c Char
         * @note 由于MSVC的模板参数推导机制,无法直接使用@c FCStr::ValueType,因此使用@c decltype(str)::ValueType代替
         * */
        using Char = typename decltype(str)::ValueType;

        /**
         * @brief 用于判断类型@c T是否为@c TStr 的编译期静态函数
         * @tparam T 待判断类型
         * @return 若@c T为@c TStr,则返回true,否则返回false
         * @retval true  @c T为@c TStr
         * @retval false @c T不为@c TStr
         * @qualifier constexpr
         * */
        template<typename T>
        static constexpr bool is(T = {}) { return std::is_same_v<T, TStr>; }

        /**
         * @brief 获取字符串内容的编译器静态函数
         * @return 字符串内容
         * @qualifier constexpr
         * */
        static constexpr auto data() { return str.data; }

        /**
         * @brief 获取字符串长度的编译器静态函数
         * @return 字符串长度
         * @qualifier constexpr
         * */
        static constexpr auto size() { return str.size; }

        /**
         * @brief 获取字符串内容的编译期字符串视图的编译器静态函数
         * @return 字符串内容的编译期字符串视图
         * @qualifier constexpr
         * */
        static constexpr std::basic_string_view<Char> view() { return str.data; }
    };

#endif

    /**
     * @brief 遍历列表,查找第一个满足条件的元素的索引
     * @details 遍历列表,查找第一个满足条件的元素的索引,使用递归算法实现
     * @note 重载版本,未找到情况
     * @tparam L 列表类型
     * @tparam F 条件函数类型
     * @retval -1 未找到
     * @qualifier constexpr
     * */
    template<class L, class F>
    constexpr std::size_t findIf(const L&, F&&, std::index_sequence<>) { return -1; }

    /**
     * @brief 遍历列表,查找第一个满足条件的元素的索引
     * @details 遍历列表,查找第一个满足条件的元素的索引,使用递归算法实现
     * @tparam L 列表类型
     * @tparam F 条件函数类型
     * @tparam N0 起始索引
     * @tparam Ns 后续索引
     * @return 若找到,则返回索引,否则返回-1
     * @qualifier constexpr
     * */
    template<class L, class F, std::size_t N0, std::size_t... Ns>
    constexpr std::size_t findIf(const L& l, F&& f, std::index_sequence<N0, Ns...>) {
        return f(l.template get<N0>()) ? N0 : findIf(l, std::forward<F>(f), std::index_sequence<Ns...>{});
    }

    /**
     * @brief 遍历列表,对每个元素进行累加
     * @details 遍历列表,对每个元素进行累加,使用递归算法实现
     * @tparam L 列表类型
     * @tparam F 累加函数类型
     * @tparam R 初始值类型
     * @return 累加结果
     * @qualifier constexpr
     * */
    template<class L, class F, class R>
    constexpr auto sum(const L&, F&&, R r, std::index_sequence<>) { return r; }

    /**
     * @brief 遍历列表,对每个元素进行累加
     * @details 遍历列表,对每个元素进行累加,使用递归算法实现
     * @tparam L 列表类型
     * @tparam F 累加函数类型
     * @tparam R 初始值类型
     * @tparam N0 起始索引
     * @tparam Ns 后续索引
     * @return 累加结果
     * @qualifier constexpr
     * */
    template<class L, class F, class R, std::size_t N0, std::size_t... Ns>
    constexpr auto sum(const L& l, F&& f, R r, std::index_sequence<N0, Ns...>) {
        return sum(l, std::forward<F>(f), f(std::move(r), l.template get<N0>()), std::index_sequence<Ns...>{});
    }

    /**
     * @brief 深度优先(Deep First Search, DFS)版本的累加算法
     * @tparam D 深度
     * @tparam T 类型
     * @tparam R 初始值类型
     * @tparam F 累加函数类型
     * @return 累加结果
     * @qualifier constexpr
     * */
    template<std::size_t D, class T, class R, class F>
    constexpr auto sumDFS(T t, F&& f, R r) {
        return t.bases.sum(std::move(r), [&](auto r, auto b) {
            if constexpr (b.is_virtual) return sumDFS<D + 1>(b.info, std::forward<F>(f), std::move(r));
            return sumDFS<D + 1>(b.info, std::forward<F>(f), std::forward<F>(f)(std::move(r), b.info, D + 1));
        });
    }

    /**
     * @brief 遍历类型信息,对每个成员变量进行操作
     * @details 遍历类型信息,对每个成员变量进行操作,使用递归算法实现
     * @tparam TI 类型信息
     * @tparam U 成员变量类型
     * @tparam F 成员变量操作函数类型
     * @qualifier constexpr
     * */
    template<class TI, class U, class F>
    constexpr void forEachVar(TI, U&& u, F&& f) {
        TI::fields.forEach([&](auto&& k) {
            using K = std::decay_t<decltype(k)>;
            if constexpr (!K::is_static && !K::is_func) std::forward<F>(f)(k, std::forward<U>(u).*(k.value));
        });
        TI::bases.forEach([&](auto b) {
            if constexpr (!b.is_virtual) forEachVar(b.info, b.info.forward(std::forward<U>(u)), std::forward<F>(f));
        });
    }
}  // namespace detl

namespace refl {
    /** @struct NamedValueBase
     * @brief 命名值基类
     * @tparam N 名称类型
     * @details 命名值基类,用于提供统一的名称接口,并提供名称元信息
     * @see NamedValue
     * */
    template<class N>
    struct NamedValueBase {
        /** @typedef TName
         * @brief 重定义模板参数-名称类型@c N
         * @details 为元信息接口提供统一的类型定义,将模板名称@c N 重定义为@c TName
         * */
        using TName                            = N;
        /** @var name
         * @brief 名称
         * @details 编译器静态常量,用于提供名称元信息
         * @qualifier constexpr
         * */
        static constexpr std::string_view name = TName::view();
    };

    /** @struct NamedValue
     * @brief 命名值
     * @tparam N 名称类型
     * @tparam T 值类型
     * @details 命名值,用于提供统一的名称接口,并提供名称元信息,并提供值接口
     * @see NamedValueBase
     * */
    template<class N, class T>
    struct NamedValue : NamedValueBase<N> {
        /** @var value
         * @brief 值
         * @details 值接口,用于提供值元信息
         * */
        T value;

        /** @var hasValue
         * @brief 是否有值
         * @details 编译器静态常量,用于提供值元信息
         * @qualifier constexpr
         * */
        static constexpr bool hasValue = true;

        constexpr NamedValue(T v)
            : value{v} {}

        /**
         * @brief 重载赋值运算符
         * @tparam U 赋值类型
         * @param v 赋值对象
         * @return 赋值结果
         * */
        template<class U>
        constexpr bool operator==(U v) const {
            if constexpr (std::is_same_v<T, U>)
                return value == v;
            return false;
        }
    };

    /** @struct NamedValue<N, void>
     * @brief 模板类@c NamedValue 的特化版本,用于提供无值版本
     * @tparam N 名称类型
     * @see NamedValue
     * */
    template<class N>
    struct NamedValue<N, void> : NamedValueBase<N> {
        /** @var hasValue
         * @brief 是否有值
         * @details 编译器静态常量,用于提供值元信息
         * @qualifier constexpr
         * */
        static constexpr bool hasValue = false;

        /**
         * @brief 重载赋值运算符
         * @tparam U 赋值类型
         * @param v 赋值对象
         * @return 赋值结果
         * */
        template<class U>
        constexpr bool operator==(U) const {
            return false;
        }
    };

    /** @struct ElemList
     * @brief 元素列表
     * @tparam Es 元素类型列表
     * */
    template<typename... Es>
    struct ElemList {
        /** @var elems
         * @brief 元素列表
         * */
        std::tuple<Es...> elems;

        /** @var size
         * @brief 元素数量
         * @details 编译器静态常量,用于提供元素数量元信息
         * @qualifier constexpr
         * */
        static constexpr std::size_t size = sizeof...(Es);

        constexpr ElemList(Es... elems)
            : elems{elems...} {}

        template<class V, class F>
        constexpr auto sum(V init, F&& func) const { return detl::sum(*this, std::forward<F>(func), std::move(init), std::make_index_sequence<size>{}); }

        template<class F>
        constexpr void forEach(F&& f) const {
            sum(0, [&](auto, const auto& field) {
                std::forward<F>(f)(field);
                return 0;
            });
        }

        template<class S>
        static constexpr bool contains(S = {}) { return (Es::TName::template is<S>() || ...); }

        template<class F>
        constexpr std::size_t findIf(F&& f) const { return detl::findIf(*this, std::forward<F>(f), std::make_index_sequence<size>{}); }

        template<class S>
        constexpr const auto& find(S = {}) const {
            constexpr std::size_t idx = []() {
                constexpr decltype(S::view()) names[]{Es::name...};
                for (std::size_t i = 0; i < size; i++)
                    if (S::view() == names[i]) return i;
                return static_cast<std::size_t>(-1);
            }();
            return get<idx>();
        }

        template<class T>
        constexpr std::size_t findValue(const T& v) const { return findIf([&v](auto e) { return e == v; }); }

        template<typename T, typename S>
        constexpr const T* valuePtrOfName(S n) const {
            return sum(nullptr, [n](auto r, const auto& e) {
                if constexpr (std::is_same_v<decltype(e.value), T>)
                    return e.name == n ? &e.value : r;
                else
                    return r;
            });
        }

        template<typename T, typename S>
        constexpr const T& valueOfName(S n) const { return *valuePtrOfName<T>(n); }

        template<class T, class C = char>
        constexpr auto nameOfValue(const T& v) const { return sum(std::basic_string_view<C>{}, [&v](auto r, auto&& e) { return e == v ? e.name : r; }); }

        template<class E>
        constexpr auto push(E e) const { return std::apply([e](auto... es) { return ElemList<Es..., E>{es..., e}; }, elems); }

        template<class E>
        constexpr auto insert(E e) const {
            if constexpr ((std::is_same_v<Es, E> || ...)) return *this;
            else return push(e);
        }

        template<std::size_t N>
        [[nodiscard]] constexpr const auto& get() const { return std::get<N>(elems); }

#define USRefl_ElemList_GetByValue(list, value) list.get<list.findValue(value)>()
    };

    template<class N, class T>
    struct Attr : NamedValue<N, T> {
        constexpr Attr(N, T v)
            : NamedValue<N, T>{v} {}
    };

    template<class N>
    struct Attr<N, void> : NamedValue<N, void> {
        constexpr Attr(N) {}
    };

    template<typename... As>
    struct AttrList : ElemList<As...> {
        constexpr AttrList(As... as)
            : ElemList<As...>{as...} {}
    };

    template<bool s, bool f>
    struct FTraitsB {
        static constexpr bool is_static = s, is_func = f;
    };

    template<class T>
    struct FTraits : FTraitsB<true, false> {};  // default is enum

    template<class U, class T>
    struct FTraits<T U::*> : FTraitsB<false, std::is_function_v<T>> {};

    template<class T>
    struct FTraits<T*> : FTraitsB<true, std::is_function_v<T>> {};  // static member

    template<class Name, class T, class AList>
    struct Field : FTraits<T>, NamedValue<Name, T> {
        AList attrs;

        constexpr Field(Name, T v, AList as = {})
            : NamedValue<Name, T>{v}
            , attrs{as} {}
    };

    template<typename... Fs>
    struct FieldList : ElemList<Fs...> {
        constexpr FieldList(Fs... fs)
            : ElemList<Fs...>{fs...} {}
    };

    /** @struct TypeInfo
     * @brief 类型信息
     * @tparam T 类型
     *
     * ## 使用方法
     *
     * ### 普通类
     * @code{.cpp}
     * // 原类定义
     * struct [[size(8)]] Point {
     *     [[not_serialize]] float x;
     *     [[info("hello")]] float y;
     *     static constexpr std::size_t id = 1024;
     *
     *     float Sum() const { return x + y; }
     * };
     *
     * // 类型信息定义
     * template<>
     * struct refl::TypeInfo<Point> : TypeInfoBase<Point> {
     *     static constexpr char name[6]   = "Point";
     *     static constexpr AttrList attrs = {
     *         Attr{TSTR("size"), 8},
     *     };
     *     static constexpr FieldList fields = {
     *         Field{
     *               TSTR("x"), &Type::x,
     *               AttrList{
     *                 Attr{TSTR("not_serialize")},
     *             }
     *         },
     *         Field{
     *               TSTR("y"), &Type::y,
     *               AttrList{
     *                 Attr{TSTR("info"), "hello"},
     *             }
     *         },
     *         Field{TSTR("id"), Type::id},
     *         Field{TSTR("Sum"), &Type::Sum},
     *     };
     * };
     *
     * // 使用
     * int main() {
     *     Point p{1.0f, 2.0f};
     *
     *     // 遍历类字段
     *     TypeInfo<Point>::fields.forEach([](auto field) {
     *         cout << field.name << endl;
     *
     *         // 遍历字段属性
     *         field.attrs.forEach([](auto attr) {
     *             cout << attr.name;
     *             if constexpr (attr.hasValue) cout << ": " << attr.value;
     *             cout << endl;
     *         });
     *     });
     *
     * // 访问字段(编译期)
     * constexpr const auto& y_field = TypeInfo<Point>::fields.find(TSTR("y"));
     * static_assert(y_field.name == "y");
     *
     * static_assert(TypeInfo<Point>::fields.contains(TSTR("x")));
     *
     * TypeInfo<Point>::forEachVarOf(p, [](auto field, auto&& var) { cout << field.name << " : " << var << endl; });
     *
     * TypeInfo<Point>::fields.forEach([](auto field) {
     * if constexpr (field.is_static) cout << field.name << ": " << field.value << endl;
     * });
     *
     * TypeInfo<Point>::fields.forEach([p](auto field) {
     * if constexpr (field.is_func) {
     *     if (field.name != "Sum") return;
     *     cout << (p.*(field.value))() << endl;
     * }
     * });
     * static_assert(TypeInfo<Point>::fields.nameOfValue(Point::id) == "id");
     * static_assert(TypeInfo<Point>::fields.valueOfName<std::size_t>("id") == Point::id);
     * }
     * @endcode
     * */
    template<class T>
    struct TypeInfo;  // TypeInfoBase, name, fields, attrs

    template<class T, bool IsVirtual = false>
    struct Base {
        static constexpr auto info       = TypeInfo<T>{};
        static constexpr bool is_virtual = IsVirtual;
    };

    template<typename... Bs>
    struct BaseList : ElemList<Bs...> {
        constexpr BaseList(Bs... bs)
            : ElemList<Bs...>{bs...} {}
    };

    template<typename... Ts>
    struct TypeInfoList : ElemList<Ts...> {
        constexpr TypeInfoList(Ts... ts)
            : ElemList<Ts...>{ts...} {}
    };

    template<class T, typename... Bases>
    struct TypeInfoBase {
        using Type = T;
        static constexpr BaseList bases{Bases{}...};

        template<class U>
        static constexpr auto&& forward(U&& derived) {
            if constexpr (std::is_same_v<std::decay_t<U>, U>)
                return static_cast<Type&&>(derived);  // right
            else if constexpr (std::is_same_v<std::decay_t<U>&, U>)
                return static_cast<Type&>(derived);  // left
            else
                return static_cast<const std::decay_t<U>&>(derived);  // std::is_same_v<const std::decay_t<U>&, U>
        }

        static constexpr auto virtualBases() {
            return bases.sum(ElemList<>{}, [](auto acc, auto base) {
                auto concated = base.info.virtualBases().sum(acc, [](auto acc, auto b) { return acc.insert(b); });
                if constexpr (!base.is_virtual)
                    return concated;
                else
                    return concated.insert(base.info);
            });
        }

        template<class R, class F>
        static constexpr auto sumDFS(R r, F&& f) {
            return detl::sumDFS<0>(TypeInfo<Type>{}, std::forward<F>(f), virtualBases().sum(std::forward<F>(f)(std::move(r), TypeInfo<Type>{}, 0), [&](auto acc, auto vb) {
                return std::forward<F>(f)(std::move(acc), vb, 1);
            }));
        }

        template<class F>
        static constexpr void forEachDFS(F&& f) {
            sumDFS(0, [&](auto, auto t, auto d) {
                std::forward<F>(f)(t, d);
                return 0;
            });
        }

        template<class U, class F>
        static constexpr void forEachVarOf(U&& obj, F&& func) {
            virtualBases().forEach([&](auto vb) {
                vb.fields.forEach([&](const auto& fld) {
                    using Fld = std::decay_t<decltype(fld)>;
                    if constexpr (!Fld::is_static && !Fld::is_func) std::forward<F>(func)(fld, std::forward<U>(obj).*(fld.value));
                });
            });
            detl::forEachVar(TypeInfo<Type>{}, std::forward<U>(obj), std::forward<F>(func));
        }
    };

    template<class N>
    Attr(N) -> Attr<N, void>;
    template<class N, class T>
    Field(N, T) -> Field<N, T, AttrList<>>;
}  // namespace refl

#endif  // TEST_REFLECTION_H
