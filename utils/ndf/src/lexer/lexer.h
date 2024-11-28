// Copyright (c) 2024. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file lexer.h
 * @author edocsitahw
 * @version 1.1
 * @date 2024/11/24 上午1:33
 * @brief
 * */
#ifndef WATCHER_LEXER_H
#define WATCHER_LEXER_H
#pragma once

/**
 * <program> ::= <statement>*
 * <statement> ::= <assignment> | <object_def> | <map_def> | <template_def> | <comment>
 * <assignment> ::= <identifier> "is" <expression>
 * <object_def> ::= <identifier> "is" <type> "(" <member_list>? ")"
 * <map_def> ::= "MAP" "[" <pair_list>? "]"
 * <pair_list> ::= <pair> ("," <pair>)*
 * <pair> ::= "(" <expression> "," <expression> ")"
 * <template_def> ::= "template" <identifier> "[" <parameter_list>? "]" "is" <type> "(" <member_list>? ")"
 * <parameter_list> ::= <parameter> ("," <parameter>)*
 * <parameter> ::= <identifier> [":" <type>] ["=" <expression>]
 * <comment> ::= "//" <text> | "\*" <text> "*\"
 * <member_list> ::= <member> ("," <member>)*
 * <member> ::= <identifier> "=" <expression>
 * <expression> ::= <literal> | <identifier> | <operation> | <object_ref> | <map_ref> | <template_ref>
 * <literal> ::= <boolean> | <string> | <integer> | <float> | <vector> | <tuple>
 * <boolean> ::= "true" | "false"
 * <string> ::= "'" <text> "'" | "\"" <text> "\""
 * <integer> ::= <digit>+
 * <float> ::= <digit>* "." <digit>+
 * <vector> ::= "[" <expression_list>? "]"
 * <expression_list> ::= <expression> ("," <expression>)*
 * <tuple> ::= "(" <expression> "," <expression> ")"
 * <identifier> ::= <letter> (<letter> | <digit>)*
 * <operation> ::= <expression> <operator> <expression>
 * <operator> ::= "+" | "-" | "*" | "/" | "%" | "div" | "<" | ">" | "<=" | ">=" | "==" | "!=" | "|"
 * <object_ref> ::= "$" "/" <identifier>
 * <map_ref> ::= "MAP[" <pair_list> "]"
 * <template_ref> ::= "template" <identifier>
 * */

#include "token.h"
#include <set>

extern std::set<char> g_Level1Op;


class Lexer {
protected:
    using Type = TokenType;
    const std::string _source;
    Pos _pos;
    void skip();
    Token extractNumber();
    Token extractString();
    Token extractLiteral();
    Token extractOperator();
    Token extractComment();
    Token extractBlockComment();
    Token extractPath();
    [[nodiscard]] char curr() const;
    [[nodiscard]] bool inScope() const;

public:
    explicit Lexer(std::string source);
    Token operator*();
    static TokenPtrs tokenize(const std::string& text);
};

#endif  // WATCHER_LEXER_H
