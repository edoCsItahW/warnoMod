// Copyright (c) 2024. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file parser.cpp
 * @author edocsitahw
 * @version 1.1
 * @date 2024/11/24 上午1:35
 * @brief
 * */
#include "parser.h"
#include "../expection.h"
#include <algorithm>  // mscv编译所需

void debug(const std::string &msg, const TokenPtr &token) {
    std::cout << token->toString() << " -- " << msg << std::endl;
}

Parser::Parser(TokenPtrs tokens)
        : _tokens(std::move(tokens)), _idx(0) {}

TokenPtr Parser::curr() const {
    if (_idx < _tokens.size()) return _tokens[_idx];
    return nullptr;
}

bool Parser::inScope() const { return _idx < _tokens.size() && curr()->type != TokenType::EOF_; }

void Parser::skip() {
    while (inScope() && (curr()->type == TokenType::NEWLINE || curr()->type == TokenType::COMMENT)) _idx++;
    _idx--;
}

TokenPtr Parser::expect(enum TokenType type, bool skipNewLine, const std::source_location &loc) {
    if (skipNewLine && type != TokenType::NEWLINE) skip();

    if (inScope() && curr()->type == type) return _tokens[_idx++];
    else
        throw SyntaxError(std::format("Expected [{}] but got '{}' line {} in <{}>", enumToStr(type), curr()->toString(),
                                      loc.line(), loc.function_name()));
}

bool Parser::fromUntilExpect(std::variant<int, size_t> _start, std::variant<int, size_t> _end, enum TokenType type,
                             const FUEKwargs &kwargs) {
    int start = std::holds_alternative<int>(_start) ? std::get<int>(_start) : static_cast<int>(std::get<size_t>(
            _start));
    int end = std::holds_alternative<int>(_end) ? std::get<int>(_end) : static_cast<int>(std::get<size_t>(_end));

    for (int i = start; i < end; i++) {
        if (kwargs.debug) std::cout << "check: " << _tokens[i]->toString() << std::endl;
        if (_tokens[i]->type == type) return true;
        else if (kwargs.skipNewLine && type != TokenType::NEWLINE && _tokens[i]->type == TokenType::NEWLINE) continue;
        else if (kwargs.firstStop) return false;
    }

    return false;
}

std::shared_ptr<ast::Program> Parser::parse(bool dbg) {
    _debug = dbg;
    if (_debug) debug("Parse", curr());
    dbg::Debugger dbgParserStatement(&Parser::parseStatement, true);
    // <program> ::= <statement>*

    auto program = std::make_shared<ast::Program>();

    program->_pos = curr()->pos();

    while (inScope()) {
        skip();

        auto stmt = dbgParserStatement(this);
        //        auto stmt = parseStatement();

        if (stmt) program->statements.push_back(stmt);

        _idx++;
    }
    return program;
}

