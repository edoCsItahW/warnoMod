// Copyright (c) 2024. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file build.cpp
 * @author edocsitahw
 * @version 1.1
 * @date 2024/11/25 下午4:18
 * @brief
 * */

#include <utility>

#include "../src/lexer/tokenProc.h"
#include "../src/parser/ast.h"
#include "../src/parser/parser.h"
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

namespace py = pybind11;

template<typename T>
auto dereference(T* t) {
    if (t == nullptr) throw std::runtime_error("Null pointer dereference.");
    return *t;
};

PYBIND11_MODULE(ndfPyAPI, m) {
    m.doc() = R"(
        Python API for Lexer and Parser in NDF language.
        用于NDF语言中Lexer和Parser的Python API
    )";

    m.def("dereference", &dereference<std::shared_ptr<Token>>, "Dereference a pointer.", py::arg("t"));

    py::enum_<TokenType>(m, "TokenType")
        .value("NUMBER", TokenType::NUMBER)
        .value("INT", TokenType::INT)
        .value("FLOAT", TokenType::FLOAT)
        .value("STRING", TokenType::STRING)
        .value("PATH", TokenType::PATH)
        .value("LITERAL", TokenType::LITERAL)
        .value("IDENTIFIER", TokenType::IDENTIFIER)
        .value("KW_IS", TokenType::KW_IS)
        .value("KW_MAP", TokenType::KW_MAP)
        .value("KW_DIV", TokenType::KW_DIV)
        .value("KW_TEMPLATE", TokenType::KW_TEMPLATE)
        .value("KW_EXPORT", TokenType::KW_EXPORT)
        .value("KW_UNNAMED", TokenType::KW_UNNAMED)
        .value("KW_NIL", TokenType::KW_NIL)
        .value("KW_PRIVATE", TokenType::KW_PRIVATE)
        .value("KW_BOOLEN", TokenType::KW_BOOLEN)
        .value("GUID", TokenType::GUID)
        .value("OPERATOR", TokenType::OPERATOR)
        .value("ADD", TokenType::ADD)
        .value("SUB", TokenType::SUB)
        .value("MUL", TokenType::MUL)
        .value("DIV", TokenType::DIV)
        .value("MOD", TokenType::MOD)
        .value("ASSIGN", TokenType::ASSIGN)
        .value("LT", TokenType::LT)
        .value("GT", TokenType::GT)
        .value("LE", TokenType::LE)
        .value("GE", TokenType::GE)
        .value("EQ", TokenType::EQ)
        .value("NE", TokenType::NE)
        .value("OR", TokenType::OR)
        .value("NOT", TokenType::NOT)
        .value("LPAREN", TokenType::LPAREN)
        .value("RPAREN", TokenType::RPAREN)
        .value("LBRACE", TokenType::LBRACE)
        .value("RBRACE", TokenType::RBRACE)
        .value("LBRACKET", TokenType::LBRACKET)
        .value("RBRACKET", TokenType::RBRACKET)
        .value("COMMA", TokenType::COMMA)
        .value("SEMICOLON", TokenType::SEMICOLON)
        .value("COLON", TokenType::COLON)
        .value("DOT", TokenType::DOT)
        .value("DOLLAR", TokenType::DOLLAR)
        .value("QUESTION", TokenType::QUESTION)
        .value("TILDE", TokenType::TILDE)
        .value("COMMENT", TokenType::COMMENT)
        .value("NEWLINE", TokenType::NEWLINE)
        .value("EOF", TokenType::EOF_)
        .value("ERROR", TokenType::ERROR_)
        .value("UNKNOWN", TokenType::UNKNOWN)
        .export_values();

    py::class_<Pos>(m, "Pos")
        // Pos(size_t line, size_t col, size_t value)
        .def(py::init<size_t, size_t, size_t>(), "Constructor of Position class", py::arg("line"), py::arg("col"), py::arg("value"))
        // int line() const
        .def_property_readonly("line", &Pos::line, "Get the line number of the position")
        // int col() const
        .def_property_readonly("col", &Pos::col, "Get the column number of the position")
        // size_t value() const
        .def("__call__", &Pos::operator(), "Get the value of the position")
        // Pos& operator++()
        .def("increment", (Pos & (Pos::*)()) & Pos::operator++, "Increment the position by 1 and automatically update the column number and value")
        // Pos operator++(int)
        .def("incrementPostfix", (Pos(Pos::*)(int)) & Pos::operator++, "First 'return' the current position, then increment the position by 1 and automatically update the column number and value")
        // Pos newLine()
        .def("newLine", &Pos::newLine, "Set the position to the beginning of a new line and automatically update the line number and value")
        // Pos& operator--()
        .def("decrement", (Pos & (Pos::*)()) & Pos::operator--, "Decrement the position by 1 and automatically update the column number and value")
        // Pos operator--(int)
        .def("decrementPostfix", (Pos(Pos::*)(int)) & Pos::operator--, "First 'return' the current position, then decrement the position by 1 and automatically update the column number and value")
        // Pos& operator+=(const Pos& other)
        .def("__iadd__", &Pos::operator+=, "Self increment assignment, similar to the increment function", py::arg("other"))
        // Pos& operator-=(const Pos& other)
        .def("__isub__", &Pos::operator-=, "Self decrement assignment, similar to the reduce function", py::arg("other"))
        // template<typename Pos> bool operator<(const Pos& other) const
        .def("__lt__", (bool(Pos::*)(const Pos&) const) & Pos::operator<, "[RELOAD]<Pos>", py::arg("other"))
        // template<typename size_t> bool operator<(const size_t& other) const
        .def("__lt__", (bool(Pos::*)(const size_t&) const) & Pos::operator<, "[RELOAD]<size_t>", py::arg("other"))
        // template<typename int> bool operator<(const int& other) const
        .def("__lt__", (bool(Pos::*)(const int&) const) & Pos::operator<, "[RELOAD]<int>", py::arg("other"))
        // template<typename Pos> bool operator<=(const Pos& other) const
        .def("__le__", (bool(Pos::*)(const Pos&) const) & Pos::operator<=, "[RELOAD]<Pos>", py::arg("other"))
        // template<typename size_t> bool operator<=(const size_t& other) const
        .def("__le__", (bool(Pos::*)(const size_t&) const) & Pos::operator<=, "[RELOAD]<size_t>", py::arg("other"))
        // template<typename int> bool operator<=(const int& other) const
        .def("__le__", (bool(Pos::*)(const int&) const) & Pos::operator<=, "[RELOAD]<int>", py::arg("other"))
        // template<typename Pos> bool operator>(const Pos& other) const
        .def("__gt__", (bool(Pos::*)(const Pos&) const) & Pos::operator>, "[RELOAD]<Pos>", py::arg("other"))
        // template<typename size_t> bool operator>(const size_t& other) const
        .def("__gt__", (bool(Pos::*)(const size_t&) const) & Pos::operator>, "[RELOAD]<size_t>", py::arg("other"))
        // template<typename int> bool operator>(const int& other) const
        .def("__gt__", (bool(Pos::*)(const int&) const) & Pos::operator>, "[RELOAD]<int>", py::arg("other"))
        // template<typename Pos> bool operator>=(const Pos& other) const
        .def("__ge__", (bool(Pos::*)(const Pos&) const) & Pos::operator>=, "[RELOAD]<Pos>", py::arg("other"))
        // template<typename size_t> bool operator>=(const size_t& other) const
        .def("__ge__", (bool(Pos::*)(const size_t&) const) & Pos::operator>=, "[RELOAD]<size_t>", py::arg("other"))
        // template<typename int> bool operator>=(const int& other) const
        .def("__ge__", (bool(Pos::*)(const int&) const) & Pos::operator>=, "[RELOAD]<int>", py::arg("other"))
        // template<typename Pos> bool operator==(const Pos& other) const
        .def("__eq__", (bool(Pos::*)(const Pos&) const) & Pos::operator==, "[RELOAD]<Pos>", py::arg("other"))
        // template<typename size_t> bool operator==(const size_t& other) const
        .def("__eq__", (bool(Pos::*)(const size_t&) const) & Pos::operator==, "[RELOAD]<size_t>", py::arg("other"))
        // template<typename int> bool operator==(const int& other) const
        .def("__eq__", (bool(Pos::*)(const int&) const) & Pos::operator==, "[RELOAD]<int>", py::arg("other"))
        // template<typename Pos> bool operator!=(const Pos& other) const
        .def("__ne__", (bool(Pos::*)(const Pos&) const) & Pos::operator!=, "[RELOAD]<Pos>", py::arg("other"))
        // template<typename size_t> bool operator!=(const size_t& other) const
        .def("__ne__", (bool(Pos::*)(const size_t&) const) & Pos::operator!=, "[RELOAD]<size_t>", py::arg("other"))
        // template<typename int> bool operator!=(const int& other) const
        .def("__ne__", (bool(Pos::*)(const int&) const) & Pos::operator!=, "[RELOAD]<int>", py::arg("other"))
        // size_t operator+(int other) const
        .def("__add__", (size_t(Pos::*)(int) const) & Pos::operator+, "[RELOAD]<int>", py::arg("other"))
        // size_t operator-(int other) const
        .def("__sub__", (size_t(Pos::*)(int) const) & Pos::operator-, "[RELOAD]<int>", py::arg("other"));

