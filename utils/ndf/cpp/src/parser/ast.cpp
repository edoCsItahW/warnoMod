// Copyright (c) 2024. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file ast.cpp
 * @author edocsitahw
 * @version 1.1
 * @date 2024/11/25 下午3:47
 * @brief
 * */
#include "ast.h"
#include <format>
#include <iostream>  // MSCV
#include <sstream>

#define GET_NODE_NAME(cls)                                                                                                                                                                             \
    std::string cls::getNodeName() const { return nodeName; }

namespace ast {
    // ----------------------------- Program -----------------------------

    std::string Program::toString() const {
        std::ostringstream oss;

        for (const auto &stmt : statements) oss << stmt->toString() << std::endl;

        return oss.str();
    }

    std::string Program::toJson() const {
        std::ostringstream oss;
        oss << R"({"nodeName":"Program", "statements":[)";
        for (int i = 0; i < statements.size(); i++) oss << statements[i]->toJson() << (i == statements.size() - 1 ? "" : ",");
        oss << "]}";

        return oss.str();
    }

    GET_NODE_NAME(Program)

    // ----------------------------- Member -----------------------------

    std::string Member::toString() const { return identifier->toString() + " = " + expression->toString(); }

    std::string Member::toJson() const {
        std::ostringstream oss;
        oss << R"({"nodeName": "Member", "identifier":)" << identifier->toJson() << "," << R"("expression": )" << expression->toJson() << "}";
        return oss.str();
    }

    GET_NODE_NAME(Member)

    // ----------------------------- Parameter -----------------------------

    std::string Parameter::toString() const { return identifier->toString() + (type ? ": " + type->toString() : "") + (expression ? " = " + expression->toString() : ""); }

    std::string Parameter::toJson() const {
        std::ostringstream oss;
        oss << R"({"nodeName":"Parameter", "identifier":)" << identifier->toJson() << "," << R"("type":)" << (type ? type->toJson() : "null") << "," << R"("expression":)"
            << (expression ? expression->toJson() : "null") << "}";

        return oss.str();
    }

    GET_NODE_NAME(Parameter)

    // ----------------------------- Export -----------------------------

    std::string Export::toString() const { return "export " + statement->toString(); }

    std::string Export::toJson() const { return R"({"nodeName":"Export", "statement":)" + statement->toJson() + "}"; }

    GET_NODE_NAME(Export)

    // ----------------------------- MapDef -----------------------------

    std::string MapDef::toString() const {
        std::ostringstream oss;

        oss << "MAP [" << std::endl;

        for (const auto &pair : pairs) oss << pair->toString() << "," << std::endl;

        oss << "]" << std::endl;

        return oss.str();
    }

    std::string MapDef::toJson() const {
        std::ostringstream oss;
        oss << R"({"nodeName": "MapDef", "pairs":[)";
        for (int i = 0; i < pairs.size(); i++) oss << pairs[i]->toJson() << (i == pairs.size() - 1 ? "" : ",");

        oss << "]}";

        return oss.str();
    }

    GET_NODE_NAME(MapDef)

    // ----------------------------- ObjectDef -----------------------------

    std::string ObjectDef::toString() const {
        std::ostringstream oss;

        oss << identifier->toString() << " is " << type->toString() << "(" << std::endl;

        for (int i = 0; i < members.size(); i++) oss << members[i]->toString() << (i == members.size() - 1 ? "" : ",") << std::endl;

        oss << ")" << std::endl;

        return oss.str();
    }

    std::string ObjectDef::toJson() const {
        std::ostringstream oss;
        oss << R"({"nodeName": "ObjectDef", "identifier": )" << identifier->toJson() << "," << R"("type":)" << type->toJson() << "," << R"("members": [)";

        for (int i = 0; i < members.size(); i++) oss << members[i]->toJson() << (i == members.size() - 1 ? "" : ",");

        oss << "]}";

        return oss.str();
    }

    GET_NODE_NAME(ObjectDef)

    // ----------------------------- Assignment -----------------------------

    std::string Assignment::toString() const { return identifier->toString() + " is " + expression->toString() + '\n'; }

    std::string Assignment::toJson() const {
        std::ostringstream oss;
        oss << R"({"nodeName":"Assignment", "identifier":)" << identifier->toJson() << R"(, "expression": )" << expression->toJson() << "}";

        return oss.str();
    }