std::shared_ptr<ast::Statement> Parser::parseStatement() {
    if (_debug) debug("ParseStatement", curr());
    dbg::Debugger dbgParseObjDef(&Parser::parseObjectDef);
    dbg::Debugger dbgParseAssign(&Parser::parseAssignment);
    dbg::Debugger dbgParseMapDef(&Parser::parseMapDef);
    dbg::Debugger dbgParseTempDef(&Parser::parseTemplateDef);
    dbg::Debugger dbgParseExport(&Parser::parseExport);
    dbg::Debugger dbgParsePrivate(&Parser::parsePrivate);
    // <statement> ::= <assignment> | <object_def> | <map_def> | <template_def> | <comment>  // comment已在TokenProcessor中剔除

    if (!curr()) return nullptr;

    switch (curr()->type) {
        case TokenType::IDENTIFIER: {
            if (_tokens[_idx + 1]->type == TokenType::KW_IS)  // 都应有is关键字
                if (_tokens[_idx + 2]->type == TokenType::IDENTIFIER) {
                    // 由于这里可以换行,会出现NEWLINE,所以_tokens[_idx + 3]不一定是LPAREN.需要向下检测,直到遇到RPAREN,
                    // 并且要求在找到RPAREN之前只能出现NEWLINE,因为排除<Template_def>的情况,IDENTIFIER is <Token>只有Assignment或者ObjectDef两种情况,所以其余我们抛出SyntaxError
                    if (fromUntilExpect(_idx + 3, _tokens.size(), TokenType::LPAREN, {.firstStop = true}))
                        return dbgParseObjDef(this);
                    else return dbgParseAssign(this);
                } else return dbgParseAssign(this);

            else {
                warn(std::format("Unexpected token: '{}' line {} in {}", enumToStr(curr()->type), __LINE__, __func__),
                     SyntaxWarning);
                return nullptr;
            }
        }
        case TokenType::KW_EXPORT:
            return dbgParseExport(this);

        case TokenType::KW_PRIVATE:
            return dbgParsePrivate(this);

        case TokenType::KW_MAP:
            return dbgParseMapDef(this);

        case TokenType::KW_TEMPLATE:
            return dbgParseTempDef(this);

        default:
            throw SyntaxError(
                    std::format("Unexpected token: '{}' line {} in {}", curr()->toString(), __LINE__, __func__));
    }
}

std::shared_ptr<ast::Assignment> Parser::parseAssignment() {
    if (_debug) debug("ParseAssignment", curr());
    dbg::Debugger dbgParseExpr(&Parser::parseExpression);
    // <assignment> ::= <identifier> "is" <expression>

    auto assign = std::make_shared<ast::Assignment>();

    assign->_pos = curr()->pos();

    assign->identifier = std::make_shared<ast::Identifier>();

    assign->identifier->name = expect(TokenType::IDENTIFIER)->value;

    expect(TokenType::KW_IS);

    assign->expression = dbgParseExpr(this);

    return assign;
}

std::shared_ptr<ast::ObjectDef> Parser::parseObjectDef() {
    if (_debug) debug("ParseObjectDef", curr());
    dbg::Debugger dbgParseMember(&Parser::parseMember);
    // <object_def> ::= <identifier> "is" <type> "(" <member_list>? ")"

    auto obj = std::make_shared<ast::ObjectDef>();

    obj->_pos = curr()->pos();

    obj->identifier = std::make_shared<ast::Identifier>();

    obj->identifier->name = expect(TokenType::IDENTIFIER)->value;

    expect(TokenType::KW_IS);

    obj->type = std::make_shared<ast::Identifier>();

    obj->type->name = expect(TokenType::IDENTIFIER)->value;

    expect(TokenType::LPAREN);

    while (inScope() && curr()->type != TokenType::RPAREN) {
        obj->members.push_back(dbgParseMember(this));

        if (curr()->type == TokenType::RPAREN) break;
        else expect(TokenType::NEWLINE, false);
    }

    expect(TokenType::RPAREN);

    return obj;
}

std::shared_ptr<ast::MapDef> Parser::parseMapDef() {
    if (_debug) debug("ParseMapDef", curr());
    dbg::Debugger dbgParsePair(&Parser::parsePair);
    // <map_def> ::= "MAP" "[" <pair_list>? "]"

    auto map = std::make_shared<ast::MapDef>();

    map->_pos = curr()->pos();

    expect(TokenType::KW_MAP);

    expect(TokenType::LBRACKET);

    while (inScope() && curr()->type != TokenType::RBRACKET) map->pairs.push_back(dbgParsePair(this));

    expect(TokenType::RBRACKET);

    return map;
}

