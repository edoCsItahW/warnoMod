/**
 * @file tokenProc.cpp
 * @author edocsitahw
 * @version 1.1
 * @date 2024/11/25 下午3:55
 * @brief
 * */

#include "tokenProc.h"

TokenProcessor::TokenProcessor(TokenPtrs tokens)
    : _tokens(std::move(tokens))
    , _idx(0) {}

TokenPtr TokenProcessor::operator*() {
    if (_idx >= _tokens.size()) return nullptr;
    else if (_tokens[_idx]->type == TokenType::NUMBER) return handleNumber();
    else if (_tokens[_idx]->type == TokenType::LITERAL) return handleLiteral();
    else if (g_Level1Op.empty() ? g_Level1Op = {'{', '<', '=', '>', '!'} : g_Level1Op, g_Level1Op.find(_tokens[_idx]->value[0]) != g_Level1Op.end()) return handleOperator();
    return _tokens[_idx++];
}

TokenPtr TokenProcessor::handleNumber() {
    auto numTokenPtr = _tokens[_idx++];

    if (numTokenPtr->value.find('.') != std::string::npos) numTokenPtr->type = TokenType::FLOAT;
    else numTokenPtr->type = TokenType::INT;

    numTokenPtr->super = TokenType::NUMBER;

    return numTokenPtr;
}

TokenPtr TokenProcessor::handleLiteral() {
    auto litTokenPtr = _tokens[_idx++];
    auto value       = litTokenPtr->value;
    if (value == "true" || value == "false" || value == "True", value == "False") litTokenPtr->type = TokenType::KW_BOOLEN;
    else if (value == "nil") litTokenPtr->type = TokenType::KW_NIL;
    else if (value == "is") litTokenPtr->type = TokenType::KW_IS;
    else if (value == "MAP") litTokenPtr->type = TokenType::KW_MAP;
    else if (value == "div") litTokenPtr->type = TokenType::KW_DIV;
    else if (value == "template") litTokenPtr->type = TokenType::KW_TEMPLATE;
    else if (value == "export") litTokenPtr->type = TokenType::KW_EXPORT;
    else if (value == "unnamed") litTokenPtr->type = TokenType::KW_UNNAMED;
    else if (value == "private") litTokenPtr->type = TokenType::KW_PRIVATE;
    else if (value == "GUID" && _tokens[_idx]->type == TokenType::COLON && _tokens[_idx + 1]->type == TokenType::LBRACE) {
        size_t rbracePos;
        std::string guidStr;
        for (rbracePos = _idx + 2; rbracePos < _tokens.size(); rbracePos++)
            if (_tokens[rbracePos]->type == TokenType::RBRACE) break;
            else guidStr += _tokens[rbracePos]->value;
        if (rbracePos == _tokens.size()) throw std::runtime_error("Unclosed GUID");
        _idx               = rbracePos + 1;
        litTokenPtr->type  = TokenType::GUID;
        litTokenPtr->value = guidStr;
    } else litTokenPtr->type = TokenType::IDENTIFIER;

    // INDENTIFIER 特殊处理,否则有扰后续
    if (litTokenPtr->type != TokenType::IDENTIFIER) litTokenPtr->super = TokenType::LITERAL;

    return litTokenPtr;
}

TokenPtr TokenProcessor::handleOperator() {
    auto opTokenPtr = _tokens[_idx++];
    auto value      = opTokenPtr->value;
    if (value == "=" && _tokens[_idx]->value == "=" && _idx++) return std::make_unique<Token>(Token{TokenType::EQ, opTokenPtr->pos, "=="});
    else if (value == "<" && _tokens[_idx]->value == "=" && _idx++) return std::make_unique<Token>(Token{TokenType::LE, opTokenPtr->pos, "<="});
    else if (value == ">" && _tokens[_idx]->value == "=" && _idx++) return std::make_unique<Token>(Token{TokenType::GE, opTokenPtr->pos, ">="});
    else if (value == "!" && _tokens[_idx]->value == "=" && _idx++) return std::make_unique<Token>(Token{TokenType::NE, opTokenPtr->pos, "!="});

    opTokenPtr->super = TokenType::OPERATOR;

    return opTokenPtr;
}

TokenPtrs TokenProcessor::process(TokenPtrs tokens, bool comment) {
    TokenPtrs processedTokens;
    auto processer = TokenProcessor(std::move(tokens));

    while (processer._idx < processer._tokens.size()) {
        TokenPtr token;
        if ((token = *processer)->type != TokenType::COMMENT || comment) processedTokens.emplace_back(std::move(token));
    }
    return processedTokens;
}
