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

namespace ast {
    std::string idt(int indent) { return std::string(indent * 4, ' '); }

    std::string Program::toString() const {
        std::ostringstream oss;

        for (const auto &stmt: statements) oss << stmt->toString() << std::endl;

        return oss.str();
    }

    std::string Program::toJson() const {
        std::ostringstream oss;
        oss << R"({"nodeName":"Program", "statements":[)";
        for (int i = 0; i < statements.size(); i++)
            oss << statements[i]->toJson() << (i == statements.size() - 1 ? "" : ",");
        oss << "]}";

        return oss.str();
    }

    std::string Assignment::toString() const {
        return idt(indent) + identifier->toString() + " is " + expression->toString() + '\n';
    }

    std::string Assignment::toJson() const {
        std::ostringstream oss;
        oss << R"({"nodeName":"Assignment", "identifier":)" << identifier->toJson() << "}";

        return oss.str();
    }

    std::string ObjectDef::toString() const {
        std::ostringstream oss;

        oss << idt(indent) << identifier->toString() << " is " << type->toString() << "(" << std::endl;

        for (int i = 0; i < memberList.size(); i++)
            oss << idt(indent + 1) << memberList[i]->toString() << (i == memberList.size() - 1 ? "" : ",") << std::endl;

        oss << ")" << std::endl;

        return oss.str();
    }

    std::string ObjectDef::toJson() const {
        std::ostringstream oss;
        oss << R"({"nodeName": "ObjectDef", "identifier": )" << identifier->toJson() << "," << R"("type":)"
            << type->toJson() << "," << R"("members": [)";

        for (int i = 0; i < memberList.size(); i++)
            oss << memberList[i]->toJson() << (i == memberList.size() - 1 ? "" : ",");

        oss << "]}";

        return oss.str();
    }

    std::string MapDef::toString() const {
        std::ostringstream oss;

        oss << "MAP [" << std::endl;

        for (const auto &pair: pairList) oss << idt(indent + 1) << pair->toString() << "," << std::endl;

        oss << "]" << std::endl;

        return oss.str();
    }

    std::string MapDef::toJson() const {
        std::ostringstream oss;
        oss << R"({"nodeName": "MapDef", "pairs":[)";
        for (int i = 0; i < pairList.size(); i++) oss << pairList[i]->toJson() << (i == pairList.size() - 1 ? "" : ",");

        oss << "]}";

        return oss.str();
    }

    std::string Parameter::toString() const {
        return identifier->toString() + (type ? ": " + type->toString() : "") +
               (expression ? " = " + expression->toString() : "");
    }

    std::string Parameter::toJson() const {
        std::ostringstream oss;
        oss << R"({"nodeName":"Parameter", "identifier":)" << identifier->toJson() << "," << R"("type":)"
            << (type ? type->toJson() : "null") << "," << R"("expression":)"
            << (expression ? expression->toJson() : "null") << "}";

        return oss.str();
    }

    std::string TemplateDef::toString() const {
        std::ostringstream oss;

        oss << idt(indent) << "template " << identifier->toString() << "[" << std::endl;

        for (int i = 0; i < parameterList.size(); i++)
            oss << idt(indent + 1) << parameterList[i]->toString() << (i == parameterList.size() - 1 ? "" : ",")
                << std::endl;

        oss << "] is " << type->toString() << "(" << std::endl;

        for (const auto &member: memberList) oss << idt(indent + 1) << member->toString() << std::endl;

        oss << idt(indent) << ")" << std::endl;

        return oss.str();
    }

    std::string TemplateDef::toJson() const {
        std::ostringstream oss;
        oss << R"({"nodeName":"TemplateDef", "identifier":)" << identifier->toJson() << "," << R"("parameters":[)";

        for (int i = 0; i < parameterList.size(); i++)
            oss << parameterList[i]->toJson() << (i == parameterList.size() - 1 ? "" : ",");
        oss << "]," << R"("type":)" << type->toJson() << "," << R"("memberList":[)";

        for (int i = 0; i < memberList.size(); i++)
            oss << memberList[i]->toJson() << (i == memberList.size() - 1 ? "" : ",");

        oss << "]}";
        return oss.str();
    }

    std::string Export::toString() const { return "export " + statement->toString(); }

    std::string Export::toJson() const {
        std::ostringstream oss;
        oss << R"({"nodeName":"Export", "statement":)" << statement->toJson() << "}";
        return oss.str();
    }

    std::string Nil::toString() const { return "nil"; }

    std::string Nil::toJson() const { return R"({"nodeName": "Nil", "value": "nil"})"; }

    std::string Member::toString() const { return identifier->toString() + " = " + expression->toString(); }

    std::string Member::toJson() const {
        std::ostringstream oss;
        oss << R"({"nodeName": "Member", "identifier":)" << identifier->toJson() << "," << R"("expression": )"
            << expression->toJson() << "}";
        return oss.str();
    }

    std::string GUID::toString() const { return "GUID:{" + value + "}"; }