std::shared_ptr<ast::TemplateDef> Parser::parseTemplateDef() {
    if (_debug) debug("ParseTemplateDef", curr());
    dbg::Debugger dbgParseParam(&Parser::parseParameter, true);
    dbg::Debugger dbgParseMember(&Parser::parseMember);
    // <template_def> ::= "template" <identifier> "[" <parameter_list>? "]" "is" <type> "(" <member_list>? ")"

    auto temp = std::make_shared<ast::TemplateDef>();

    temp->_pos = curr()->pos();

    expect(TokenType::KW_TEMPLATE);

    temp->identifier = std::make_shared<ast::Identifier>();

    temp->identifier->name = expect(TokenType::IDENTIFIER)->value;

    expect(TokenType::LBRACKET);

    while (inScope() && curr()->type != TokenType::RBRACKET) {
        temp->parameters.push_back(dbgParseParam(this));
        if (curr()->type == TokenType::RBRACKET) break;
        else expect(TokenType::COMMA, false);
        skip();
    }

    expect(TokenType::RBRACKET);

    expect(TokenType::KW_IS);

    temp->type = std::make_shared<ast::Identifier>();

    temp->type->name = expect(TokenType::IDENTIFIER)->value;

    expect(TokenType::LPAREN);

    while (inScope() && curr()->type != TokenType::RPAREN) {
        temp->members.push_back(dbgParseMember(this));
        if (curr()->type == TokenType::RPAREN) break;
        else expect(TokenType::NEWLINE, false);
    }

    expect(TokenType::RPAREN);

    return temp;
}

std::shared_ptr<ast::Export> Parser::parseExport() {
    if (_debug) debug("ParseExport", curr());
    dbg::Debugger dbgParseStatement(&Parser::parseStatement);
    // <export> ::= "export" <statement>

    auto export_ = std::make_shared<ast::Export>();

    export_->_pos = curr()->pos();

    expect(TokenType::KW_EXPORT);

    export_->statement = dbgParseStatement(this);

    return export_;
}

std::shared_ptr<ast::Private> Parser::parsePrivate() {
    if (_debug) debug("ParsePrivate", curr());
    dbg::Debugger dbgParseStatement(&Parser::parseStatement);
    // <private> ::= "private" <identifier>

    auto private_ = std::make_shared<ast::Private>();

    private_->_pos = curr()->pos();

    expect(TokenType::KW_PRIVATE);

    private_->statement = dbgParseStatement(this);

    return private_;
}

std::shared_ptr<ast::Expression> Parser::parsePrimaryExpression() {
    if (_debug) debug("ParsePrimaryExpression", curr());
    dbg::Debugger dbgParseExpr(&Parser::parseExpression);
    dbg::Debugger dbgParseLiteral(&Parser::parseLiteral, true);
    dbg::Debugger dbgParseIdentifier(&Parser::parseIdentifier);
    dbg::Debugger dbgParseObjRef(&Parser::parseObjectRef);
    dbg::Debugger dbgParseMapRef(&Parser::parseMapRef);
    dbg::Debugger dbgParseTempRef(&Parser::parseTemplateRef);
    dbg::Debugger dbgParsePath(&Parser::parsePath);
    dbg::Debugger dbgParseObjIns(&Parser::parseObjectIns);
    dbg::Debugger dbgParseExprInParen(&Parser::parseExprInParenthese);
    dbg::Debugger dbgParseEnumRef(&Parser::parseEnumRef);
    dbg::Debugger dbgParseTempParam(&Parser::parseTemplateParam);
    // <expression> ::= <literal> | <identifier> | <operation> | <object_ref> | <map_ref> | <template_ref>

    skip();

    if (curr()->type == TokenType::LPAREN)  // 处理括号表达式
        // 由于歧义,这里特殊处理
        return dbgParseExprInParen(this);
    else if (curr()->type == TokenType::KW_MAP) return dbgParseMapRef(this);
    else if (curr()->type == TokenType::KW_TEMPLATE) return dbgParseTempRef(this);
    else if (curr()->super == TokenType::LITERAL || curr()->super == TokenType::NUMBER || curr()->type == TokenType::STRING || curr()->type == TokenType::LBRACKET)
        return dbgParseLiteral(this);
    else if (curr()->type == TokenType::IDENTIFIER)
        if (_tokens[_idx + 1]->type == TokenType::DIV) return dbgParseEnumRef(this);
        else if (fromUntilExpect(_idx + 1, _tokens.size(), TokenType::LPAREN, {.firstStop = true}))
            return dbgParseObjIns(this);
        else return dbgParseIdentifier(this);
    else if (curr()->type == TokenType::DOLLAR) return dbgParseObjRef(this);
    else if (curr()->type == TokenType::PATH) return dbgParsePath(this);
    else if (curr()->type == TokenType::LT) return dbgParseTempParam(this);
    else throw SyntaxError(std::format("Unexpected token: '{}' line {} in {}", curr()->toString(), __LINE__, __func__));
}

