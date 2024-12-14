/**
 * @file build.cpp
 * @author edocsitahw
 * @version 1.1
 * @date 2024/12/05 下午2:24
 * @brief
 * */

#include "build.h"
#include <cstring>
#include <ranges>

/** @def CONST_ABC
 * @brief 抽象类构造函数
 * @param clsName 类名
 * */
#define CONST_ABC(clsName)                                                                                                                                                                             \
    clsName##Wrapper::clsName##Wrapper(const node::CallbackInfo& info)                                                                                                                                 \
        : node::ObjectWrap<clsName##Wrapper>(info) {}

/** @def CONST_SPEC
 * @brief 具体类构造函数
 * @param clsName 类名
 * @param attrCode 属性代码
 * */
#define CONST_SPEC(clsName, attrCode)                                                                                                                                                                  \
    clsName##Wrapper::clsName##Wrapper(const node::CallbackInfo& info)                                                                                                                                 \
        : node::ObjectWrap<clsName##Wrapper>(info)                                                                                                                                                     \
        , instance(std::make_shared<ast::clsName>()) {                                                                                                                                                 \
        if (DEBUG) std::cout << "[Constructor] " << #clsName << std::endl;                                                                                                                             \
        info.This().As<node::Object>().Set("nodeName", node::String::New(info.Env(), #clsName));                                                                                                       \
        if (info.Length() == 1 && info[0].IsObject()) {                                                                                                                                                \
            attrCode                                                                                                                                                                                   \
        } else                                                                                                                                                                                         \
            node::Error::New(info.Env(), "Invalid arguments for '" #clsName "' constructor.").ThrowAsJavaScriptException();                                                                            \
    }

/** @def INIT_ABC
 * @brief 抽象类初始化
 * @param clsName 类名
 * */
#define INIT_ABC(clsName)                                                                                                                                                                              \
    node::Object clsName##Wrapper::Init(node::Env env, node::Object exports) {                                                                                                                         \
        node::Function func = DefineClass(env, #clsName, {});                                                                                                                                          \
        constructor         = Persistent(func);                                                                                                                                                        \
        constructor.SuppressDestruct();                                                                                                                                                                \
        env.Global().Set(#clsName, func);                                                                                                                                                              \
        exports.Set(#clsName, func);                                                                                                                                                                   \
        return exports;                                                                                                                                                                                \
    }

/** @def INIT_SPEC
 * @brief 具体类初始化
 * @param clsName 类名
 * @param attrCode 属性代码
 * */
#define INIT_SPEC(clsName, ...)                                                                                                                                                                        \
    node::Object clsName##Wrapper::Init(node::Env env, node::Object exports) {                                                                                                                         \
        node::Function func = DefineClass(                                                                                                                                                             \
            env, #clsName,                                                                                                                                                                             \
            {StaticAccessor(                                                                                                                                                                           \
                 "nodeName", [](const node::CallbackInfo& info) -> node::Value { return node::String::New(info.Env(), #clsName); }, nullptr                                                            \
             ),                                                                                                                                                                                        \
             InstanceMethod("toString", &clsName##Wrapper::toString), InstanceMethod("toJson", &clsName##Wrapper::toJson), __VA_ARGS__}                                                                \
        );                                                                                                                                                                                             \
        constructor = Persistent(func);                                                                                                                                                                \
        constructor.SuppressDestruct();                                                                                                                                                                \
        env.Global().Set(#clsName, func);                                                                                                                                                              \
        exports.Set(#clsName, func);                                                                                                                                                                   \
        return exports;                                                                                                                                                                                \
    }

/** @def ATTR_GET_VALUE
 * @brief 获取属性值
 * @param clsName 类名
 * @param attrName 属性名
 * @param transCode 转换代码
 * */
#define ATTR_GET_VALUE(clsName, attrName, transCode)                                                                                                                                                   \
    node::Value clsName##Wrapper::attrName##Get(const node::CallbackInfo& info) {                                                                                                                      \
        if (DEBUG) std::cout << "[Getter] " << #clsName << "." << #attrName << std::endl;                                                                                                              \
        return transCode;                                                                                                                                                                              \
    }

/** @def ATTR_GET_VALUE_SIMPLE
 * @brief 获取属性值(简易情况)
 * @param clsName 类名
 * @param attrName 属性名
 * @param attrType 属性类型
 * */
#define ATTR_GET_VALUE_SIMPLE(clsName, attrName, attrType) ATTR_GET_VALUE(clsName, attrName, node::attrType::New(info.Env(), instance->attrName))

/** @def ATTR_GET_VALUE_TRANS
 * @brief 获取属性值(转换)
 * @param clsName 类名
 * @param attrName 属性名
 * @param attrType 属性类型
 * */
#define ATTR_GET_VALUE_TRANS(clsName, attrName, attrType) ATTR_GET_VALUE(clsName, attrName, attrType##Wrapper::trans(info.Env(), instance->attrName))

#define ATTR_GET_ARR(clsName, attrName, trans)                                                                                                                                                         \
    node::Value clsName##Wrapper::attrName##Get(const node::CallbackInfo& info) {                                                                                                                      \
        if (DEBUG) std::cout << "[Getter] " << #clsName << "." << #attrName << std::endl;                                                                                                              \
        auto arr = node::Array::New(info.Env(), instance->attrName.size());                                                                                                                            \
        for (int i = 0; i < instance->attrName.size(); i++) arr[i] = trans;                                                                                                                            \
        return arr;                                                                                                                                                                                    \
    }

#define ATTR_GET_ARR_TRANS(clsName, attrName, ofCls) ATTR_GET_ARR(clsName, attrName, ofCls##Wrapper::trans(info.Env(), instance->attrName[i]))

#define ATTR_SET_VALUE(clsName, attrName, transCode)                                                                                                                                                   \
    void clsName##Wrapper::attrName##Set(const node::CallbackInfo& info, const node::Value& value) { instance->attrName = transCode; }

#define ATTR_SET_VALUE_CONST(clsName, attrName, attrType, transCode) ATTR_SET_VALUE(clsName, attrName, value.As<node::attrType>() transCode)

#define ATTR_SET_VALUE_TRANS(clsName, attrName, attrType) ATTR_SET_VALUE(clsName, attrName, attrType##Wrapper::trans(value.As<node::Object>()))

#define ATTR_SET_ARR(clsName, attrName, trans)                                                                                                                                                         \
    void clsName##Wrapper::attrName##Set(const node::CallbackInfo& info, const node::Value& value) {                                                                                                   \
        if (DEBUG) std::cout << "[Setter] " << #clsName << "." << #attrName << std::endl;                                                                                                              \
        for (auto [i, arr] = std::pair<int, node::Array>(0, value.As<node::Array>()); i < arr.Length(); i++) instance->attrName[i] = trans;                                                            \
    }

#define ATTR_SET_ARR_TRANS(clsName, attrName, ofCls) ATTR_SET_ARR(clsName, attrName, ofCls##Wrapper::trans(arr.Get(i).As<node::Object>()))

#define ATTR_VALUE(clsName, attrName, attrType) ATTR_GET_VALUE_TRANS(clsName, attrName, attrType) ATTR_SET_VALUE_TRANS(clsName, attrName, attrType)

#define ATTR_ARR(clsName, attrName, attrType) ATTR_GET_ARR_TRANS(clsName, attrName, attrType) ATTR_SET_ARR_TRANS(clsName, attrName, attrType)

#define CONST_ATTR_ARR(clsName, attrName, attrType)                                                                                                                                                    \
    if (auto arg = info[0].As<node::Object>(); arg.Has(#attrName))                                                                                                                                     \
        for (auto [i, arr] = std::pair<int, node::Array>(0, arg.Get(#attrName).As<node::Array>()); i < arr.Length(); i++)                                                                              \
            instance->attrName.push_back(attrType##Wrapper::trans(arr.Get(i).As<node::Object>()));                                                                                                     \
    else                                                                                                                                                                                               \
        node::Error::New(info.Env(), "Missing `" #attrName "` property in '" #clsName "' object.").ThrowAsJavaScriptException();

#define CONST_ATTR(clsName, attrName, transCode)                                                                                                                                                       \
    if (auto arg = info[0].As<node::Object>(); arg.Has(#attrName))                                                                                                                                     \
        instance->attrName = transCode;                                                                                                                                                                \
    else                                                                                                                                                                                               \
        node::Error::New(info.Env(), "Missing `" #attrName "` property in '" #clsName "' object.").ThrowAsJavaScriptException();

#define CONST_ATTR_TRANS(clsName, attrName, attrType) CONST_ATTR(clsName, attrName, attrType##Wrapper::trans(arg.Get(#attrName).As<node::Object>()))

#define INIT_INS_ACC(clsName, attrName) InstanceAccessor(#attrName, &clsName##Wrapper::attrName##Get, &clsName##Wrapper::attrName##Set)

#define TRANS_SPEC_JS(clsName, attrCode)                                                                                                                                                               \
    std::shared_ptr<ast::clsName> clsName##Wrapper::trans(const node::Object& obj) {                                                                                                                   \
        if (DEBUG) std::cout << "[Trans] " << #clsName << " -> JS" << std::endl;                                                                                                                       \
        auto ins = std::make_shared<ast::clsName>();                                                                                                                                                   \
        attrCode return ins;                                                                                                                                                                           \
    }

#define TRANS_SPEC_ATTR_JS(attrName, transCode) ins->attrName = transCode;

#define TRANS_SPEC_ATTR_JS_TRANS(attrName, attrType) TRANS_SPEC_ATTR_JS(attrName, attrType##Wrapper::trans(obj.Get(#attrName).As<node::Object>()))

#define TRANS_SPEC_ATTR_JS_ARR(attrName, transCode)                                                                                                                                                    \
    for (auto [i, arr] = std::pair<int, node::Array>(0, obj.Get(#attrName).As<node::Array>()); i < arr.Length(); i++) ins->attrName.push_back(transCode);

#define TRANS_SPEC_ATTR_JS_ARR_TRANS(attrName, attrType) TRANS_SPEC_ATTR_JS_ARR(attrName, attrType##Wrapper::trans(arr.Get(i).As<node::Object>()))

#define TRANS_ABC_JS(clsName, attrCode)                                                                                                                                                                \
    std::shared_ptr<ast::clsName> clsName##Wrapper::trans(const node::Object& obj) {                                                                                                                   \
        if (DEBUG) std::cout << "[Trans] " << #clsName << " -> JS" << std::endl;                                                                                                                       \
        auto name = obj.Get("nodeName").ToString().Utf8Value();                                                                                                                                        \
        attrCode return nullptr;                                                                                                                                                                       \
    }

#define TRANS_ABC_ATTR_JS(type)                                                                                                                                                                        \
    if (name == #type) return type##Wrapper::trans(obj);

#define TRANS_ABC_ATTR_JS_END(clsName) node::TypeError::New(obj.Env(), #clsName "Wrapper::trans: Invalid object type '" + name + "'!").ThrowAsJavaScriptException();

#define TRANS_SPEC_CPP(clsName, attrCode)                                                                                                                                                              \
    node::Object clsName##Wrapper::trans(const node::Env& env, const std::shared_ptr<ast::clsName>& obj) {                                                                                             \
        if (DEBUG) std::cout << "[Trans] " << #clsName << " -> CPP" << std::endl;                                                                                                                      \
        auto arg = node::Object::New(env);                                                                                                                                                             \
        attrCode return getConstructor(env, #clsName).New({arg}).As<node::Object>();                                                                                                                   \
    }

#define TRANS_SPEC_ATTR_CPP(attrName, transCode) arg.Set(#attrName, transCode);

#define TRANS_SPEC_ATTR_CPP_TRANS(attrName, attrType) TRANS_SPEC_ATTR_CPP(attrName, attrType##Wrapper::trans(env, obj->attrName))

#define TRANS_SPEC_ATTR_CPP_ARR(attrName, attrType) arg.Set(#attrName, arrConvert(env, obj->attrName, [&env](const auto& item) -> node::Value { return attrType##Wrapper::trans(env, item); }));

#define TRANS_ABC_CPP(clsName, attrCode)                                                                                                                                                               \
    node::Object clsName##Wrapper::trans(const node::Env& env, const std::shared_ptr<ast::clsName>& obj) {                                                                                             \
        if (DEBUG) std::cout << "[Trans] " << #clsName << " -> CPP" << std::endl;                                                                                                                      \
        attrCode                                                                                                                                                                                       \
    }

#define TRANS_ABC_ATTR_CPP(type)                                                                                                                                                                       \
    if (obj->getNodeName() == #type) return type##Wrapper::trans(env, std::static_pointer_cast<ast::type>(obj));

#define TRANS_ABC_ATTR_CPP_END(clsName) node::TypeError::New(env, #clsName "Wrapper::trans: Invalid object type '" + std::string(obj->nodeName) + "'!").ThrowAsJavaScriptException();

#define TO_STRING(clsName)                                                                                                                                                                             \
    node::Value clsName##Wrapper::toString(const node::CallbackInfo& info) { return node::String::New(info.Env(), instance->toString()); }

#define TO_JSON(clsName)                                                                                                                                                                               \
    node::Value clsName##Wrapper::toJson(const node::CallbackInfo& info) { return node::String::New(info.Env(), instance->toJson()); }

node::Function getConstructor(const node::Env& env, std::string name) {
    if (auto constructor = env.Global().Get(name); constructor.IsFunction()) return constructor.As<node::Function>();
    node::Error::New(env, std::format("Constructor of class {} not found or not export to global scope!", name)).ThrowAsJavaScriptException();
}

namespace astNodeApi {
    std::vector<std::string> literalTypes = {"Boolean", "Float", "Integer", "GUID", "Nil", "Pair", "Path", "String", "Vector"};
    bool DEBUG                            = false;

    // --------------------------------------- Program ---------------------------------------

    CONST_SPEC(Program, CONST_ATTR_ARR(Program, statements, Statement))
    INIT_SPEC(Program, INIT_INS_ACC(Program, statements))

    TRANS_SPEC_CPP(Program, TRANS_SPEC_ATTR_CPP_ARR(statements, Statement))
    TRANS_SPEC_JS(Program, TRANS_SPEC_ATTR_JS_ARR_TRANS(statements, Statement))

    ATTR_ARR(Program, statements, Statement)

    TO_STRING(Program)
    TO_JSON(Program)

    // --------------------------------------- Member ---------------------------------------

    CONST_SPEC(Member, CONST_ATTR_TRANS(Member, identifier, Identifier) CONST_ATTR_TRANS(Member, expression, Expression))
    INIT_SPEC(Member, INIT_INS_ACC(Member, identifier), INIT_INS_ACC(Member, expression))

    TRANS_SPEC_CPP(Member, TRANS_SPEC_ATTR_CPP_TRANS(identifier, Identifier) TRANS_SPEC_ATTR_CPP_TRANS(expression, Expression))
    TRANS_SPEC_JS(Member, TRANS_SPEC_ATTR_JS_TRANS(identifier, Identifier) TRANS_SPEC_ATTR_JS_TRANS(expression, Expression))

    ATTR_VALUE(Member, identifier, Identifier)
    ATTR_VALUE(Member, expression, Expression)

    TO_STRING(Member)
    TO_JSON(Member)

    // --------------------------------------- Parameter ---------------------------------------

    CONST_SPEC(Parameter, CONST_ATTR_TRANS(Parameter, identifier, Identifier) CONST_ATTR_TRANS(Parameter, type, Identifier) CONST_ATTR_TRANS(Parameter, expression, Expression))
    INIT_SPEC(Parameter, INIT_INS_ACC(Parameter, identifier), INIT_INS_ACC(Parameter, type), INIT_INS_ACC(Parameter, expression))

    TRANS_SPEC_CPP(Parameter, TRANS_SPEC_ATTR_CPP_TRANS(identifier, Identifier) TRANS_SPEC_ATTR_CPP_TRANS(type, Identifier) TRANS_SPEC_ATTR_CPP_TRANS(expression, Expression))
    TRANS_SPEC_JS(Parameter, TRANS_SPEC_ATTR_JS_TRANS(identifier, Identifier) TRANS_SPEC_ATTR_JS_TRANS(type, Identifier) TRANS_SPEC_ATTR_JS_TRANS(expression, Expression))

    ATTR_VALUE(Parameter, identifier, Identifier)
    ATTR_VALUE(Parameter, type, Identifier)
    ATTR_VALUE(Parameter, expression, Expression)

    TO_STRING(Parameter)
    TO_JSON(Parameter)

    // --------------------------------------- Statement ---------------------------------------

    CONST_ABC(Statement)
    INIT_ABC(Statement)

    TRANS_ABC_CPP(
        Statement,
        TRANS_ABC_ATTR_CPP(Assignment) else TRANS_ABC_ATTR_CPP(Export) else TRANS_ABC_ATTR_CPP(MapDef) else TRANS_ABC_ATTR_CPP(ObjectDef) else TRANS_ABC_ATTR_CPP(TemplateDef
        ) else TRANS_ABC_ATTR_CPP_END(Statement)
    )

    TRANS_ABC_JS(
        Statement,
        TRANS_ABC_ATTR_JS(Assignment) else TRANS_ABC_ATTR_JS(Export) else TRANS_ABC_ATTR_JS(MapDef) else TRANS_ABC_ATTR_JS(ObjectDef) else TRANS_ABC_ATTR_JS(TemplateDef
        ) else TRANS_ABC_ATTR_JS_END(Statement)
    )

    // --------------------------------------- Export ---------------------------------------

    CONST_SPEC(Export, CONST_ATTR_TRANS(Export, statement, Statement))
    INIT_SPEC(Export, INIT_INS_ACC(Export, statement))

    TRANS_SPEC_CPP(Export, TRANS_SPEC_ATTR_CPP_TRANS(statement, Statement))
    TRANS_SPEC_JS(Export, TRANS_SPEC_ATTR_JS_TRANS(statement, Statement))

    ATTR_VALUE(Export, statement, Statement)

    TO_STRING(Export)
    TO_JSON(Export)

    // --------------------------------------- MapDef ---------------------------------------

    CONST_SPEC(MapDef, CONST_ATTR_ARR(MapDef, pairs, Pair))
    INIT_SPEC(MapDef, INIT_INS_ACC(MapDef, pairs))

    TRANS_SPEC_CPP(MapDef, TRANS_SPEC_ATTR_CPP_ARR(pairs, Pair))
    TRANS_SPEC_JS(MapDef, TRANS_SPEC_ATTR_JS_ARR_TRANS(pairs, Pair))

    ATTR_ARR(MapDef, pairs, Pair)

    TO_STRING(MapDef)
    TO_JSON(MapDef)

    // --------------------------------------- ObjectDef ---------------------------------------

    CONST_SPEC(ObjectDef, CONST_ATTR_TRANS(ObjectDef, identifier, Identifier) CONST_ATTR_ARR(ObjectDef, members, Member))
    INIT_SPEC(ObjectDef, INIT_INS_ACC(ObjectDef, identifier), INIT_INS_ACC(ObjectDef, members))

    TRANS_SPEC_CPP(ObjectDef, TRANS_SPEC_ATTR_CPP_TRANS(identifier, Identifier) TRANS_SPEC_ATTR_CPP_ARR(members, Member))
    TRANS_SPEC_JS(ObjectDef, TRANS_SPEC_ATTR_JS_TRANS(identifier, Identifier) TRANS_SPEC_ATTR_JS_ARR_TRANS(members, Member))

    ATTR_VALUE(ObjectDef, identifier, Identifier)
    ATTR_ARR(ObjectDef, members, Member)

    TO_STRING(ObjectDef)
    TO_JSON(ObjectDef)

    // --------------------------------------- Assignment ----------

    CONST_SPEC(Assignment, CONST_ATTR_TRANS(Assignment, identifier, Identifier) CONST_ATTR_TRANS(Assignment, expression, Expression))
    INIT_SPEC(Assignment, INIT_INS_ACC(Assignment, identifier), INIT_INS_ACC(Assignment, expression))

    TRANS_SPEC_CPP(Assignment, TRANS_SPEC_ATTR_CPP_TRANS(identifier, Identifier) TRANS_SPEC_ATTR_CPP_TRANS(expression, Expression))
    TRANS_SPEC_JS(Assignment, TRANS_SPEC_ATTR_JS_TRANS(identifier, Identifier) TRANS_SPEC_ATTR_JS_TRANS(expression, Expression))

    ATTR_VALUE(Assignment, identifier, Identifier)
    ATTR_VALUE(Assignment, expression, Expression)

    TO_STRING(Assignment)
    TO_JSON(Assignment)

    // --------------------------------------- TemplateDef ---------------------------------------

    CONST_SPEC(TemplateDef, CONST_ATTR_TRANS(TemplateDef, identifier, Identifier) CONST_ATTR_ARR(TemplateDef, parameters, Parameter) CONST_ATTR_ARR(TemplateDef, members, Member))
    INIT_SPEC(TemplateDef, INIT_INS_ACC(TemplateDef, identifier), INIT_INS_ACC(TemplateDef, parameters), INIT_INS_ACC(TemplateDef, members))

    TRANS_SPEC_CPP(TemplateDef, TRANS_SPEC_ATTR_CPP_TRANS(identifier, Identifier) TRANS_SPEC_ATTR_CPP_ARR(parameters, Parameter) TRANS_SPEC_ATTR_CPP_ARR(members, Member))
    TRANS_SPEC_JS(TemplateDef, TRANS_SPEC_ATTR_JS_TRANS(identifier, Identifier) TRANS_SPEC_ATTR_JS_ARR_TRANS(parameters, Parameter) TRANS_SPEC_ATTR_JS_ARR_TRANS(members, Member))

    ATTR_VALUE(TemplateDef, identifier, Identifier)
    ATTR_ARR(TemplateDef, parameters, Parameter)
    ATTR_ARR(TemplateDef, members, Member)

    TO_STRING(TemplateDef)
    TO_JSON(TemplateDef)

    // --------------------------------------- Private ---------------------------------------

    CONST_SPEC(Private, CONST_ATTR_TRANS(Private, statement, Statement))
    INIT_SPEC(Private, INIT_INS_ACC(Private, statement))

    TRANS_SPEC_CPP(Private, TRANS_SPEC_ATTR_CPP_TRANS(statement, Statement))
    TRANS_SPEC_JS(Private, TRANS_SPEC_ATTR_JS_TRANS(statement, Statement))

    ATTR_VALUE(Private, statement, Statement)

    TO_STRING(Private)
    TO_JSON(Private)

    // --------------------------------------- Expression ---------------------------------------

    CONST_ABC(Expression)
    INIT_ABC(Expression)

    TRANS_ABC_CPP(Expression,
                  TRANS_ABC_ATTR_CPP(EnumRef) else TRANS_ABC_ATTR_CPP(Identifier
                  ) else if (std::ranges::find(literalTypes, obj->getNodeName()) != literalTypes.end()) return LiteralWrapper::trans(env, std::static_pointer_cast<ast::Literal>(obj));
                  else TRANS_ABC_ATTR_CPP(MapRef) else TRANS_ABC_ATTR_CPP(ObjectIns) else TRANS_ABC_ATTR_CPP(ObjectRef) else TRANS_ABC_ATTR_CPP(Operation) else TRANS_ABC_ATTR_CPP(Operator
                  ) else TRANS_ABC_ATTR_CPP(TemplateParam) else TRANS_ABC_ATTR_CPP(TemplateRef) else TRANS_ABC_ATTR_CPP_END(Expression))

    TRANS_ABC_JS(Expression, TRANS_ABC_ATTR_JS(EnumRef) else TRANS_ABC_ATTR_JS(Identifier) else if (std::ranges::find(literalTypes, name) != literalTypes.end()) return LiteralWrapper::trans(obj);
                 else TRANS_ABC_ATTR_JS(MapRef) else TRANS_ABC_ATTR_JS(ObjectIns) else TRANS_ABC_ATTR_JS(ObjectRef) else TRANS_ABC_ATTR_JS(Operation) else TRANS_ABC_ATTR_JS(Operator
                 ) else TRANS_ABC_ATTR_JS(TemplateParam) else TRANS_ABC_ATTR_JS(TemplateRef) else TRANS_ABC_ATTR_JS_END(Expression))

    // --------------------------------------- Identifier ---------------------------------------

    CONST_SPEC(Identifier, CONST_ATTR(Identifier, name, arg.Get("name").ToString().Utf8Value()))
    INIT_SPEC(Identifier, INIT_INS_ACC(Identifier, name))

    TRANS_SPEC_CPP(Identifier, TRANS_SPEC_ATTR_CPP(name, node::String::New(env, obj->name)))
    TRANS_SPEC_JS(Identifier, TRANS_SPEC_ATTR_JS(name, obj.Get("name").ToString().Utf8Value()))

    ATTR_GET_VALUE_SIMPLE(Identifier, name, String)
    ATTR_SET_VALUE_CONST(Identifier, name, String, .Utf8Value())

    TO_STRING(Identifier)
    TO_JSON(Identifier)

    // --------------------------------------- EnumRef ---------------------------------------

    CONST_SPEC(EnumRef, CONST_ATTR_TRANS(EnumRef, enumName, Identifier) CONST_ATTR_TRANS(EnumRef, enumValue, Identifier))
    INIT_SPEC(EnumRef, INIT_INS_ACC(EnumRef, enumName), INIT_INS_ACC(EnumRef, enumValue))

    TRANS_SPEC_CPP(EnumRef, TRANS_SPEC_ATTR_CPP_TRANS(enumName, Identifier) TRANS_SPEC_ATTR_CPP_TRANS(enumValue, Identifier))
    TRANS_SPEC_JS(EnumRef, TRANS_SPEC_ATTR_JS_TRANS(enumName, Identifier) TRANS_SPEC_ATTR_JS_TRANS(enumValue, Identifier))

    ATTR_VALUE(EnumRef, enumName, Identifier)
    ATTR_VALUE(EnumRef, enumValue, Identifier)

    TO_STRING(EnumRef)
    TO_JSON(EnumRef)

    // --------------------------------------- MapRef ---------------------------------------

    CONST_SPEC(MapRef, CONST_ATTR_ARR(MapRef, pairs, Pair))
    INIT_SPEC(MapRef, INIT_INS_ACC(MapRef, pairs))

    TRANS_SPEC_CPP(MapRef, TRANS_SPEC_ATTR_CPP_ARR(pairs, Pair))
    TRANS_SPEC_JS(MapRef, TRANS_SPEC_ATTR_JS_ARR_TRANS(pairs, Pair))

    ATTR_ARR(MapRef, pairs, Pair)

    TO_STRING(MapRef)
    TO_JSON(MapRef)

    // --------------------------------------- ObjectIns ----------

    CONST_SPEC(ObjectIns, CONST_ATTR_TRANS(ObjectIns, identifier, Identifier) CONST_ATTR_ARR(ObjectIns, members, Member))
    INIT_SPEC(ObjectIns, INIT_INS_ACC(ObjectIns, identifier), INIT_INS_ACC(ObjectIns, members))

    TRANS_SPEC_CPP(ObjectIns, TRANS_SPEC_ATTR_CPP_TRANS(identifier, Identifier) TRANS_SPEC_ATTR_CPP_ARR(members, Member))
    TRANS_SPEC_JS(ObjectIns, TRANS_SPEC_ATTR_JS_TRANS(identifier, Identifier) TRANS_SPEC_ATTR_JS_ARR_TRANS(members, Member))

    ATTR_VALUE(ObjectIns, identifier, Identifier)
    ATTR_ARR(ObjectIns, members, Member)

    TO_STRING(ObjectIns)
    TO_JSON(ObjectIns)

    // --------------------------------------- ObjectRef ---------------------------------------

    CONST_SPEC(ObjectRef, CONST_ATTR_TRANS(ObjectRef, identifier, Identifier))
    INIT_SPEC(ObjectRef, INIT_INS_ACC(ObjectRef, identifier))

    TRANS_SPEC_CPP(ObjectRef, TRANS_SPEC_ATTR_CPP_TRANS(identifier, Identifier))
    TRANS_SPEC_JS(ObjectRef, TRANS_SPEC_ATTR_JS_TRANS(identifier, Identifier))

    ATTR_VALUE(ObjectRef, identifier, Identifier)

    TO_STRING(ObjectRef)
    TO_JSON(ObjectRef)

    // --------------------------------------- Operation ---------------------------------------

    CONST_SPEC(Operation, CONST_ATTR_TRANS(Operation, left, Expression) CONST_ATTR_TRANS(Operation, right, Expression) CONST_ATTR_TRANS(Operation, operator_, Operator))
    INIT_SPEC(Operation, INIT_INS_ACC(Operation, left), INIT_INS_ACC(Operation, right), INIT_INS_ACC(Operation, operator_))

    TRANS_SPEC_CPP(Operation, TRANS_SPEC_ATTR_CPP_TRANS(left, Expression) TRANS_SPEC_ATTR_CPP_TRANS(right, Expression) TRANS_SPEC_ATTR_CPP_TRANS(operator_, Operator))
    TRANS_SPEC_JS(Operation, TRANS_SPEC_ATTR_JS_TRANS(left, Expression) TRANS_SPEC_ATTR_JS_TRANS(right, Expression) TRANS_SPEC_ATTR_JS_TRANS(operator_, Operator))

    ATTR_VALUE(Operation, left, Expression)
    ATTR_VALUE(Operation, right, Expression)
    ATTR_VALUE(Operation, operator_, Operator)

    TO_STRING(Operation)
    TO_JSON(Operation)

    // --------------------------------------- Operator ---------------------------------------

    CONST_SPEC(Operator, CONST_ATTR(Operator, value, arg.Get("value").ToString().Utf8Value()))
    INIT_SPEC(Operator, INIT_INS_ACC(Operator, value))

    TRANS_SPEC_CPP(Operator, TRANS_SPEC_ATTR_CPP(value, node::String::New(env, obj->value)))
    TRANS_SPEC_JS(Operator, TRANS_SPEC_ATTR_JS(value, obj.Get("value").ToString().Utf8Value()))

    ATTR_GET_VALUE_SIMPLE(Operator, value, String)
    ATTR_SET_VALUE_CONST(Operator, value, String, .Utf8Value())

    TO_STRING(Operator)
    TO_JSON(Operator)

    // --------------------------------------- TemplateParam ---------------------------------------

    CONST_SPEC(TemplateParam, CONST_ATTR_TRANS(TemplateParam, identifier, Identifier))
    INIT_SPEC(TemplateParam, INIT_INS_ACC(TemplateParam, identifier))

    TRANS_SPEC_CPP(TemplateParam, TRANS_SPEC_ATTR_CPP_TRANS(identifier, Identifier))
    TRANS_SPEC_JS(TemplateParam, TRANS_SPEC_ATTR_JS_TRANS(identifier, Identifier))

    ATTR_VALUE(TemplateParam, identifier, Identifier)

    TO_STRING(TemplateParam)
    TO_JSON(TemplateParam)

    // --------------------------------------- TemplateRef ---------------------------------------

    CONST_SPEC(TemplateRef, CONST_ATTR_TRANS(TemplateRef, identifier, Identifier))
    INIT_SPEC(TemplateRef, INIT_INS_ACC(TemplateRef, identifier))

    TRANS_SPEC_CPP(TemplateRef, TRANS_SPEC_ATTR_CPP_TRANS(identifier, Identifier))
    TRANS_SPEC_JS(TemplateRef, TRANS_SPEC_ATTR_JS_TRANS(identifier, Identifier))

    ATTR_VALUE(TemplateRef, identifier, Identifier)

    TO_STRING(TemplateRef)
    TO_JSON(TemplateRef)

    // --------------------------------------- Literal ---------------------------------------

    CONST_ABC(Literal)
    INIT_ABC(Literal)

    TRANS_ABC_CPP(
        Literal,
        TRANS_ABC_ATTR_CPP(Boolean) else TRANS_ABC_ATTR_CPP(Float) else TRANS_ABC_ATTR_CPP(Integer) else TRANS_ABC_ATTR_CPP(GUID) else TRANS_ABC_ATTR_CPP(Nil) else TRANS_ABC_ATTR_CPP(Pair
        ) else TRANS_ABC_ATTR_CPP(Path) else TRANS_ABC_ATTR_CPP(String) else TRANS_ABC_ATTR_CPP(Vector) else TRANS_ABC_ATTR_CPP_END(Literal)
    )
    TRANS_ABC_JS(
        Literal,
        TRANS_ABC_ATTR_JS(Boolean) else TRANS_ABC_ATTR_JS(Float) else TRANS_ABC_ATTR_JS(Integer) else TRANS_ABC_ATTR_JS(GUID) else TRANS_ABC_ATTR_JS(Nil) else TRANS_ABC_ATTR_JS(Pair
        ) else TRANS_ABC_ATTR_JS(Path) else TRANS_ABC_ATTR_JS(String) else TRANS_ABC_ATTR_JS(Vector) else TRANS_ABC_ATTR_JS_END(Literal)
    )

    // --------------------------------------- Boolean ---------------------------------------

    CONST_SPEC(Boolean, CONST_ATTR(Boolean, value, arg.Get("value").ToBoolean()))
    INIT_SPEC(Boolean, INIT_INS_ACC(Boolean, value))

    TRANS_SPEC_CPP(Boolean, TRANS_SPEC_ATTR_CPP(value, node::Boolean::New(env, obj->value)))
    TRANS_SPEC_JS(Boolean, TRANS_SPEC_ATTR_JS(value, obj.Get("value").ToBoolean()))

    ATTR_GET_VALUE_SIMPLE(Boolean, value, Boolean)
    ATTR_SET_VALUE_CONST(Boolean, value, Boolean, )

    TO_STRING(Boolean)
    TO_JSON(Boolean)

    // --------------------------------------- Float ---------------------------------------

    CONST_SPEC(Float, CONST_ATTR(Float, value, arg.Get("value").ToNumber()))
    INIT_SPEC(Float, INIT_INS_ACC(Float, value))

    TRANS_SPEC_CPP(Float, TRANS_SPEC_ATTR_CPP(value, node::Number::New(env, obj->value)))
    TRANS_SPEC_JS(Float, TRANS_SPEC_ATTR_JS(value, obj.Get("value").ToNumber()))

    ATTR_GET_VALUE_SIMPLE(Float, value, Number)
    ATTR_SET_VALUE_CONST(Float, value, Number, )

    TO_STRING(Float)
    TO_JSON(Float)

    // --------------------------------------- Integer ---------------------------------------

    CONST_SPEC(Integer, CONST_ATTR(Integer, value, arg.Get("value").ToNumber()))
    INIT_SPEC(Integer, INIT_INS_ACC(Integer, value))

    TRANS_SPEC_CPP(Integer, TRANS_SPEC_ATTR_CPP(value, node::Number::New(env, obj->value)))
    TRANS_SPEC_JS(Integer, TRANS_SPEC_ATTR_JS(value, obj.Get("value").ToNumber()))

    ATTR_GET_VALUE_SIMPLE(Integer, value, Number)
    ATTR_SET_VALUE_CONST(Integer, value, Number, )

    TO_STRING(Integer)
    TO_JSON(Integer)

    // --------------------------------------- GUID ---------------------------------------

    CONST_SPEC(GUID, CONST_ATTR(GUID, value, arg.Get("value").ToString().Utf8Value()))
    INIT_SPEC(GUID, INIT_INS_ACC(GUID, value))

    TRANS_SPEC_CPP(GUID, TRANS_SPEC_ATTR_CPP(value, node::String::New(env, obj->value)))
    TRANS_SPEC_JS(GUID, TRANS_SPEC_ATTR_JS(value, obj.Get("value").ToString().Utf8Value()))

    ATTR_GET_VALUE_SIMPLE(GUID, value, String)
    ATTR_SET_VALUE_CONST(GUID, value, String, .Utf8Value())

    TO_STRING(GUID)
    TO_JSON(GUID)

    // --------------------------------------- Nil ---------------------------------------

    CONST_SPEC(Nil, CONST_ATTR(Nil, value, arg.Get("value").ToString().Utf8Value()))
    INIT_SPEC(Nil, INIT_INS_ACC(Nil, value))

    TRANS_SPEC_CPP(Nil, TRANS_SPEC_ATTR_CPP(value, node::String::New(env, obj->value)))
    TRANS_SPEC_JS(Nil, TRANS_SPEC_ATTR_JS(value, obj.Get("value").ToString().Utf8Value()))

    ATTR_GET_VALUE_SIMPLE(Nil, value, String)
    ATTR_SET_VALUE_CONST(Nil, value, String, .Utf8Value())

    TO_STRING(Nil)
    TO_JSON(Nil)

    // --------------------------------------- Pair ---------------------------------------

    CONST_SPEC(Pair, CONST_ATTR_TRANS(Pair, first, Expression) CONST_ATTR_TRANS(Pair, second, Expression))
    INIT_SPEC(Pair, INIT_INS_ACC(Pair, first), INIT_INS_ACC(Pair, second))

    TRANS_SPEC_CPP(Pair, TRANS_SPEC_ATTR_CPP_TRANS(first, Expression) TRANS_SPEC_ATTR_CPP_TRANS(second, Expression))
    TRANS_SPEC_JS(Pair, TRANS_SPEC_ATTR_JS_TRANS(first, Expression) TRANS_SPEC_ATTR_JS_TRANS(second, Expression))

    ATTR_VALUE(Pair, first, Expression)
    ATTR_VALUE(Pair, second, Expression)

    TO_STRING(Pair)
    TO_JSON(Pair)

    // --------------------------------------- Path ---------------------------------------

    CONST_SPEC(Path, CONST_ATTR(Path, value, arg.Get("value").ToString().Utf8Value()))
    INIT_SPEC(Path, INIT_INS_ACC(Path, value))

    TRANS_SPEC_CPP(Path, TRANS_SPEC_ATTR_CPP(value, node::String::New(env, obj->value)))
    TRANS_SPEC_JS(Path, TRANS_SPEC_ATTR_JS(value, obj.Get("value").ToString().Utf8Value()))

    ATTR_GET_VALUE_SIMPLE(Path, value, String)
    ATTR_SET_VALUE_CONST(Path, value, String, .Utf8Value())

    TO_STRING(Path)
    TO_JSON(Path)

    // --------------------------------------- String ---------------------------------------

    CONST_SPEC(String, CONST_ATTR(String, value, arg.Get("value").ToString().Utf8Value()))
    INIT_SPEC(String, INIT_INS_ACC(String, value))

    TRANS_SPEC_CPP(String, TRANS_SPEC_ATTR_CPP(value, node::String::New(env, obj->value)))
    TRANS_SPEC_JS(String, TRANS_SPEC_ATTR_JS(value, obj.Get("value").ToString().Utf8Value()))

    ATTR_GET_VALUE_SIMPLE(String, value, String)
    ATTR_SET_VALUE_CONST(String, value, String, .Utf8Value())

    TO_STRING(String)
    TO_JSON(String)

    // --------------------------------------- Vector ----------

    CONST_SPEC(Vector, CONST_ATTR_ARR(Vector, expressions, Expression))
    INIT_SPEC(Vector, INIT_INS_ACC(Vector, expressions))

    TRANS_SPEC_CPP(Vector, TRANS_SPEC_ATTR_CPP_ARR(expressions, Expression))
    TRANS_SPEC_JS(Vector, TRANS_SPEC_ATTR_JS_ARR_TRANS(expressions, Expression))

    ATTR_ARR(Vector, expressions, Expression)

    TO_STRING(Vector)
    TO_JSON(Vector)

    node::FunctionReference ProgramWrapper::constructor;
    node::FunctionReference MemberWrapper::constructor;
    node::FunctionReference ParameterWrapper::constructor;
    node::FunctionReference StatementWrapper::constructor;
    node::FunctionReference ExportWrapper::constructor;
    node::FunctionReference MapDefWrapper::constructor;
    node::FunctionReference ObjectDefWrapper::constructor;
    node::FunctionReference AssignmentWrapper::constructor;
    node::FunctionReference TemplateDefWrapper::constructor;
    node::FunctionReference PrivateWrapper::constructor;
    node::FunctionReference ExpressionWrapper::constructor;
    node::FunctionReference IdentifierWrapper::constructor;
    node::FunctionReference EnumRefWrapper::constructor;
    node::FunctionReference MapRefWrapper::constructor;
    node::FunctionReference ObjectInsWrapper::constructor;
    node::FunctionReference ObjectRefWrapper::constructor;
    node::FunctionReference OperationWrapper::constructor;
    node::FunctionReference OperatorWrapper::constructor;
    node::FunctionReference TemplateParamWrapper::constructor;
    node::FunctionReference TemplateRefWrapper::constructor;
    node::FunctionReference LiteralWrapper::constructor;
    node::FunctionReference BooleanWrapper::constructor;
    node::FunctionReference FloatWrapper::constructor;
    node::FunctionReference IntegerWrapper::constructor;
    node::FunctionReference GUIDWrapper::constructor;
    node::FunctionReference NilWrapper::constructor;
    node::FunctionReference PairWrapper::constructor;
    node::FunctionReference PathWrapper::constructor;
    node::FunctionReference StringWrapper::constructor;
    node::FunctionReference VectorWrapper::constructor;

    node::Value parse(const node::CallbackInfo& info) {
        if (info.Length() >= 1 && info[0].IsString()) {
            auto tokens = TokenProcessor::process(Lexer::tokenize(info[0].ToString().Utf8Value()));
            auto parser = Parser(tokens);

            if (info.Length() == 2 && info[1].IsBoolean())
                DEBUG = info[1].ToBoolean().Value();

            return ProgramWrapper::trans(info.Env(), parser.parse());
        }
        node::TypeError::New(info.Env(), "Invalid argument").ThrowAsJavaScriptException();
        return info.Env().Undefined();
    }
}  // namespace astNodeApi



node::Object Init(Napi::Env env, Napi::Object exports) {
    using namespace astNodeApi;
    ProgramWrapper::Init(env, exports);
    MemberWrapper::Init(env, exports);
    ParameterWrapper::Init(env, exports);
    StatementWrapper::Init(env, exports);
    ExportWrapper::Init(env, exports);
    MapDefWrapper::Init(env, exports);
    ObjectDefWrapper::Init(env, exports);
    AssignmentWrapper::Init(env, exports);
    TemplateDefWrapper::Init(env, exports);
    PrivateWrapper::Init(env, exports);
    ExpressionWrapper::Init(env, exports);
    IdentifierWrapper::Init(env, exports);
    EnumRefWrapper::Init(env, exports);
    MapRefWrapper::Init(env, exports);
    ObjectInsWrapper::Init(env, exports);
    ObjectRefWrapper::Init(env, exports);
    OperationWrapper::Init(env, exports);
    OperatorWrapper::Init(env, exports);
    TemplateParamWrapper::Init(env, exports);
    TemplateRefWrapper::Init(env, exports);
    LiteralWrapper::Init(env, exports);
    BooleanWrapper::Init(env, exports);
    FloatWrapper::Init(env, exports);
    IntegerWrapper::Init(env, exports);
    GUIDWrapper::Init(env, exports);
    NilWrapper::Init(env, exports);
    PairWrapper::Init(env, exports);
    PathWrapper::Init(env, exports);
    StringWrapper::Init(env, exports);
    VectorWrapper::Init(env, exports);

    exports.Set("parse", Napi::Function::New(env, parse));

    return exports;
}

NODE_API_MODULE(ndfNodeAPI, Init)
