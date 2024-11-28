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
 * page bnf BNF描述
 * - [x] <program> ::= <statement>*
 * - [x] <statement> ::= <assignment> | <objectDef> | <mapDef> | <templateDef> | <comment> | <export>
 * - [x] <export> ::= "export" <statement>
 * - [x] <assignment> ::= <identifier> "is" <expression>
 * - [x] <objectDef> ::= <identifier> "is" <type> "(" <member_list>? ")"  // 由于@ref "类型（几乎？）总是以大写字母“T”开头。它们代表游戏的内部数据结构，其定义不可用。"因此,我们将<type>改为<identifier>。
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
 * */

/**
 * @page antlr ANTLR描述
 * grammar MyLanguage;  
 * Program      : Statement* ;  
 * 
 * Statement    : Assignment   
 *              | ObjectDef
 *              | MapDef
 *              | TemplateDef
 *              | Comment
 *              | Export ;
 * 
 * Assignment   : Identifier 'is' Expression ;  
 * 
 * ObjectDef   : Identifier 'is' Identifier '(' Member* ')' ;  
 * 
 * MapDef      : 'MAP' '[' Pair* ']' ;
 *
 * TemplateDef : 'template' Identifier '[' Parameter* ']' 'is' Identifier '(' Member* ')' ;
 *
 * comment      : '//' text
 *              | '*' text '*';
 *
 * Export       : 'export' Statement ;
 *
 * Expression   : Literal
 *              | Identifier
 *              | Operation
 *              | ObjectRef
 *              | MapRef
 *              | TemplateRef
 *              | ObjectIns
 *              | TemplateParam ;
 *
 * Literal      : Boolean
 *              | String
 *              | Integer
 *              | Float
 *              | Vector
 *              | Pair ;
 *
 * Identifier   : LETTER (LETTER | DIGIT)* ;
 *
 * Operation    : Expression Operator Expression ;
 *
 * ObjectRef   : '$' '/' Identifier ;
 *
 * MapRef      : 'MAP' '[' Pair* ']' ;
 *
 * ObjectIns   : Identifier '(' Member* ')' ;
 *
 * TemplateParam : '<' Identifier '>'
 *
 * Pair         : '(' Expression ',' Expression ')' ;
 *
 * parameter    : Identifier [':' Identifier] ['=' Expression] ;
 *
 * Member       : Identifier '=' Expression ;
 *
 * Boolean      : 'true'
 *              | 'false'
 *              | 'True'
 *              | 'False' ;
 *
 * String       : '\'' text '\''
 *              | '\"' text '\"' ;
 *
 * Integer      : DIGIT+ ;
 *
 * float        : DIGIT* '.' DIGIT+ ;
 *
 * vector       : '[' Expression_list? ']' ;
 *
 * Expression_list: Expression (',' Expression)* ;
 *
 * operator     : '+'
 *              | '-'
 *              | '*'
 *              | '/'
 *              | '%'
 *              | 'div'
 *              | '<'
 *              | '>'
 *              | '<='
 *              | '>='
 *              | '=='
 *              | '!='
 *              | '|' ;
 *
 * template_ref : 'template' Identifier ;  
 * 
 * // 词法规则  
 * DIGIT        : [0-9] ;  
 * LETTER       : [a-zA-Z] ;  
 * text         : (LETTER | DIGIT | ' ' | ',' | ';' | ':' | '-' | '_' | '.' | '!' | '?' | '@')* ;
 * */

#include "../lexer/lexer.h"
#include "ast.h"
#include <source_location>
#include <variant>

struct FUE_Kwargs {
    bool firstStop = false;
    bool skipNewLine = true;
    bool debug = false;
};

class Parser {
private:
    TokenPtrs _tokens;
    size_t _idx;
    bool _debug;
    [[nodiscard]] TokenPtr curr() const;
    void skip();
    TokenPtr expect(TokenType type, bool skipNewLine = true, const std::source_location& loc = std::source_location::current());
    bool fromUntilExpect(std::variant<int, size_t> start, std::variant<int, size_t> end, TokenType type, const FUE_Kwargs& kwargs = FUE_Kwargs());
    TokenPtr expect(std::initializer_list<TokenType> types);
    [[nodiscard]] bool inScope() const;

    std::shared_ptr<ast::Statement> parseStatement();

    std::shared_ptr<ast::Assignment> parseAssignment();
    std::shared_ptr<ast::ObjectDef> parseObjectDef();
    std::shared_ptr<ast::MapDef> parseMapDef();
    std::shared_ptr<ast::TemplateDef> parseTemplateDef();

    std::shared_ptr<ast::Pair> parsePair();
    std::shared_ptr<ast::Parameter> parseParameter();
    std::shared_ptr<ast::Member> parseMember();

    std::shared_ptr<ast::Expression> parsePrimaryExpression();
    std::shared_ptr<ast::Expression> parseExpression();
    std::shared_ptr<ast::Literal> parseLiteral();
    std::shared_ptr<ast::Expression> parseOperation(int precedence);
    static int getPrecedence(TokenType type);
    std::shared_ptr<ast::Boolean> parseBoolean();
    std::shared_ptr<ast::String> parseString();
    std::shared_ptr<ast::Integer> parseInteger();
    std::shared_ptr<ast::Float> parseFloat();
    std::shared_ptr<ast::Vector> parseVector();
    std::shared_ptr<ast::Identifier> parseIdentifier();
    std::shared_ptr<ast::ObjectRef> parseObjectRef();
    std::shared_ptr<ast::MapRef> parseMapRef();
    std::shared_ptr<ast::TemplateRef> parseTemplateRef();
    std::shared_ptr<ast::GUID> parseGuid();
    std::shared_ptr<ast::Path> parsePath();
    std::shared_ptr<ast::ObjectIns> parseObjectIns();
    std::shared_ptr<ast::EnumRef> parseEnumRef();
    std::shared_ptr<ast::Export> parseExport();
    std::shared_ptr<ast::Expression> parseExprInParenthese();
    std::shared_ptr<ast::Nil> parseNil();
    std::shared_ptr<ast::TemplateParam> parseTemplateParam();

public:
    explicit Parser(TokenPtrs tokens);
    std::shared_ptr<ast::Program> parse(bool debug = false);
};

void debug(const std::string& msg, const TokenPtr& token);

#endif  // NDF_PARSER_H
