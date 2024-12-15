/**
 * @file build.h
 * @author edocsitahw
 * @version 1.1
 * @date 2024/12/11 下午3:16
 * @brief
 * */
#ifndef NDFNODEAPI_BUILD_H
#define NDFNODEAPI_BUILD_H
#pragma once

#include "../src/lexer/lexer.h"
#include "../src/lexer/tokenProc.h"
#include "../src/parser/parser.h"
#include "../src/utils.h"
#include "napi.h"
#include <functional>

namespace node = Napi;

node::Function getConstructor(const node::Env& env, std::string name);

template<typename F, typename T, typename R>
concept Convertor = std::invocable<F, const T&> && std::same_as<std::invoke_result_t<F, const T&>, R>;

template<typename T, typename F>
std::vector<T> arrConvert(const node::Array& arr, F func) requires Convertor<F, node::Value, T> {
    std::vector<T> res;
    for (size_t i = 0; i < arr.Length(); i++) res.push_back(func(arr[i]));
    return res;
}

template<typename T, typename F>
node::Array arrConvert(const node::Env& env, const std::vector<T>& vec, F func) requires Convertor<F, T, node::Value> {
    node::Array res = node::Array::New(env, vec.size());
    for (size_t i = 0; i < vec.size(); i++) res[i] = func(vec[i]);
    return res;
}

#define AST_WRAPPER(clsName, attrCode, insCode)                                                                                                                                                        \
    class clsName##Wrapper : public node::ObjectWrap<clsName##Wrapper> {                                                                                                                               \
    public:                                                                                                                                                                                            \
        explicit clsName##Wrapper(const node::CallbackInfo& info);                                                                                                                                     \
        static node::Object Init(node::Env env, node::Object exports);                                                                                                                                 \
        static node::Object trans(const node::Env& env, const std::shared_ptr<ast::clsName>& cls);                                                                                                     \
        static std::shared_ptr<ast::clsName> trans(const node::Object& cls);                                                                                                                           \
                                                                                                                                                                                                       \
    private:                                                                                                                                                                                           \
        static node::FunctionReference constructor;                                                                                                                                                    \
        insCode attrCode node::Value toString(const node::CallbackInfo& info);                                                                                                                         \
        node::Value toJson(const node::CallbackInfo& info);                                                                                                                                            \
    };

#define AST_WRAP_ABC(clsName) AST_WRAPPER(clsName, , )

#define AST_WRAP_SPEC(clsName, attrCode) AST_WRAPPER(clsName, attrCode, std::shared_ptr<ast::clsName> instance;)

#define AST_WRAP_ATTR(attrName)                                                                                                                                                                        \
    node::Value attrName##Get(const node::CallbackInfo& info);                                                                                                                                         \
    void attrName##Set(const node::CallbackInfo& info, const node::Value& value);

namespace astNodeApi {
    extern bool DEBUG;

    AST_WRAP_SPEC(Program, AST_WRAP_ATTR(statements))

    AST_WRAP_SPEC(Member, AST_WRAP_ATTR(identifier) AST_WRAP_ATTR(expression))

    AST_WRAP_SPEC(Parameter, AST_WRAP_ATTR(identifier) AST_WRAP_ATTR(type) AST_WRAP_ATTR(expression))

    AST_WRAP_ABC(Statement)

    AST_WRAP_SPEC(Export, AST_WRAP_ATTR(statement))

    AST_WRAP_SPEC(MapDef, AST_WRAP_ATTR(pairs))

    AST_WRAP_SPEC(ObjectDef, AST_WRAP_ATTR(identifier) AST_WRAP_ATTR(type) AST_WRAP_ATTR(members))

    AST_WRAP_SPEC(Assignment, AST_WRAP_ATTR(identifier) AST_WRAP_ATTR(expression))

    AST_WRAP_SPEC(TemplateDef, AST_WRAP_ATTR(identifier) AST_WRAP_ATTR(parameters) AST_WRAP_ATTR(members))

    AST_WRAP_SPEC(Private, AST_WRAP_ATTR(statement))

    AST_WRAP_ABC(Expression)

    AST_WRAP_SPEC(Identifier, AST_WRAP_ATTR(name))

    AST_WRAP_SPEC(EnumRef, AST_WRAP_ATTR(enumName) AST_WRAP_ATTR(enumValue))

    AST_WRAP_SPEC(MapRef, AST_WRAP_ATTR(pairs))

    AST_WRAP_SPEC(ObjectIns, AST_WRAP_ATTR(identifier) AST_WRAP_ATTR(members))

    AST_WRAP_SPEC(ObjectRef, AST_WRAP_ATTR(identifier))

    AST_WRAP_SPEC(Operation, AST_WRAP_ATTR(left) AST_WRAP_ATTR(right) AST_WRAP_ATTR(operator_))

    AST_WRAP_SPEC(Operator, AST_WRAP_ATTR(value))

    AST_WRAP_SPEC(TemplateParam, AST_WRAP_ATTR(identifier))

    AST_WRAP_SPEC(TemplateRef, AST_WRAP_ATTR(identifier))

    AST_WRAP_ABC(Literal)

    AST_WRAP_SPEC(Boolean, AST_WRAP_ATTR(value))

    AST_WRAP_SPEC(Float, AST_WRAP_ATTR(value))

    AST_WRAP_SPEC(Integer, AST_WRAP_ATTR(value))

    AST_WRAP_SPEC(GUID, AST_WRAP_ATTR(value))

    AST_WRAP_SPEC(Nil, AST_WRAP_ATTR(value))

    AST_WRAP_SPEC(Pair, AST_WRAP_ATTR(first) AST_WRAP_ATTR(second))

    AST_WRAP_SPEC(Path, AST_WRAP_ATTR(value))

    AST_WRAP_SPEC(String, AST_WRAP_ATTR(value))

    AST_WRAP_SPEC(Vector, AST_WRAP_ATTR(expressions))

    node::Value parse(const node::CallbackInfo& info);
}


#endif  // NDFNODEAPI_BUILD_H
