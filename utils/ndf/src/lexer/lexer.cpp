// Copyright (c) 2024. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file lexer.cpp
 * @author edocsitahw
 * @version 1.1
 * @date 2024/11/24 上午1:33
 * @brief
 * */
#include "lexer.h"

std::set<char> g_Level1Op;

Lexer::Lexer(std::string source)
    : _source(std::move(source))
    , _pos(1, 0, 0) {}

bool Lexer::inScope() const { return _pos < _source.size(); }

char Lexer::curr() const { return inScope() ? _source[_pos()] : '\0'; }

Token Lexer::operator*() {
    skip();

    if (!inScope())
        return {TokenType::EOF_, _pos};
    else if (curr() == '\n')
        return {TokenType::NEWLINE, _pos.newLine()};
    else if (isdigit(curr()) || (curr() == '.' && isdigit(_source[_pos + 1])))
        return extractNumber();
    else if (isalpha(curr()) || curr() == '_')
        return extractLiteral();
    else if (curr() == '/' && _source[_pos + 1] == '/')
        return extractComment();
    else if (curr() == '/' && _source[_pos + 1] == '*')
        return extractBlockComment();
    else if (curr() == '"' || curr() == '\'')
        return extractString();
    else if ((curr() == '$' || curr() == '~' || curr() == '.') && _source[_pos + 1] == '/')
        return extractPath();
    else if (!isalpha(curr()) && !isdigit(curr())) {
        auto op  = extractOperator();
        op.super = TokenType::OPERATOR;
        return op;
    }
    return {TokenType::UNKNOWN, _pos++};
}

Token Lexer::extractNumber() {
    std::string numStr;
    auto start = _pos;
    while (inScope() && (isdigit(curr()) || curr() == '.')) numStr += _source[_pos++()];
    return {TokenType::NUMBER, start, std::move(numStr)};
}

Token Lexer::extractLiteral() {
    std::string litStr;
    auto start = _pos;
    while (inScope() && (isalpha(curr()) || curr() == '_' || isdigit(curr()))) litStr += _source[_pos++()];
    return {TokenType::LITERAL, start, std::move(litStr)};
}

Token Lexer::extractComment() {
    std::string commentStr;
    auto start = _pos;
    _pos += 2;
    while (inScope() && curr() != '\n') commentStr += _source[_pos++()];
    return {TokenType::COMMENT, start, std::move(commentStr)};
}

Token Lexer::extractBlockComment() {
    std::string blockCommentStr;
    auto start = _pos;
    _pos += 2;
    while (inScope() && !(_source[_pos()] == '*' && _source[_pos() + 1] == '/')) {
        blockCommentStr += _source[_pos++()];
        if (curr() == '\n') _pos.newLine();
    }
    _pos += 2;
    return {TokenType::COMMENT, start, std::move(blockCommentStr)};
}

Token Lexer::extractString() {
    std::string str;
    auto start = _pos;
    auto quote = curr();
    _pos++;
    while (inScope() && curr() != quote) {
        if (curr() == '\\') {
            _pos++;
            if (curr() == 'n')
                str += '\n';
            else if (curr() == 'r')
                str += '\r';
            else if (curr() == 't')
                str += '\t';
            else if (curr() == '\\')
                str += '\\';
            else if (curr() == '\'')
                str += '\'';
            else if (curr() == '\"')
                str += '\"';
            else if (curr() == '0')
                str += '\0';
            else
                str += curr();
        } else
            str += curr();
        _pos++;
    }
    _pos++;
    return {TokenType::STRING, start, std::move(str)};
}

Token Lexer::extractPath() {
    std::string pathStr;
    auto start = _pos;
    pathStr += _source[_pos++()];
    pathStr += _source[_pos++()];
    while (inScope() && (isalpha(curr()) || isdigit(curr()) || curr() == '_' || curr() == '/')) pathStr += _source[_pos++()];
    return {TokenType::PATH, start, std::move(pathStr)};
}

Token Lexer::extractOperator() {
    switch (curr()) {
        case '+': return {TokenType::ADD, _pos++, "+"};
        case '-': return {TokenType::SUB, _pos++, "-"};
        case '*': return {TokenType::MUL, _pos++, "*"};
        case '/': return {TokenType::DIV, _pos++, "/"};
        case '%': return {TokenType::MOD, _pos++, "%"};
        case '=': return {TokenType::ASSIGN, _pos++, "="};
        case '<': return {TokenType::LT, _pos++, "<"};
        case '>': return {TokenType::GT, _pos++, ">"};
        case '|': return {TokenType::OR, _pos++, "|"};
        case '!': return {TokenType::NOT, _pos++, "!"};
        case '(': return {TokenType::LPAREN, _pos++, "("};
        case ')': return {TokenType::RPAREN, _pos++, ")"};
        case '{': return {TokenType::LBRACE, _pos++, "{"};
        case '}': return {TokenType::RBRACE, _pos++, "}"};
        case '[': return {TokenType::LBRACKET, _pos++, "["};
        case ']': return {TokenType::RBRACKET, _pos++, "]"};
        case ',': return {TokenType::COMMA, _pos++, ","};
        case ';': return {TokenType::SEMICOLON, _pos++, ";"};
        case '.': return {TokenType::DOT, _pos++, "."};
        case ':': return {TokenType::COLON, _pos++, ":"};
        case '?': return {TokenType::QUESTION, _pos++, "?"};
        case '~': return {TokenType::TILDE, _pos++, "~"};
        case '$': return {TokenType::DOLLAR, _pos++, "$"};
        default: return {TokenType::UNKNOWN, _pos++};
    }
}

void Lexer::skip() {
    while (inScope() && isspace(curr()) && curr() != '\n') _pos++;
}

TokenPtrs Lexer::tokenize(const std::string& text) {
    Lexer lexer(text);
    TokenPtrs tokens;
    while (lexer.inScope()) {
        auto token = *lexer;
        tokens.emplace_back(std::make_unique<Token>(token));
    }
    return tokens;
}


