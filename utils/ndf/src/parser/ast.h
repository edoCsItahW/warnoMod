// Copyright (c) 2024. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file ast.h
 * @author edocsitahw
 * @version 1.1
 * @date 2024/11/25 下午3:47
 * @brief
 * */
#ifndef NDF_AST_H
#define NDF_AST_H
#pragma once

#include <iostream>
#include <memory>
#include <vector>

namespace ast {
    class Statement;
    class Pair;
    class Parameter;
    class Member;
    class Expression;
    class Literal;
    class Identifier;
    class Operator;

    struct AST {
        const char* nodeName = "AST";
    };

    /** @struct Program
     *
     * @if zh
     * @brief AST节点Program
     * @details Program节点表示整个程序,包含多个Statement节点.
     * @remark Program应该是整个AST的根节点.
     * @note 这是一个具体的节点,其包括有子节点.
     *
     * @else
     * @brief AST node Program
     * @details The Program node represents the entire program, which contains multiple Statement nodes.
     * @remark The Program node should be the root node of the AST.
     * @note This is a concrete node that includes child nodes.
     * @endif
     *
     * @code{.antlr}
     * Program : Statement* ;
     * @endcode
     *
     * @see AST
     * */
    struct Program : public AST {
        int indent           = 0;
        const char* nodeName = "Program";
        std::vector<std::shared_ptr<Statement>> statements;
        [[nodiscard]] std::string toJson() const;
        [[nodiscard]] std::string toString() const;
    };

    /** @struct Statement
     *
     * @if zh
     * @brief AST节点Statement
     * @details Statement节点表示程序中的语句,包含多个Expression节点.
     * @note 这是一个抽象的节点,其不包括子节点.
     *
     * @else
     * @brief AST node Statement
     * @details The Statement node represents a statement in the program, which contains multiple Expression nodes.
     * @note This is an abstract node that does not include child nodes.
     * @endif
     *
     * @code{.antlr}
     * // zh: 实际的AST中注释语句被剔除了(在词法分析中)
     * // en: The comment statement is removed in the actual AST (during lexical analysis)
     * Statement : Assignment
     *           | ObjectDef
     *           | MapDef
     *           | TemplateDef
     *           | Comment ;
     * @endcode
     *
     * @see AST
     * */
    struct Statement : public AST {
        int indent                                         = 0;
        const char* nodeName                               = "Statement";
        [[nodiscard]] virtual std::string toJson() const   = 0;
        [[nodiscard]] virtual std::string toString() const = 0;
    };

    /** @struct Assignment
     *
     * @if zh
     * @brief AST节点Assignment
     * @details Assignment节点表示赋值语句,包含一个Identifier节点和一个Expression节点.
     * @note 这是一个具体的节点,其包括有子节点.
     *
     * @else
     * @brief AST node Assignment
     * @details The Assignment node represents an assignment statement, which contains an Identifier node and an Expression node.
     * @note This is a concrete node that includes child nodes.
     * @endif
     *
     * @code{.antlr}
     * Assignment : Identifier 'is' Expression ;
     * @endcode
     *
     * @see Statement
     * */
    struct Assignment : public Statement {
        int indent           = 0;
        const char* nodeName = "Assignment";
        std::shared_ptr<Identifier> identifier;
        std::shared_ptr<Expression> expression;
        [[nodiscard]] std::string toJson() const override;
        [[nodiscard]] std::string toString() const override;
    };

    /** @struct ObjectDef
     *
     * @if zh
     * @brief AST节点ObjectDef
     * @details ObjectDef节点表示对象定义语句,包含一个Identifier节点,一个Identifier节点,一个Member节点的列表.
     * @note 这是一个具体的节点,其包括有子节点.
     * @warning
     * 注意: 由于@c "类型(几乎?)总是以大写字母"T"开头.它们代表游戏的内部数据结构,其定义不可用." 因此,我们将<type>改为<identifier>。
     *
     * @else
     * @brief AST node ObjectDef
     * @details The ObjectDef node represents an object definition statement, which contains an Identifier node, an Identifier node, and a list of Member nodes.
     * @note This is a concrete node that includes child nodes.
     * @warning
     * Note: Since `"types (almost?) always start with a capital letter "T", they represent internal data structures of the game, which cannot be defined."` Therefore, we changed \<type> to
     * \<identifier>.
     * @endif
     *
     * @code{.antlr}
     * ObjectDef : Identifier 'is' Identifier '(' Member* ')' ;
     * @endcode
     *
     * @see Statement
     * */
    struct ObjectDef : public Statement {
        int indent           = 0;
        const char* nodeName = "ObjectDef";
        std::shared_ptr<Identifier> identifier;
        std::shared_ptr<Identifier> type;
        std::vector<std::shared_ptr<Member>> memberList;
        [[nodiscard]] std::string toJson() const override;
        [[nodiscard]] std::string toString() const override;
    };

