/**
 * @file build.cpp
 * @author edocsitahw
 * @version 1.1
 * @date 2024/12/05 下午2:24
 * @brief
 * */

#include "../src/lexer/lexer.h"
#include "../src/lexer/tokenProc.h"
#include "../src/parser/ast.h"
#include "../src/parser/parser.h"
#include "../src/expection.h"
#include <map>
#include "napi.h"

namespace node = Napi;

template<class C>
void createWithAttr(const std::string &attrs, const node::Object &obj) {
    if (attrs.empty()) throw ArgumentError("Invalid attribute string");

    if (attrs[0] != '{' || attrs[attrs.size() - 1] != '}') throw SyntaxError("attribute string must be enclosed in curly braces");

    auto attrMap = std::map<std::string, std::string>();

    std::string attrName;
    std::string attrType;
    bool isType = false;
    for (auto i = 1; i <= attrs.size() - 1; i++) {

        if (attrs[i] == ',' || attrs[i] == '}') {
            attrMap[attrName] = attrType;
            isType = false;
            attrName.clear();
            attrType.clear();
        }
        else if (attrs[i] == ':') isType = true;
        else if (attrs[i] ==' ') continue;
        else {
            if (isType) attrType += attrs[i];
            else attrName += attrs[i];
        }
    }

    for (auto [name, value] : obj.GetPropertyNames())
        if (attrMap.find(name.ToString()) == attrMap.end()) throw KeyError(std::format("Invalid attribute name: {}", name.ToString().Utf8Value()));

    for (const auto& [name, value] : attrMap)
        if (!obj.HasOwnProperty(name)) throw ArgumentError(std::format("Missing attribute: {}", name));


}

#define DECLARE_AST_CLASS_ABC(name)                                                                                                                                                                    \
    class name##Wrapper : public node::ObjectWrap<name##Wrapper> {                                                                                                                                     \
    private:                                                                                                                                                                                           \
        static node::FunctionReference constructor;                                                                                                                                                    \
                                                                                                                                                                                                       \
    public:                                                                                                                                                                                            \
        explicit name##Wrapper(const node::CallbackInfo &info)                                                                                                                                         \
            : node::ObjectWrap<name##Wrapper>(info) {}                                                                                                                                                 \
        static node::Object Init(node::Env env, node::Object exports) {                                                                                                                                \
            node::Function func = DefineClass(env, #name, {});                                                                                                                                         \
            constructor         = node::Persistent(func);                                                                                                                                              \
            constructor.SuppressDestruct();                                                                                                                                                            \
            exports.Set(#name, func);                                                                                                                                                                  \
            return exports;                                                                                                                                                                            \
        }                                                                                                                                                                                              \
    };

#define DECLARE_AST_CLASS(name, ...)                                                                                                                                                                   \
    class name##Wrapper : public node::ObjectWrap<name##Wrapper> {                                                                                                                                     \
    private:                                                                                                                                                                                           \
        static node::FunctionReference constructor;                                                                                                                                                    \
        name *instance;                                                                                                                                                                                \
                                                                                                                                                                                                       \
    public:                                                                                                                                                                                            \
        explicit name##Wrapper(const node::CallbackInfo &info)                                                                                                                                         \
            : node::ObjectWrap<name##Wrapper>(info) {                                                                                                                                                  \
            if (info.Length() == 1 && info[0].IsExternal())                                                                                                                                            \
                if (info[0].IsObject()) {                                                                                                                                                              \
                } else                                                                                                                                                                                 \
                    throw node::Error::New(info.Env(), "Invalid external object").ThrowAsJavaScriptException();                                                                                                                                                                       \
        }
}

DECLARE_AST_CLASS_ABC(AST)

DECLARE_AST_CLASS(Program)
