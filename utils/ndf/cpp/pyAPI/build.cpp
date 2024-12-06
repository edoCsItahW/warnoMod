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
#include <confunc.h>

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


    m.def("parseAST", [](const std::string& target, const std::string& cwd = ".") {
        auto text = std::get<std::string>(os::readFile(target, os::Type::UTF8, cwd));

        auto tokens = TokenProcessor::process(Lexer::tokenize(text));

        auto parser = Parser(tokens);

        auto result = parser.parse();

        return result->toJson();
    }, "Parse the NDF code and return the AST in JSON format", py::arg("target"), py::arg("cwd") = ".");
}