    /** @struct MapDef
     *
     * @if zh
     * @brief AST节点MapDef
     * @details MapDef节点表示映射定义语句,包含一个Pair节点的列表.
     * @note 这是一个具体的节点,其包括有子节点.
     *
     * @else
     * @brief AST node MapDef
     * @details The MapDef node represents a map definition statement, which contains a list of Pair nodes.
     * @note This is a concrete node that includes child nodes.
     * @endif
     *
     * @code{.antlr}
     * MapDef : 'MAP' '[' Pair* ']' ;
     * @endcode
     *
     * @see Statement
     * */
    struct MapDef : public Statement {
        int indent           = 0;
        const char* nodeName = "MapDef";
        std::vector<std::shared_ptr<Pair>> pairList;
        [[nodiscard]] std::string toJson() const override;
        [[nodiscard]] std::string toString() const override;
    };

    /** @struct TemplateDef
     *
     * @if zh
     * @brief AST节点TemplateDef
     * @details TemplateDef节点表示模板定义语句,包含一个Identifier节点,一个Parameter节点的列表,一个Identifier节点,一个Member节点的列表.
     * @note 这是一个具体的节点,其包括有子节点.
     *
     * @else
     * @brief AST node TemplateDef
     * @details The TemplateDef node represents a template definition statement, which contains an Identifier node, a list of Parameter nodes, an Identifier node, and a list of Member nodes.
     * @note This is a concrete node that includes child nodes.
     * @endif
     *
     * @code{.antlr}
     * TemplateDef : 'template' Identifier '[' Parameter* ']' 'is' Identifier '(' Member* ')' ;
     * @endcode
     *
     * @see Statement
     * */
    struct TemplateDef : public Statement {
        int indent           = 0;
        const char* nodeName = "TemplateDef";
        std::shared_ptr<Identifier> identifier;
        std::vector<std::shared_ptr<Parameter>> parameterList;
        std::shared_ptr<Identifier> type;
        std::vector<std::shared_ptr<Member>> memberList;
        [[nodiscard]] std::string toJson() const override;
        [[nodiscard]] std::string toString() const override;
    };

    struct Export : public Statement {
        int indent           = 0;
        const char* nodeName = "Export";
        std::shared_ptr<Statement> statement;
        [[nodiscard]] std::string toJson() const override;
        [[nodiscard]] std::string toString() const override;
    };

    /** @struct Parameter
     *
     * @if zh
     * @brief AST节点Parameter
     * @details Parameter节点表示模板参数,包含一个Identifier节点,一个Identifier节点,一个Expression节点.
     * @note 这是一个具体的节点,其包括有子节点.
     *
     * @else
     * @brief AST node Parameter
     * @details The Parameter node represents a template parameter, which contains an Identifier node, an Identifier node, and an Expression node.
     * @note This is a concrete node that includes child nodes.
     * @endif
     *
     * @code{.antlr}
     * Parameter    : Identifier [':' Identifier] ['=' Expression] ;
     * @endcode
     *
     * @see AST
     * */
    struct Parameter : public AST {
        int indent           = 0;
        const char* nodeName = "Parameter";
        std::shared_ptr<Identifier> identifier;
        std::shared_ptr<Identifier> type;
        std::shared_ptr<Expression> expression;
        [[nodiscard]] std::string toJson() const;
        [[nodiscard]] std::string toString() const;
    };

    struct Member : public AST {
        int indent           = 0;
        const char* nodeName = "Member";
        std::shared_ptr<Identifier> identifier;
        std::shared_ptr<Expression> expression;
        [[nodiscard]] std::string toJson() const;
        [[nodiscard]] std::string toString() const;
    };

    struct Expression : public AST {
        int indent                                         = 0;
        const char* nodeName                               = "Expression";
        [[nodiscard]] virtual std::string toJson() const   = 0;
        [[nodiscard]] virtual std::string toString() const = 0;
    };

    struct Literal : public Expression {
        int indent           = 0;
        const char* nodeName = "Literal";
    };

    struct GUID : public Literal {
        int indent           = 0;
        const char* nodeName = "GUID";
        std::string value;
        explicit GUID(std::string value);
        [[nodiscard]] std::string toJson() const override;
        [[nodiscard]] std::string toString() const override;
    };