    GET_NODE_NAME(Assignment)

    // ----------------------------- TemplateDef -----------------------------

    std::string TemplateDef::toString() const {
        std::ostringstream oss;

        oss << "template " << identifier->toString() << "[" << std::endl;

        for (int i = 0; i < parameters.size(); i++) oss << parameters[i]->toString() << (i == parameters.size() - 1 ? "" : ",") << std::endl;

        oss << "] is " << type->toString() << "(" << std::endl;

        for (const auto &member : members) oss << member->toString() << std::endl;

        oss << ")" << std::endl;

        return oss.str();
    }

    std::string TemplateDef::toJson() const {
        std::ostringstream oss;
        oss << R"({"nodeName":"TemplateDef", "identifier":)" << identifier->toJson() << "," << R"("parameters":[)";

        for (int i = 0; i < parameters.size(); i++) oss << parameters[i]->toJson() << (i == parameters.size() - 1 ? "" : ",");
        oss << "]," << R"("type":)" << type->toJson() << "," << R"("members":[)";

        for (int i = 0; i < members.size(); i++) oss << members[i]->toJson() << (i == members.size() - 1 ? "" : ",");

        oss << "]}";
        return oss.str();
    }

    GET_NODE_NAME(TemplateDef)

    // ----------------------------- Private -----------------------------

    std::string Private::toString() const { return "private " + statement->toString(); }

    std::string Private::toJson() const { return R"({"nodeName":"Private", "statement":)" + statement->toJson() + "}"; }

    GET_NODE_NAME(Private)

    // ----------------------------- Identifier -----------------------------

    std::string Identifier::toString() const { return name; }