std::shared_ptr<ast::Expression> Parser::parseExprInParenthese() {
    if (_debug) debug("ParseExprInParen", curr());
    dbg::Debugger dbgParseExpr(&Parser::parseExpression);
    dbg::Debugger dbgParsePair(&Parser::parsePair);
    // 由于Expression可以被括号包裹,且Pair也是由括号包裹的,最重要的是Pair := (Expression, Expression),所以有语法歧义,
    // (2 * (1 + 3 * (1, 2)), 1 * (1, 2)) => [(3, 7), (9, 13), (0, 14)]

    std::stack<TokenPtr> stack;    // 用于记录括号
    int loyout = 0, commaNum = 0;  // 用于记录括号内的逗号数量和括号嵌套层数
    for (size_t i = _idx; i < _tokens.size(); i++)
        if (_tokens[i]->type == TokenType::LPAREN) {  // 遇到左括号,入栈
            loyout++;
            stack.push(_tokens[i]);
        }
        else if (_tokens[i]->type == TokenType::RPAREN) {  // 遇到右括号,出栈
            stack.pop();
            loyout--;
            if (stack.empty()) break;  // 栈为空,说明括号已经匹配完毕(由于是部分遍历,将栈空的情况视为正常的括号一一对应完毕)
        }
        else if (_tokens[i]->type == TokenType::COMMA && loyout == 1) commaNum++;

    if (commaNum == 1) return dbgParsePair(this);
    else return dbgParseExpr(this);
}

std::shared_ptr<ast::Expression> Parser::parseExpression() {
    if (_debug) debug("ParseExpression", curr());
    dbg::Debugger dbgParseOperation(&Parser::parseOperation);
    // <expression> ::= <literal> | <identifier> | <operation> | <object_ref> | <map_ref> | <template_ref>

    return dbgParseOperation(this, 0);
}

std::shared_ptr<ast::Literal> Parser::parseLiteral() {
    if (_debug) debug("ParseLiteral", curr());
    dbg::Debugger dbgParseBoolean(&Parser::parseBoolean);
    dbg::Debugger dbgParseGuid(&Parser::parseGuid);
    dbg::Debugger dbgParseString(&Parser::parseString);
    dbg::Debugger dbgParseInteger(&Parser::parseInteger);
    dbg::Debugger dbgParseFloat(&Parser::parseFloat);
    dbg::Debugger dbgParseVector(&Parser::parseVector);
    dbg::Debugger dbgParsePair(&Parser::parsePair);
    dbg::Debugger dbgParseNil(&Parser::parseNil);
    // <literal> ::= <boolean> | <string> | <integer> | <float> | <vector> | <pair>

    if (curr()->super == TokenType::LITERAL && curr()->type == TokenType::KW_BOOLEN) return dbgParseBoolean(this);
    else if (curr()->type == TokenType::KW_NIL) return dbgParseNil(this);
    else if (curr()->type == TokenType::GUID) return dbgParseGuid(this);
    else if (curr()->type == TokenType::STRING) return dbgParseString(this);
    else if (curr()->super == TokenType::NUMBER) {
        if (curr()->type == TokenType::INT) return dbgParseInteger(this);
        else if (curr()->type == TokenType::FLOAT) return dbgParseFloat(this);
        else
            throw SyntaxError(
                    std::format("Unexpected token: '{}' line {} in {}", curr()->toString(), __LINE__, __func__));
    } else if (curr()->type == TokenType::LBRACKET) return dbgParseVector(this);
    else if (curr()->type == TokenType::LPAREN) return dbgParsePair(this);
    else throw SyntaxError(std::format("Unexpected token: '{}' line {} in {}", curr()->toString(), __LINE__, __func__));
}

