// Copyright (c) 2024. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file parser.h
 * @author edocsitahw
 * @version 1.1
 * @date 2024/11/24 上午1:35
 * @brief
 * */
#ifndef NDF_PARSER_H
#define NDF_PARSER_H
#pragma once

/**
 * @if zh
 * @page bnf BNF描述
 * @else
 * @page bnf BNF Description
 * @endif
 *
 * @code{.bnf}
 * - [x] <program> ::= <statement>*
 * - [x] <statement> ::= <assignment> | <objectDef> | <mapDef> | <templateDef> | <comment> | <export>
 * - [x] <export> ::= "export" <statement>
 * - [x] <assignment> ::= <identifier> "is" <expression>
 * - [x] <objectDef> ::= <identifier> "is" <type> "(" <member_list>? ")"
 * - [x] <object_instance> ::= <identifier> "(" <member_list>? ")"
 * - [x] <map_def> ::= "MAP" "[" <pair_list>? "]"
 * - [\] <pair_list> ::= <pair> ("," <pair>)*
 * - [x] <pair> ::= "(" <expression> "," <expression> ")"
 * - [x] <template_def> ::= "template" <identifier> "[" <parameter_list>? "]" "is" <type> "(" <member_list>? ")"
 * - [x] <template_param> ::= '<' <identifier> '>'
 * - [\] <parameter_list> ::= <parameter> ("," <parameter>)*
 * - [x] <parameter> ::= <identifier> [":" <type>] ["=" <expression>]
 * - [x] <comment> ::= "//" <text> | "\*" <text> "*\"
 * - [\] <member_list> ::= <member> ("," <member>)*
 * - [x] <member> ::= <identifier> "=" <expression>
 * - [x] <expression> ::= <literal> | <identifier> | <operation> | <object_ref> | <map_ref> | <template_ref> | <object_instance> | <template_param>
 * - [x] <literal> ::= <boolean> | <string> | <integer> | <float> | <vector> | <pair>
 * - [x] <boolean> ::= "true" | "false"
 * - [x] <string> ::= "'" <text> "'" | "\"" <text> "\""
 * - [x] <integer> ::= <digit>+
 * - [x] <float> ::= <digit>* "." <digit>+
 * - [x] <vector> ::= "[" <expression_list>? "]"
 * - [\] <expression_list> ::= <expression> ("," <expression>)*
 * - [x] <identifier> ::= <letter> (<letter> | <digit>)*
 * - [x] <operation> ::= <expression> <operator> <expression>
 * - [x] <operator> ::= "+" | "-" | "*" | "/" | "%" | "div" | "<" | ">" | "<=" | ">=" | "==" | "!=" | "|"
 * - [x] <object_ref> ::= "$" "/" <identifier>
 * - [x] <map_ref> ::= "MAP[" <pair_list> "]"
 * - [x] <template_ref> ::= "template" <identifier>
 * - [x] <enum_ref> ::= <identifier> '/' <identifier>
 * @endcode
 * */

#include "../lexer/lexer.h"
#include "ast.h"
#include <source_location>
#include <variant>

/** @struct FUEKwargs
 *
 * @if zh
 * @brief 函数@ref Parser::fromUntilExpect() 的@b 可变关键字 参数
 * @details 用于控制@ref Parser::fromUntilExpect() 函数的行为，其包括: firstStop, skipNewLine, debug
 *
 * @else
 * @brief The keyword arguments of function @ref Parser::fromUntilExpect()
 * @details It is used to control the behavior of function @ref Parser::fromUntilExpect(). It includes: firstStop, skipNewLine, debug
 *
 * @endif
 *
 * @see Parser::fromUntilExpect()
 * */