    std::string Identifier::toJson() const {
        std::ostringstream oss;
        oss << R"({"nodeName": "Identifier", "name":")" << name << "\"}";
        return oss.str();
    }

    GET_NODE_NAME(Identifier)

    // ----------------------------- EnumRef -----------------------------

    std::string EnumRef::toString() const { return enumName->toString() + "/" + enumValue->toString(); }

    std::string EnumRef::toJson() const { return std::format(R"({{"nodeName": "EnumRef", "enumName": {}, "enumValue": {}}})", enumName->toJson(), enumValue->toJson()); }

    GET_NODE_NAME(EnumRef)

    // ----------------------------- MapRef -----------------------------

    std::string MapRef::toString() const {
        std::ostringstream oss;

        oss << "MAP [" << std::endl;

        for (int i = 0; i < pairs.size(); i++) oss << pairs[i]->toString() << (i == pairs.size() - 1 ? "" : ", \n");

        oss << "]";

        return oss.str();
    }

    std::string MapRef::toJson() const {
        std::ostringstream oss;
        oss << R"({"nodeName":"MapRef", "pairs":[)";

        for (int i = 0; i < pairs.size(); i++) oss << pairs[i]->toJson() << (i == pairs.size() - 1 ? "" : ",");

        oss << "]}";
        return oss.str();
    }

    GET_NODE_NAME(MapRef)

    // ----------------------------- ObjectIns -----------------------------

    std::string ObjectIns::toString() const {
        std::ostringstream oss;

        oss << identifier->toString() << '(' << std::endl;

        for (const auto &member : members) oss << member->toString() << std::endl;

        oss << ")" << std::endl;

        return oss.str();
    }

    std::string ObjectIns::toJson() const {
        std::ostringstream oss;
        oss << R"({"nodeName":"ObjectIns", "identifier":)" << identifier->toJson() << "," << R"("members":[)";

        for (int i = 0; i < members.size(); i++) oss << members[i]->toJson() << (i == members.size() - 1 ? "" : ",");

        oss << "]}";
        return oss.str();
    }

    GET_NODE_NAME(ObjectIns)

    // ----------------------------- ObjectRef -----------------------------

    std::string ObjectRef::toString() const { return "$/" + identifier->toString(); }

    std::string ObjectRef::toJson() const { return std::format(R"({{"nodeName": "ObjectRef", "identifier": {}}})", identifier->toJson()); }

    GET_NODE_NAME(ObjectRef)

    // ----------------------------- Operation -----------------------------

    std::string Operation::toString() const { return left->toString() + " " + operator_->toString() + " " + right->toString(); }

    std::string Operation::toJson() const { return std::format(R"({{"nodeName": "Operation", "left": {}, "operator": {}, "right": {}}})", left->toJson(), operator_->toJson(), right->toJson()); }

    GET_NODE_NAME(Operation)

    // ----------------------------- Operator -----------------------------

    std::string Operator::toString() const { return value; }

    std::string Operator::toJson() const { return R"({"nodeName": "Operator", "value":")" + value + "\"}"; }

    GET_NODE_NAME(Operator)

    // ----------------------------- TemplateParam -----------------------------

    std::string TemplateParam::toString() const { return '<' + identifier->toString() + '>'; }

    std::string TemplateParam::toJson() const { return std::format(R"({{"nodeName": "TemplateParam", "identifier": {}}})", identifier->toJson()); }

    GET_NODE_NAME(TemplateParam)

    // ----------------------------- TemplateRef -----------------------------

    std::string TemplateRef::toString() const { return "template " + identifier->toString(); }

    std::string TemplateRef::toJson() const { return std::format(R"({{"nodeName": "TemplateRef", "identifier": {}}})", identifier->toJson()); }

    GET_NODE_NAME(TemplateRef)

    // ----------------------------- Boolean -----------------------------

    std::string Boolean::toString() const { return value ? "true" : "false"; }

    std::string Boolean::toJson() const {
        std::ostringstream oss;
        oss << R"({"nodeName": "Boolean", "value": )" << (value ? "true" : "false") << "}";
        return oss.str();
    }

    GET_NODE_NAME(Boolean)

    // ----------------------------- Float -----------------------------

    std::string Float::toString() const { return std::to_string(value); }

    std::string Float::toJson() const { return R"({"nodeName": "Float", "value": )" + std::to_string(value) + "}"; }

    GET_NODE_NAME(Float)

    // ----------------------------- Integer -----------------------------

    std::string Integer::toString() const { return std::to_string(value); }

    std::string Integer::toJson() const { return R"({"nodeName": "Integer", "value": )" + std::to_string(value) + "}"; }

    GET_NODE_NAME(Integer)

    // ----------------------------- GUID -----------------------------

    std::string GUID::toString() const { return "GUID:{" + value + "}"; }

    std::string GUID::toJson() const { return R"({"nodeName": "GUID", "value":")" + value + "\"}"; }

    GET_NODE_NAME(GUID)

    // ----------------------------- Nil -----------------------------

    std::string Nil::toString() const { return "nil"; }

    std::string Nil::toJson() const { return R"({"nodeName": "Nil", "value": "nil"})"; }

    GET_NODE_NAME(Nil)

    // ----------------------------- Pair -----------------------------

    std::string Pair::toString() const { return "(" + first->toString() + ", " + second->toString() + ")"; }

    std::string Pair::toJson() const { return R"({"nodeName": "Pair", "first":)" + first->toJson() + R"(, "second":)" + second->toJson() + "}"; }

    GET_NODE_NAME(Pair)

    // ----------------------------- Path -----------------------------

    std::string Path::toString() const { return value; }

    std::string Path::toJson() const { return R"({"nodeName": "Path", "value":")" + value + "\"}"; }

    GET_NODE_NAME(Path)

    // ----------------------------- String -----------------------------

    std::string String::toString() const { return '"' + value + '"'; }

    std::string String::toJson() const { return R"({"nodeName": "String", "value": ")" + value + "\"}"; }

    GET_NODE_NAME(String)

    // ----------------------------- Vector -----------------------------

    std::string Vector::toString() const {
        std::ostringstream oss;

        oss << "[" << std::endl;

        for (int i = 0; i < expressions.size(); i++) oss << " " << expressions[i]->toString() << (i == expressions.size() - 1 ? "" : ", ");

        oss << "]";

        return oss.str();
    }

    std::string Vector::toJson() const {
        std::ostringstream oss;
        oss << R"({"nodeName": "Vector", "expressions": [)";
        for (int i = 0; i < expressions.size(); i++) oss << expressions[i]->toJson() << (i == expressions.size() - 1 ? "" : ",");
        oss << "]}";
        return oss.str();
    }

    GET_NODE_NAME(Vector)
}  // namespace ast