std::shared_ptr<ast::TemplateParam> Parser::parseTemplateParam() {
    if (_debug) debug("ParseTemplateParam", curr());
    // <template_param> ::= '<' <identifier> '>'

    auto param = std::make_shared<ast::TemplateParam>();

    param->_pos = curr()->pos();

    expect(TokenType::LT);

    param->identifier = std::make_shared<ast::Identifier>();

    param->identifier->name = expect(TokenType::IDENTIFIER)->value;

    expect(TokenType::GT);

    return param;
}

std::shared_ptr<ast::Expression> Parser::parseOperation(int precedence) {
    if (_debug) debug("ParseOperation", curr());
    dbg::Debugger dbgParsePrimaryExpr(&Parser::parsePrimaryExpression, false);  // 如果造成无限递归,将立即停止设为true
    dbg::Debugger dbgParseOperation(&Parser::parseOperation);
    // <operation> ::= <expression> <operator> <expression>

    auto left = dbgParsePrimaryExpr(this);
    while (inScope() && curr()->super == TokenType::OPERATOR && precedence < getPrecedence(curr()->type)) {
        auto op = std::make_shared<ast::Operator>();
        op->value = curr()->value;
        _idx++;
        auto right = dbgParseOperation(this, getPrecedence(curr()->type));

        auto opt = std::make_shared<ast::Operation>();

        opt->_pos = curr()->pos();

        opt->left = left;
        opt->right = right;
        opt->operator_ = op;

        left = opt;
    }
    return left;
}

std::shared_ptr<ast::Pair> Parser::parsePair() {
    if (_debug) debug("ParsePair", curr());
    dbg::Debugger dbgParseExpr(&Parser::parseExpression);
    // <pair> ::= "(" <expression> "," <expression> ")"

    auto pair = std::make_shared<ast::Pair>();

    pair->_pos = curr()->pos();

    expect(TokenType::LPAREN);

    pair->first = dbgParseExpr(this);

    expect(TokenType::COMMA);

    pair->second = dbgParseExpr(this);

    expect(TokenType::RPAREN);

    return pair;
}

std::shared_ptr<ast::Parameter> Parser::parseParameter() {
    if (_debug) debug("ParseParameter", curr());
    dbg::Debugger dbgParseExpr(&Parser::parseExpression);
    // <parameter> ::= <identifier> [":" <type>] ["=" <expression>]

    auto param = std::make_shared<ast::Parameter>();

    param->_pos = curr()->pos();

    param->identifier = std::make_shared<ast::Identifier>();

    param->identifier->name = expect(TokenType::IDENTIFIER)->value;

    if (inScope() && curr()->type == TokenType::COLON) {
        expect(TokenType::COLON);
        param->type = std::make_shared<ast::Identifier>();

        param->type->name = expect(TokenType::IDENTIFIER)->value;
    }

    if (inScope() && curr()->type == TokenType::ASSIGN) {
        expect(TokenType::ASSIGN);
        param->expression = dbgParseExpr(this);
    }

    return param;
}

std::shared_ptr<ast::Member> Parser::parseMember() {
    if (_debug) debug("ParseMember", curr());
    dbg::Debugger dbgParseExpr(&Parser::parseExpression);
    // <identifier> "=" <expression>

    auto member = std::make_shared<ast::Member>();

    member->_pos = curr()->pos();

    member->identifier = std::make_shared<ast::Identifier>();

    member->identifier->name = expect(TokenType::IDENTIFIER)->value;

    expect(TokenType::ASSIGN);

    member->expression = dbgParseExpr(this);

    return member;
}