struct FUEKwargs {
    /** @var firstStop
     * @public @memberof FUEKwargs
     *
     * @if zh
     * @brief 是否当遍历到的第一个Token类型不是期望的类型时停止
     * @details 当@c firstStop 为@b true 时，如果遍历到的第一个Token类型不是期望的类型，则函数@ref Parser::fromUntilExpect() 会立即返回@b false ，而不会继续遍历Token列表。
     *
     * @else
     * @brief Whether to stop when the first token type is not the expected type
     * @details When @c firstStop is @b true, if the first token type is not the expected type, the function @ref Parser::fromUntilExpect() will immediately return @b false and will not continue to traverse the token list.
     *
     * @endif
     *
     * @see Parser::fromUntilExpect()
     * */
    bool firstStop   = false;
    /** @var skipNewLine
     * @public @memberof FUEKwargs
     *
     * @if zh
     * @brief 是否跳过换行符
     * @details 当@c skipNewLine 为@b true 时，如果遍历到换行符，则函数@ref Parser::fromUntilExpect() 会跳过该换行符。
     * @note 注意,如果期望的Token类型是@ref TokenType::NEWLINE，则即使@c skipNewLine 为@b true 也不会跳过换行符。
     *
     * @else
     * @brief Whether to skip the newline character
     * @details When @c skipNewLine is @b true, if the function @ref Parser::fromUntilExpect() encounters a newline character, it will skip it.
     * @note Note that if the expected token type is @ref TokenType::NEWLINE, even if @c skipNewLine is @b true , it will not skip the newline character.
     *
     * @endif
     *
     * @see Parser::fromUntilExpect()
     * */
    bool skipNewLine = true;
    /** @var debug
     * @public @memberof FUEKwargs
     *
     * @if zh
     * @brief 是否打印调试信息
     * @details 当@c debug 为@b true 时，函数@ref Parser::fromUntilExpect() 会打印调试信息,形如@e "check: <token>" 。
     *
     * @else
     * @brief Whether to print debugging information
     * @details When @c debug is @b true, the function @ref Parser::fromUntilExpect() will print debugging information.
     *
     * @endif
     *
     * @see Parser::fromUntilExpect()
     * */
    bool debug       = false;
};

class Parser {
private:
    TokenPtrs _tokens;
    size_t _idx;
    bool _debug;
    [[nodiscard]] TokenPtr curr() const;
    void skip();
    TokenPtr expect(TokenType type, bool skipNewLine = true, const std::source_location& loc = std::source_location::current());
    bool fromUntilExpect(std::variant<int, size_t> start, std::variant<int, size_t> end, TokenType type, const FUEKwargs& kwargs = FUEKwargs());
    TokenPtr expect(std::initializer_list<TokenType> types);
    [[nodiscard]] bool inScope() const;

    std::shared_ptr<ast::Nil> parseNil();
    std::shared_ptr<ast::Pair> parsePair();
    std::shared_ptr<ast::GUID> parseGuid();
    std::shared_ptr<ast::Path> parsePath();
    std::shared_ptr<ast::Float> parseFloat();
    std::shared_ptr<ast::String> parseString();
    std::shared_ptr<ast::Member> parseMember();
    std::shared_ptr<ast::MapDef> parseMapDef();
    std::shared_ptr<ast::Vector> parseVector();
    std::shared_ptr<ast::MapRef> parseMapRef();
    std::shared_ptr<ast::Export> parseExport();
    std::shared_ptr<ast::Literal> parseLiteral();
    std::shared_ptr<ast::Boolean> parseBoolean();
    std::shared_ptr<ast::Integer> parseInteger();
    std::shared_ptr<ast::EnumRef> parseEnumRef();
    std::shared_ptr<ast::Statement> parseStatement();
    std::shared_ptr<ast::ObjectDef> parseObjectDef();
    std::shared_ptr<ast::Parameter> parseParameter();
    std::shared_ptr<ast::ObjectRef> parseObjectRef();
    std::shared_ptr<ast::ObjectIns> parseObjectIns();
    std::shared_ptr<ast::Assignment> parseAssignment();
    std::shared_ptr<ast::Expression> parseExpression();
    std::shared_ptr<ast::Identifier> parseIdentifier();
    std::shared_ptr<ast::TemplateDef> parseTemplateDef();
    std::shared_ptr<ast::TemplateRef> parseTemplateRef();
    std::shared_ptr<ast::TemplateParam> parseTemplateParam();
    std::shared_ptr<ast::Expression> parseExprInParenthese();
    std::shared_ptr<ast::Expression> parsePrimaryExpression();
    std::shared_ptr<ast::Expression> parseOperation(int precedence);
    static int getPrecedence(TokenType type);

public:
    explicit Parser(TokenPtrs tokens);
    std::shared_ptr<ast::Program> parse(bool debug = false);
};

void debug(const std::string& msg, const TokenPtr& token);

#endif  // NDF_PARSER_H