    struct Path : public Literal {
        int indent           = 0;
        const char* nodeName = "Path";
        std::string value;
        explicit Path(std::string value);
        [[nodiscard]] std::string toJson() const override;
        [[nodiscard]] std::string toString() const override;
    };

    struct Pair : public Literal {
        int indent           = 0;
        const char* nodeName = "Pair";
        std::shared_ptr<Expression> first;
        std::shared_ptr<Expression> second;
        [[nodiscard]] std::string toJson() const override;
        [[nodiscard]] std::string toString() const override;
    };

    struct Boolean : public Literal {
        int indent = 0;
        bool value;
        const char* nodeName = "Boolean";
        [[nodiscard]] std::string toJson() const override;
        [[nodiscard]] std::string toString() const override;
    };

    struct String : public Literal {
        int indent           = 0;
        const char* nodeName = "String";
        std::string value;
        [[nodiscard]] std::string toJson() const override;
        [[nodiscard]] std::string toString() const override;
    };

    struct Integer : public Literal {
        int indent           = 0;
        const char* nodeName = "Integer";
        int value;
        [[nodiscard]] std::string toJson() const override;
        [[nodiscard]] std::string toString() const override;
    };

    struct Float : public Literal {
        int indent           = 0;
        const char* nodeName = "Float";
        double value;
        [[nodiscard]] std::string toJson() const override;
        [[nodiscard]] std::string toString() const override;
    };

    struct Nil : public Literal {
        int indent           = 0;
        const char* nodeName = "Nil";
        std::string value    = "nil";
        [[nodiscard]] std::string toJson() const override;
        [[nodiscard]] std::string toString() const override;
    };

    struct TemplateParam : public Expression {
        int indent           = 0;
        const char* nodeName = "TemplateParam";
        std::shared_ptr<Identifier> identifier;
        [[nodiscard]] std::string toJson() const override;
        [[nodiscard]] std::string toString() const override;
    };

    struct Vector : public Literal {
        int indent           = 0;
        const char* nodeName = "Vector";
        std::vector<std::shared_ptr<Expression>> expressionList;
        [[nodiscard]] std::string toJson() const override;
        [[nodiscard]] std::string toString() const override;
    };

    struct Identifier : public Expression {
        int indent           = 0;
        const char* nodeName = "Identifier";
        std::string name;
        explicit Identifier(std::string name);
        [[nodiscard]] std::string toJson() const override;
        [[nodiscard]] std::string toString() const override;
    };

    struct Operation : public Expression {
        int indent           = 0;
        const char* nodeName = "Operation";
        std::shared_ptr<Expression> left;
        std::shared_ptr<Operator> operator_;
        std::shared_ptr<Expression> right;
        explicit Operation(std::shared_ptr<Expression> left, std::shared_ptr<Operator> operator_, std::shared_ptr<Expression> right);
        [[nodiscard]] std::string toJson() const override;
        [[nodiscard]] std::string toString() const override;
    };

    struct Operator : public Expression {
        int indent           = 0;
        const char* nodeName = "Operator";
        std::string value;
        explicit Operator(std::string value);
        [[nodiscard]] std::string toJson() const override;
        [[nodiscard]] std::string toString() const override;
    };

    struct ObjectRef : public Expression {
        const char* nodeName = "ObjectRef";
        int indent           = 0;
        std::shared_ptr<Identifier> identifier;
        [[nodiscard]] std::string toJson() const override;
        [[nodiscard]] std::string toString() const override;
    };

    struct ObjectIns : public Expression {
        const char* nodeName = "ObjectIns";
        int indent           = 0;
        std::shared_ptr<Identifier> identifier;
        std::vector<std::shared_ptr<Member>> memberList;
        [[nodiscard]] std::string toJson() const override;
        [[nodiscard]] std::string toString() const override;
    };

    struct MapRef : public Expression {
        const char* nodeName = "MapRef";
        int indent           = 0;
        std::vector<std::shared_ptr<Pair>> pairList;
        [[nodiscard]] std::string toJson() const override;
        [[nodiscard]] std::string toString() const override;
    };

    struct EnumRef : public Expression {
        const char* nodeName = "EnumRef";
        int indent           = 0;
        std::shared_ptr<Identifier> enumName;
        std::shared_ptr<Identifier> enumValue;
        [[nodiscard]] std::string toJson() const override;
        [[nodiscard]] std::string toString() const override;
    };

    struct TemplateRef : public Expression {
        const char* nodeName = "TemplateRef";
        int indent           = 0;
        std::shared_ptr<Identifier> identifier;
        [[nodiscard]] std::string toJson() const override;
        [[nodiscard]] std::string toString() const override;
    };
}  // namespace ast

std::string idt(int indent);

#endif  // NDF_AST_H