std::shared_ptr<ast::Boolean> Parser::parseBoolean() {
    if (_debug) debug("ParseBoolean", curr());
    // <boolean> ::= "true" | "false"
    auto boolean = std::make_shared<ast::Boolean>();

    boolean->_pos = curr()->pos();

    boolean->value = (curr()->value == "true" || curr()->value == "True");

    expect(TokenType::KW_BOOLEN);

    return boolean;
}

std::shared_ptr<ast::String> Parser::parseString() {
    if (_debug) debug("ParseString", curr());
    // <string> ::= "'" <text> "'" | "\"" <text> "\""
    auto string = std::make_shared<ast::String>();

    string->_pos = curr()->pos();

    string->value = expect(TokenType::STRING)->value;

    return string;
}

std::shared_ptr<ast::Integer> Parser::parseInteger() {
    if (_debug) debug("ParseInteger", curr());
    // <integer> ::= <digit>+
    auto integer = std::make_shared<ast::Integer>();

    integer->_pos = curr()->pos();

    integer->value = std::stoi(expect(TokenType::INT)->value);

    return integer;
}

std::shared_ptr<ast::Float> Parser::parseFloat() {
    if (_debug) debug("ParseFloat", curr());
    // <float> ::= <digit>* "." <digit>+
    auto float_ = std::make_shared<ast::Float>();

    float_->_pos = curr()->pos();

    float_->value = std::stof(expect(TokenType::FLOAT)->value);
    
    return float_;
}

std::shared_ptr<ast::Vector> Parser::parseVector() {
    if (_debug) debug("ParseVector", curr());
    dbg::Debugger bdgParseExpr(&Parser::parseExpression);
    // <vector> ::= "[" <expression_list>? "]"

    auto vector = std::make_shared<ast::Vector>();

    vector->_pos = curr()->pos();

    expect(TokenType::LBRACKET);

    while (inScope() && curr()->type != TokenType::RBRACKET) {
        vector->expressions.push_back(bdgParseExpr(this));
        if (curr()->type == TokenType::RBRACKET) break;
        else expect(TokenType::COMMA);
        skip();
    }

    expect(TokenType::RBRACKET);

    return vector;
}

std::shared_ptr<ast::Identifier> Parser::parseIdentifier() {
    if (_debug) debug("ParseIdentifier", curr());
    // <identifier> ::= <letter> (<letter> | <digit>)*

    auto identifier = std::make_shared<ast::Identifier>();

    identifier->_pos = curr()->pos();

    identifier->name = expect(TokenType::IDENTIFIER)->value;

    return identifier;
}

std::shared_ptr<ast::ObjectRef> Parser::parseObjectRef() {
    if (_debug) debug("ParseObjectRef", curr());
    // <object_ref> ::= "$" "/" <identifier>
    auto objRef = std::make_shared<ast::ObjectRef>();

    objRef->_pos = curr()->pos();

    expect(TokenType::DOLLAR);

    expect(TokenType::DIV);

    objRef->identifier = std::make_shared<ast::Identifier>();

    objRef->identifier->name = expect(TokenType::IDENTIFIER)->value;

    return objRef;
}

std::shared_ptr<ast::MapRef> Parser::parseMapRef() {
    if (_debug) debug("ParseMapRef", curr());
    dbg::Debugger dbgParsePair(&Parser::parsePair);
    // <map_ref> ::= "MAP[" <pair_list> "]"

    auto mapRef = std::make_shared<ast::MapRef>();
    mapRef->_pos = curr()->pos();

    expect(TokenType::KW_MAP);

    expect(TokenType::LBRACKET);

    while (inScope() && curr()->type != TokenType::RBRACKET) {
        mapRef->pairs.push_back(dbgParsePair(this));
        skip();

        if (curr()->type == TokenType::RBRACKET) break;
        else expect(TokenType::COMMA);
        skip();
    }

    expect(TokenType::RBRACKET);

    return mapRef;
}