//    py::class_<Token>(m, "Token")
//        .def_readwrite("type", &Token::type, "Get the type of the token")
//        .def_readwrite("pos", &Token::pos, "Get the position of the token")
//        .def_readwrite("value", &Token::value, "Get the value of the token")
//        .def_readwrite("super", &Token::super, "superordinate")
//        .def("__str__", &Token::toString, "Get the string representation of the token");

    //    m.attr("g_Level10p") = py::cast(g_Level1Op);

//    py::class_<Lexer>(m, "Lexer")
//        .def(py::init<std::string>(), "Constructor of Lexer class", py::arg("source"))
//        .def("next", &Lexer::operator*, "Get the next token and move the position to the next token")
//        .def_static("tokenize", &Lexer::tokenize, "Tokenize the source code and return the tokens", py::arg("text"));

//    py::class_<TokenProcessor>(m, "TokenProcessor")
//        .def(py::init<TokenPtrs>(), "Constructor of TokenProcessor class", py::arg("tokens"))
//        .def("next", &TokenProcessor::operator*, "Get the next token and move the position to the next token")
//        .def_static("process", &TokenProcessor::process, "Process the token list and return the token list that has undergone Level 2 processing", py::arg("tokens"), py::arg("common") = false);

    // ------------------------------- AST ----------------------------------

    py::class_<ast::AST>(m, "AST");

    py::class_<ast::Program>(m, "Program")
        .def_readwrite("indent", &ast::Program::indent, "Indent of the program")
        .def_readwrite("statements", &ast::Program::statements, "Get the statements of the program")
        .def_readonly("parent", &ast::Program::parent, "Get the parent of the program")
        .def_readonly("children", &ast::Program::children, "Get the children of the program");

    py::class_<ast::Statement>(m, "Statement")
        .def_readwrite("indent", &ast::Statement::indent, "Indent of the statement");

    py::class_<ast::Assignment>(m, "Assignment")
        .def_readwrite("indent", &ast::Assignment::indent, "Indent of the assignment")
        .def_readwrite("identifier", &ast::Assignment::identifier, "Get the identifier of the assignment")
        .def_readwrite("expression", &ast::Assignment::expression, "Get the expression of the assignment");

    py::class_<ast::ObjectDef>(m, "ObjectDef")
        .def_readwrite("indent", &ast::ObjectDef::indent, "Indent of the object definition")
        .def_readwrite("identifier", &ast::ObjectDef::identifier, "Get the identifier of the object definition")
        .def_readwrite("type", &ast::ObjectDef::type, "Get the type of the object definition")
        .def_readwrite("members", &ast::ObjectDef::memberList, "Get the member list of the object definition");

    py::class_<ast::MapDef>(m, "MapDef")
        .def_readwrite("indent", &ast::MapDef::indent, "Indent of the map definition")
        .def_readwrite("pairs", &ast::MapDef::pairList, "Get the pairs of the map definition");

    py::class_<ast::TemplateDef>(m, "TemplateDef")
        .def_readwrite("indent", &ast::TemplateDef::indent, "Indent of the template definition")
        .def_readwrite("identifier", &ast::TemplateDef::identifier, "Get the identifier of the template definition")
        .def_readwrite("type", &ast::TemplateDef::type, "Get the type of the template definition")
        .def_readwrite("members", &ast::TemplateDef::memberList, "Get the member list of the template definition");

    py::class_<ast::Export>(m, "Export")
        .def_readwrite("indent", &ast::Export::indent, "Indent of the export")
        .def_readwrite("statement", &ast::Export::statement, "Get the statement of the export");

    py::class_<ast::Parameter>(m, "Parameter")
        .def_readwrite("indent", &ast::Parameter::indent, "Indent of the parameter")
        .def_readwrite("identifier", &ast::Parameter::identifier, "Get the identifier of the parameter")
        .def_readwrite("type", &ast::Parameter::type, "Get the type of the parameter")
        .def_readwrite("expression", &ast::Parameter::expression, "Get the expression of the parameter");

    py::class_<ast::Member>(m, "Member")
        .def_readwrite("indent", &ast::Member::indent, "Indent of the member")
        .def_readwrite("identifier", &ast::Member::identifier, "Get the identifier of the member")
        .def_readwrite("expression", &ast::Member::expression, "Get the expression of the member");

    py::class_<ast::Expression>(m, "Expression")
        .def_readwrite("indent", &ast::Expression::indent, "Indent of the expression");

    py::class_<ast::Literal>(m, "Literal")
        .def_readwrite("indent", &ast::Literal::indent, "Indent of the literal");

    py::class_<ast::GUID>(m, "Guid")
        .def_readwrite("indent", &ast::GUID::indent, "Indent of the GUID")
        .def_readwrite("value", &ast::GUID::value, "Get the value of the GUID");

    py::class_<ast::Path>(m, "Path")
        .def_readwrite("indent", &ast::Path::indent, "Indent of the path")
        .def_readwrite("value", &ast::Path::value, "Get the value of the path");

    py::class_<ast::Pair>(m, "Pair")
        .def_readwrite("indent", &ast::Pair::indent, "Indent of the pair")
        .def_readwrite("first", &ast::Pair::first, "Get the first element of the pair")
        .def_readwrite("second", &ast::Pair::second, "Get the second element of the pair");

    py::class_<ast::Boolean>(m, "Boolean")
        .def_readwrite("indent", &ast::Boolean::indent, "Indent of the boolean")
        .def_readwrite("value", &ast::Boolean::value, "Get the value of the boolean");

    py::class_<ast::String>(m, "String")
        .def_readwrite("indent", &ast::String::indent, "Indent of the string")
        .def_readwrite("value", &ast::String::value, "Get the value of the string");

    py::class_<ast::Integer>(m, "Integer")
        .def_readwrite("indent", &ast::Integer::indent, "Indent of the integer")
        .def_readwrite("value", &ast::Integer::value, "Get the value of the integer");

    py::class_<ast::Float>(m, "Float")
        .def_readwrite("indent", &ast::Float::indent, "Indent of the float")
        .def_readwrite("value", &ast::Float::value, "Get the value of the float");

    py::class_<ast::Nil>(m, "Nil")
        .def_readwrite("indent", &ast::Nil::indent, "Indent of the nil");

    py::class_<ast::TemplateParam>(m, "TemplateParam")
        .def_readwrite("indent", &ast::TemplateParam::indent, "Indent of the template parameter")
        .def_readwrite("identifier", &ast::TemplateParam::identifier, "Get the identifier of the template parameter");

    py::class_<ast::Vector>(m, "Vector")
        .def_readwrite("indent", &ast::Vector::indent, "Indent of the vector")
        .def_readwrite("expressions", &ast::Vector::expressionList, "Get the expressions of the vector");

    py::class_<ast::Identifier>(m, "Identifier")
        .def_readwrite("indent", &ast::Identifier::indent, "Indent of the identifier")
        .def_readwrite("name", &ast::Identifier::name, "Get the name of the identifier");

    py::class_<ast::Operation>(m, "Operation")
        .def_readwrite("indent", &ast::Operation::indent, "Indent of the operation")
        .def_readwrite("left", &ast::Operation::left, "Get the left operand of the operation")
        .def_readwrite("operator", &ast::Operation::operator_, "Get the operator of the operation")
        .def_readwrite("right", &ast::Operation::right, "Get the right operand of the operation");

    py::class_<ast::Operator>(m, "Operator")
        .def_readwrite("indent", &ast::Operator::indent, "Indent of the operator")
        .def_readwrite("value", &ast::Operator::value, "Get the value of the operator");

    py::class_<ast::ObjectRef>(m, "ObjectRef")
        .def_readwrite("indent", &ast::ObjectRef::indent, "Indent of the object reference")
        .def_readwrite("identifier", &ast::ObjectRef::identifier, "Get the identifier of the object reference");

    py::class_<ast::ObjectIns>(m, "ObjectIns")
        .def_readwrite("indent", &ast::ObjectIns::indent, "Indent of the object instantiation")
        .def_readwrite("identifier", &ast::ObjectIns::identifier, "Get the identifier of the object instantiation")
        .def_readwrite("members", &ast::ObjectIns::memberList, "Get the member list of the object instantiation");

    py::class_<ast::MapRef>(m, "MapRef")
        .def_readwrite("indent", &ast::MapRef::indent, "Indent of the map reference")
        .def_readwrite("pairs", &ast::MapRef::pairList, "Get the pairs of the map reference");

    py::class_<ast::EnumRef>(m, "EnumRef")
        .def_readwrite("indent", &ast::EnumRef::indent, "Indent of the enum reference")
        .def_readwrite("enumName", &ast::EnumRef::enumName, "Get the name of the enum reference")
        .def_readwrite("enumValue", &ast::EnumRef::enumValue, "Get the value of the enum reference");

    py::class_<ast::TemplateRef>(m, "TemplateRef")
        .def_readwrite("indent", &ast::TemplateRef::indent, "Indent of the template reference")
        .def_readwrite("identifier", &ast::TemplateRef::identifier, "Get the identifier of the template reference");

    py::class_<Parser>(m, "Parser")
        .def(py::init<TokenPtrs>(), "Constructor of Parser class", py::arg("tokens"))
        .def("parse", &Parser::parse, "Parse the token list and return the AST", py::arg("debug"));

    m.def("parse", [](const std::string& text) -> std::shared_ptr<ast::Program> {
        auto tokens = TokenProcessor::process(Lexer::tokenize(text));
        return Parser(tokens).parse();
    }, "Parse the text and return the AST", py::arg("text"));
}