    std::string GUID::toJson() const { return R"({"nodeName": "GUID", "value":")" + value + "\"}"; }

    std::string Path::toString() const { return value; }

    std::string Path::toJson() const { return R"({"nodeName": "Path", "value":")" + value + "\"}"; }

    std::string Pair::toString() const { return "(" + first->toString() + ", " + second->toString() + ")"; }

    std::string Pair::toJson() const {
        return R"({"nodeName": "Pair", "first":)" + first->toJson() + R"(, "second":)" + second->toJson() + "}";
    }

    std::string Boolean::toString() const { return value ? "true" : "false"; }

    std::string Boolean::toJson() const {
        std::ostringstream oss;
        oss << R"({"nodeName": "Boolean", "value": )" << (value ? "true" : "false") << "}";
        return oss.str();
    }

    std::string String::toString() const { return '"' + value + '"'; }

    std::string String::toJson() const { return R"({"nodeName": "String", "value": ")" + value + "\"}"; }

    std::string Integer::toString() const { return std::to_string(value); }

    std::string Integer::toJson() const { return R"({"nodeName": "Integer", "value": )" + std::to_string(value) + "}"; }

    std::string Float::toString() const { return std::to_string(value); }

    std::string Float::toJson() const { return R"({"nodeName": "Float", "value": )" + std::to_string(value) + "}"; }

    std::string Vector::toString() const {
        std::ostringstream oss;

        oss << "[" << std::endl;

        for (int i = 0; i < expressionList.size(); i++)
            oss << idt(indent + 1) << " " << expressionList[i]->toString()
                << (i == expressionList.size() - 1 ? "" : ", ");

        oss << idt(indent) << "]";

        return oss.str();
    }

    std::string Vector::toJson() const {
        std::ostringstream oss;
        oss << R"({"nodeName": "Vector", "expressions": [)";
        for (int i = 0; i < expressionList.size(); i++)
            oss << expressionList[i]->toJson() << (i == expressionList.size() - 1 ? "" : ",");
        oss << "]}";
        return oss.str();
    }

    std::string Identifier::toString() const { return name; }

    std::string Identifier::toJson() const {
        std::ostringstream oss;
        oss << R"({"nodeName": "Identifier", "name":")" << name << "\"}";
        return oss.str();
    }

    std::string Operation::toString() const {
        return left->toString() + " " + operator_->toString() + " " + right->toString();
    }

    std::string Operation::toJson() const {
        return std::format(R"({{"nodeName": "Operation", "left": {}, "operator": {}, "right": {}}})", left->toJson(),
                           operator_->toJson(), right->toJson());
    }

    std::string Operator::toString() const { return value; }

    std::string Operator::toJson() const { return R"({"nodeName": "Operator", "value":")" + value + "\"}"; }

    std::string ObjectRef::toString() const { return "$/" + identifier->toString(); }

    std::string ObjectRef::toJson() const {
        return std::format(R"({{"nodeName": "ObjectRef", "identifier": {}}})", identifier->toJson());
    }

    std::string ObjectIns::toString() const {
        std::ostringstream oss;

        oss << idt(indent) << identifier->toString() << '(' << std::endl;

        for (const auto &member: memberList) oss << idt(indent + 1) << member->toString() << std::endl;

        oss << idt(indent) << ")" << std::endl;

        return oss.str();
    }

    std::string ObjectIns::toJson() const {
        std::ostringstream oss;
        oss << R"({"nodeName":"ObjectIns", "identifier":)" << identifier->toJson() << "," << R"("members":[)";

        for (int i = 0; i < memberList.size(); i++)
            oss << memberList[i]->toJson() << (i == memberList.size() - 1 ? "" : ",");

        oss << "]}";
        return oss.str();
    }

    std::string MapRef::toString() const {
        std::ostringstream oss;

        oss << "MAP [" << std::endl;

        for (int i = 0; i < pairList.size(); i++)
            oss << idt(indent + 1) << pairList[i]->toString() << (i == pairList.size() - 1 ? "" : ", \n");

        oss << idt(indent) << "]";

        return oss.str();
    }

    std::string MapRef::toJson() const {
        std::ostringstream oss;
        oss << R"({"nodeName":"MapRef", "pairs":[)";

        for (int i = 0; i < pairList.size(); i++) oss << pairList[i]->toJson() << (i == pairList.size() - 1 ? "" : ",");

        oss << "]}";
        return oss.str();
    }

    std::string TemplateRef::toString() const { return "template " + identifier->toString(); }

    std::string TemplateRef::toJson() const {
        return std::format(R"({{"nodeName": "TemplateRef", "identifier": {}}})", identifier->toJson());
    }

    std::string EnumRef::toString() const { return enumName->toString() + "/" + enumValue->toString(); }

    std::string EnumRef::toJson() const {
        return std::format(R"({{"nodeName": "EnumRef", "enumName": {}, "enumValue": {}}})", enumName->toJson(),
                           enumValue->toJson());
    }

    std::string TemplateParam::toString() const { return '<' + identifier->toString() + '>'; }

    std::string TemplateParam::toJson() const {
        return std::format(R"({{"nodeName": "TemplateParam", "identifier": {}}})", identifier->toJson());
    }

    Identifier::Identifier(std::string name)
            : name(std::move(name)) {}

    Operator::Operator(std::string value)
            : value(std::move(value)) {}

    Operation::Operation(std::shared_ptr<Expression> left, std::shared_ptr<Operator> operator_,
                         std::shared_ptr<Expression> right)
            : left(std::move(left)), operator_(std::move(operator_)), right(std::move(right)) {}

    GUID::GUID(std::string value)
            : value(std::move(value)) {}

    Path::Path(std::string value)
            : value(std::move(value)) {}

}  // namespace ast