std::shared_ptr<ast::TemplateRef> Parser::parseTemplateRef() {
    if (_debug) debug("ParseTemplateRef", curr());
    // <template_ref> ::= "template" <identifier>
    auto tempRef = std::make_shared<ast::TemplateRef>();
    tempRef->_pos = curr()->pos();
    expect(TokenType::KW_TEMPLATE);

    tempRef->identifier = std::make_shared<ast::Identifier>();

    tempRef->identifier->name = expect(TokenType::IDENTIFIER)->value;

    return tempRef;
}

std::shared_ptr<ast::GUID> Parser::parseGuid() {
    if (_debug) debug("ParseGuid", curr());
    // <guid> ::= <hex_digit>+
    auto guid = std::make_shared<ast::GUID>();
    guid->_pos = curr()->pos();
    guid->value = expect(TokenType::GUID)->value;
    return guid;
}

std::shared_ptr<ast::Path> Parser::parsePath() {
    if (_debug) debug("ParsePath", curr());
    // <path> ::= "$" "/" <identifier> ("/" <identifier>)* | "~" "/" <identifier> ("/" <identifier>)* | "." "/" <identifier> ("/" <identifier>)*
    auto path = std::make_shared<ast::Path>();
    path->_pos = curr()->pos();
    path->value = expect(TokenType::PATH)->value;
    return path;
}

std::shared_ptr<ast::ObjectIns> Parser::parseObjectIns() {
    if (_debug) debug("ParseObjectIns", curr());
    dbg::Debugger dbgParseMember(&Parser::parseMember);
    // <object_ins> ::= <identifier> "(" <member_list> ")"
    auto objIns = std::make_shared<ast::ObjectIns>();

    objIns->_pos = curr()->pos();

    objIns->identifier = std::make_shared<ast::Identifier>();

    objIns->identifier->name = expect(TokenType::IDENTIFIER)->value;

    expect(TokenType::LPAREN);

    while (inScope() && curr()->type != TokenType::RPAREN) {
        objIns->members.push_back(dbgParseMember(this));
//        if (curr()->type == TokenType::RPAREN) break;
        if (fromUntilExpect(_idx, _tokens.size(), TokenType::RPAREN, {.firstStop = true})) break;
        else expect(TokenType::NEWLINE, false);
    }

    expect(TokenType::RPAREN);

    return objIns;
}

std::shared_ptr<ast::EnumRef> Parser::parseEnumRef() {
    if (_debug) debug("ParseEnumRef", curr());
    // <enum_ref> ::= <identifier> '/' <identifier>
    auto enumRef = std::make_shared<ast::EnumRef>();

    enumRef->_pos = curr()->pos();

    enumRef->enumName = std::make_shared<ast::Identifier>();

    enumRef->enumName->name = expect(TokenType::IDENTIFIER)->value;

    expect(TokenType::DIV);

    enumRef->enumValue = std::make_shared<ast::Identifier>();

    enumRef->enumValue->name = expect(TokenType::IDENTIFIER)->value;

    return enumRef;
}

std::shared_ptr<ast::Nil> Parser::parseNil() {
    if (_debug) debug("ParseNil", curr());
    // <nil> ::= "nil"
    auto nil = std::make_shared<ast::Nil>();
    nil->_pos = curr()->pos();
    _idx++;
    return nil;
}

int Parser::getPrecedence(enum TokenType type) {
    switch (type) {
        case TokenType::ADD:
        case TokenType::SUB:
            return 1;
        case TokenType::MUL:
        case TokenType::DIV:
            return 2;
        case TokenType::MOD:
            return 3;
        case TokenType::OR:
            return 5;
        case TokenType::EQ:
        case TokenType::NE:
            return 7;
        case TokenType::GT:
        case TokenType::GE:
        case TokenType::LT:
        case TokenType::LE:
            return 8;
        default:
            return 0;
    }
}
