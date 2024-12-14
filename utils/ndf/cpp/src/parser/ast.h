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
 *
 * @if zh
 * @brief 该文件定义了AST节点的抽象类和具体类.
 *
 * @else
 * @brief This file defines the abstract class and concrete class of the AST node.
 * @endif
 * */
#ifndef NDF_AST_H
#define NDF_AST_H
#pragma once

#include <iostream>
#include <memory>
#include <vector>

/**
 * @if zh
 * @page antlr ANTLR描述
 * @else
 * @page antlr ANTLR description
 * @endif
 *
 * @code{.antlr}
 * grammar MyLanguage;
 * Program      : Statement* ;
 *
 * Statement    : Assignment
 *              | ObjectDef
 *              | MapDef
 *              | TemplateDef
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
 * // zh: 词法规则
 * // en: Lexical rules
 * DIGIT        : [0-9] ;
 * LETTER       : [a-zA-Z] ;
 * text         : (LETTER | DIGIT | ' ' | ',' | ';' | ':' | '-' | '_' | '.' | '!' | '?' | '@')* ;
 * @endcode
 * */

/** @namespace ast
 *
 * @if zh
 * @brief 该命名空间包含了AST节点的抽象类和具体类.
 *
 * @else
 * @brief This namespace contains the abstract class and concrete class of the AST node.
 * @endif
 * */
namespace ast {
    class Statement;

    class Pair;

    class Expression;

    class Identifier;

    class Operator;

    /** @struct AST
     *
     * @if zh
     * @brief AST节点AST
     * @details AST节点是所有AST节点的基类.
     * @remark 这是一个抽象的节点,其不包括子节点.
     *
     * @else
     * @brief AST node AST
     * @details The AST node is the base class of all AST nodes.
     * @remark This is an abstract node that does not include child nodes.
     * @endif
     * */
    struct AST {
        /** @var nodeName
         *
         * @if zh
         * @brief 节点名称
         * @details 节点名称用于标识节点类型.
         * @invariant 所有节点名称都是预定义的,不应该被修改.
         *
         * @else
         * @brief Node name
         * @details The node name is used to identify the node type.
         * @invariant All node names are predefined and should not be modified.
         *
         * @endif
         *
         * @qualifier const
         * @public @memberof AST
         * @pure
         * */
        const char *nodeName = "AST";

        [[nodiscard]] virtual std::string getNodeName() const = 0;

        /**
         * @if zh
         * @brief 将AST节点转换为JSON格式字符串.
         * @return std::string 转换后的JSON格式字符串.
         *
         * @else
         * @brief Convert the AST node to a JSON format string.
         * @return std::string The converted JSON format string.
         *
         * @endif
         *
         * @pure
         * */
        [[nodiscard]] virtual std::string toJson() const = 0;

        /**
         * @if zh
         * @brief 将AST节点转换为字符串.
         * @return std::string 转换后的字符串.
         *
         * @else
         * @brief Convert the AST node to a string.
         * @return std::string The converted string.
         * @endif
         *
         * @pure
         * */
        [[nodiscard]] virtual std::string toString() const = 0;
    };

    /** @struct Program
     *
     * @if zh
     * @brief AST节点Program
     * @details Program节点表示整个程序,包含多个Statement节点.
     * @note Program应该是整个AST的根节点.
     * @remark 这是一个具体的节点,其包括有子节点.
     *
     * @else
     * @brief AST node Program
     * @details The Program node represents the entire program, which contains multiple Statement nodes.
     * @note The Program node should be the root node of the AST.
     * @remark This is a concrete node that includes child nodes.
     * @endif
     *
     * @code{.antlr}
     * Program : Statement* ;
     * @endcode
     *
     * @see AST
     * */
    struct Program : public AST {
        const char *nodeName = "Program";  ///< @see AST
        std::vector<std::shared_ptr<Statement>> statements;

        [[nodiscard]] std::string getNodeName() const override;

        [[nodiscard]] std::string toJson() const override;

        [[nodiscard]] std::string toString() const override;
    };

    /** @struct Member
     *
     * @if zh
     * @brief AST节点Member
     * @details Member节点表示对象成员,包含一个Identifier节点和一个Expression节点.
     * @remark 这是一个具体的节点,其包括有子节点.
     *
     * @else
     * @brief AST node Member
     * @details The Member node represents an object member, which contains an Identifier node and an Expression node.
     * @remark This is a concrete node that includes child nodes.
     * @endif
     *
     * @code{.antlr}
     * Member : Identifier '=' Expression ;
     * @endcode
     *
     * @see AST
     * */
    struct Member : public AST {
        const char *nodeName = "Member";
        std::shared_ptr<Identifier> identifier;
        std::shared_ptr<Expression> expression;

        [[nodiscard]] std::string getNodeName() const override;

        [[nodiscard]] std::string toJson() const override;

        [[nodiscard]] std::string toString() const override;
    };

    /** @struct Parameter
     *
     * @if zh
     * @brief AST节点Parameter
     * @details Parameter节点表示模板参数,包含一个Identifier节点,一个Identifier节点,一个Expression节点.
     * @remark 这是一个具体的节点,其包括有子节点.
     * @note
     * 注意: 由于@c "类型（几乎？）总是以大写字母“T”开头。它们代表游戏的内部数据结构，其定义不可用。"因此，我们将<type>更改为<identifier>。
     *
     * @else
     * @brief AST node Parameter
     * @details The Parameter node represents a template parameter, which contains an Identifier node, an Identifier node, and an Expression node.
     * @remark This is a concrete node that includes child nodes.
     * @note
     * Note: Since `"types (almost?) always start with a capital letter "T", they represent internal data structures of the game, which cannot be defined."` Therefore, we changed \<type> to
     * \<identifier>.
     * @endif
     *
     * @code{.antlr}
     * Parameter : Identifier [':' Identifier] ['=' Expression] ;
     * @endcode
     *
     * @see AST
     * @see \warnoMod\utils\usefulInfo\Modding Manual.pdf
     * */
    struct Parameter : public AST {
        const char *nodeName = "Parameter";
        std::shared_ptr<Identifier> identifier;
        std::shared_ptr<Identifier> type;
        std::shared_ptr<Expression> expression;

        [[nodiscard]] std::string getNodeName() const override;

        [[nodiscard]] std::string toJson() const override;

        [[nodiscard]] std::string toString() const override;
    };

    /** @struct Statement
     *
     * @todo 注释语句可能会重新加入
     *
     * @if zh
     * @brief AST节点Statement
     * @details Statement节点表示程序中的语句
     * @remark 这是一个抽象的节点,其不包括子节点.
     *
     * @else
     * @brief AST node Statement
     * @details The Statement node represents a statement in the program, which contains multiple Expression nodes.
     * @remark This is an abstract node that does not include child nodes.
     * @endif
     *
     * @pure
     *
     * @code{.antlr}
     * // zh: 实际的AST中注释语句被剔除了(在词法分析中)
     * // en: The comment statement is removed in the actual AST (during lexical analysis)
     * Statement : Assignment
     *           | ObjectDef
     *           | MapDef
     *           | TemplateDef
     *           | Comment
     *           | Export ;
     * @endcode
     *
     * @see AST
     * */
    struct Statement : public AST {
        const char *nodeName = "Statement";
    };

    /** @struct Export
     *
     * @if zh
     * @brief AST节点Export
     * @details Export节点表示导出语句,包含一个Statement节点.
     * @remark 这是一个具体的节点,其包括有子节点.
     *
     * @else
     * @brief AST node Export
     * @details The Export node represents an export statement, which contains a Statement node.
     * @remark This is a concrete node that includes child nodes.
     * @endif
     *
     * @code{.antlr}
     * Export : 'export' Statement ;
     * @endcode
     *
     * @see Statement
     * */
    struct Export : public Statement {
        const char *nodeName = "Export";
        std::shared_ptr<Statement> statement;

        [[nodiscard]] std::string getNodeName() const override;

        [[nodiscard]] std::string toJson() const override;

        [[nodiscard]] std::string toString() const override;
    };

    /** @struct MapDef
     *
     * @if zh
     * @brief AST节点MapDef
     * @details MapDef节点表示映射定义语句,包含一个Pair节点的列表.
     * @remark 这是一个具体的节点,其包括有子节点.
     *
     * @else
     * @brief AST node MapDef
     * @details The MapDef node represents a map definition statement, which contains a list of Pair nodes.
     * @remark This is a concrete node that includes child nodes.
     * @endif
     *
     * @code{.antlr}
     * MapDef : 'MAP' '[' Pair* ']' ;
     * @endcode
     *
     * @see Statement
     * */
    struct MapDef : public Statement {
        const char *nodeName = "MapDef";
        std::vector<std::shared_ptr<Pair>> pairs;

        [[nodiscard]] std::string getNodeName() const override;

        [[nodiscard]] std::string toJson() const override;

        [[nodiscard]] std::string toString() const override;
    };

    /** @struct ObjectDef
     *
     * @if zh
     * @brief AST节点ObjectDef
     * @details ObjectDef节点表示对象定义语句,包含一个Identifier节点,一个Identifier节点,一个Member节点的列表.
     * @remark 这是一个具体的节点,其包括有子节点.
     * @warning
     * 注意: 由于@c "类型(几乎?)总是以大写字母"T"开头.它们代表游戏的内部数据结构,其定义不可用." 因此,我们将<type>改为<identifier>。
     *
     * @else
     * @brief AST node ObjectDef
     * @details The ObjectDef node represents an object definition statement, which contains an Identifier node, an Identifier node, and a list of Member nodes.
     * @remark This is a concrete node that includes child nodes.
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
        const char *nodeName = "ObjectDef";
        std::shared_ptr<Identifier> identifier;
        std::shared_ptr<Identifier> type;
        std::vector<std::shared_ptr<Member>> members;

        [[nodiscard]] std::string getNodeName() const override;

        [[nodiscard]] std::string toJson() const override;

        [[nodiscard]] std::string toString() const override;
    };

    /** @struct Assignment
     *
     * @if zh
     * @brief AST节点Assignment
     * @details Assignment节点表示赋值语句,包含一个Identifier节点和一个Expression节点.
     * @remark 这是一个具体的节点,其包括有子节点.
     *
     * @else
     * @brief AST node Assignment
     * @details The Assignment node represents an assignment statement, which contains an Identifier node and an Expression node.
     * @remark This is a concrete node that includes child nodes.
     * @endif
     *
     * @code{.antlr}
     * Assignment : Identifier 'is' Expression ;
     * @endcode
     *
     * @see Statement
     * */
    struct Assignment : public Statement {
        const char *nodeName = "Assignment";
        std::shared_ptr<Identifier> identifier;
        std::shared_ptr<Expression> expression;

        [[nodiscard]] std::string getNodeName() const override;

        [[nodiscard]] std::string toJson() const override;

        [[nodiscard]] std::string toString() const override;
    };

    /** @struct TemplateDef
     *
     * @if zh
     * @brief AST节点TemplateDef
     * @details TemplateDef节点表示模板定义语句,包含一个Identifier节点,一个Parameter节点的列表,一个Identifier节点,一个Member节点的列表.
     * @remark 这是一个具体的节点,其包括有子节点.
     *
     * @else
     * @brief AST node TemplateDef
     * @details The TemplateDef node represents a template definition statement, which contains an Identifier node, a list of Parameter nodes, an Identifier node, and a list of Member nodes.
     * @remark This is a concrete node that includes child nodes.
     * @endif
     *
     * @code{.antlr}
     * TemplateDef : 'template' Identifier '[' Parameter* ']' 'is' Identifier '(' Member* ')' ;
     * @endcode
     *
     * @see Statement
     * */
    struct TemplateDef : public Statement {
        const char *nodeName = "TemplateDef";
        std::shared_ptr<Identifier> identifier;
        std::vector<std::shared_ptr<Parameter>> parameters;
        std::shared_ptr<Identifier> type;
        std::vector<std::shared_ptr<Member>> members;

        [[nodiscard]] std::string getNodeName() const override;

        [[nodiscard]] std::string toJson() const override;

        [[nodiscard]] std::string toString() const override;
    };

    /** @struct Private
     *
     * @if zh
     * @brief AST节点Private
     * @details Private节点表示私有语句,包含一个Statement节点.
     * @remark 这是一个具体的节点,其包括有子节点.
     *
     * @else
     * @brief AST node Private
     * @details The Private node represents a private statement, which contains a Statement node.
     * @remark This is a concrete node that includes child nodes.
     * @endif
     *
     * @code{.antlr}
     * Private : 'private' Statement ;
     * @endcode
     *
     * @see Statement
     * */
    struct Private : public Statement {
        const char *nodeName = "Private";
        std::shared_ptr<Statement> statement;

        [[nodiscard]] std::string getNodeName() const override;

        [[nodiscard]] std::string toJson() const override;

        [[nodiscard]] std::string toString() const override;
    };

    /** @struct Expression
     *
     * @if zh
     * @brief AST节点Expression
     * @details Expression节点表示表达式
     * @remark 这是一个抽象的节点,其不包括子节点.
     *
     * @else
     * @brief AST node Expression
     * @details The Expression node represents an expression, which contains a Literal node, an Identifier node, and an Operation node.
     * @remark This is an abstract node that does not include child nodes.
     * @endif
     *
     * @pure
     *
     * @code{.antlr}
     * Expression : Literal
     *            | Identifier
     *            | Operation
     *            | ObjectRef
     *            | MapRef
     *            | TemplateRef
     *            | ObjectIns
     *            | TemplateParam
     *            '(' Expression ')' ;
     * @endcode
     *
     * @see AST
     * */
    struct Expression : public AST {
        const char *nodeName = "Expression";
    };

    /** @struct Identifier
     *
     * @if zh
     * @brief AST节点Identifier
     * @details Identifier节点表示标识符.其包含一个字符串值.
     * @remark 这是一个具体的节点,其不包括子节点.
     *
     * @else
     * @brief AST node Identifier
     * @details The Identifier node represents an identifier, which contains a string value.
     * @remark This is a concrete node that does not include child nodes.
     * @endif
     *
     * @code{.antlr}
     * Identifier : LETTER (LETTER | DIGIT | '_')* ;
     * @endcode
     *
     * @see Expression
     * */
    struct Identifier : public Expression {
        const char *nodeName = "Identifier";
        std::string name;

        [[nodiscard]] std::string getNodeName() const override;

        [[nodiscard]] std::string toJson() const override;

        [[nodiscard]] std::string toString() const override;
    };

    /** @struct EnumRef
     *
     * @if zh
     * @brief AST节点EnumRef
     * @details EnumRef节点表示枚举引用.其包含一个Identifier节点和一个Identifier节点.
     * @remark 这是一个具体的节点,其包括有子节点.
     *
     * @else
     * @brief AST node EnumRef
     * @details The EnumRef node represents an enum reference, which contains an Identifier node and an Identifier node.
     * @remark This is a concrete node that includes child nodes.
     * @endif
     *
     * @code{.antlr}
     * EnumRef : Identifier '/' Identifier ;
     * @endcode
     *
     * @see Expression
     * */
    struct EnumRef : public Expression {
        const char *nodeName = "EnumRef";
        std::shared_ptr<Identifier> enumName;
        std::shared_ptr<Identifier> enumValue;

        [[nodiscard]] std::string getNodeName() const override;

        [[nodiscard]] std::string toJson() const override;

        [[nodiscard]] std::string toString() const override;
    };

    /** @struct MapRef
     *
     * @if zh
     * @brief AST节点MapRef
     * @details MapRef节点表示映射引用.其包含一个Pair节点的列表.
     * @remark 这是一个具体的节点,其包括有子节点.
     *
     * @else
     * @brief AST node MapRef
     * @details The MapRef node represents a map reference, which contains a list of Pair nodes.
     * @remark This is a concrete node that includes child nodes.
     * @endif
     *
     * @code{.antlr}
     * MapRef : 'MAP' '[' Pair* ']' ;
     * @endcode
     *
     * @see Expression
     * */
    struct MapRef : public Expression {
        const char *nodeName = "MapRef";
        std::vector<std::shared_ptr<Pair>> pairs;

        [[nodiscard]] std::string getNodeName() const override;

        [[nodiscard]] std::string toJson() const override;

        [[nodiscard]] std::string toString() const override;
    };

    /** @struct ObjectIns
     *
     * @if zh
     * @brief AST节点ObjectIns
     * @details ObjectIns节点表示对象实例化.其包含一个Identifier节点和一个Member节点的列表.
     * @remark 这是一个具体的节点,其包括有子节点.
     *
     * @else
     * @brief AST node ObjectIns
     * @details The ObjectIns node represents an object instantiation, which contains an Identifier node and a list of Member nodes.
     * @remark This is a concrete node that includes child nodes.
     * @endif
     *
     * @code{.antlr}
     * ObjectIns : Identifier '(' Member* ')' ;
     * @endcode
     *
     * @see Expression
     * */
    struct ObjectIns : public Expression {
        const char *nodeName = "ObjectIns";
        std::shared_ptr<Identifier> identifier;
        std::vector<std::shared_ptr<Member>> members;

        [[nodiscard]] std::string getNodeName() const override;

        [[nodiscard]] std::string toJson() const override;

        [[nodiscard]] std::string toString() const override;
    };

    /** @struct ObjectRef
     *
     * @if zh
     * @brief AST节点ObjectRef
     * @details ObjectRef节点表示对象引用.其包含一个Identifier节点.
     * @remark 这是一个具体的节点,其包括有子节点.
     *
     * @else
     * @brief AST node ObjectRef
     * @details The ObjectRef node represents an object reference, which contains an Identifier node.
     * @remark This is a concrete node that includes child nodes.
     * @endif
     *
     * @code{.antlr}
     * ObjectRef : '$' '/' Identifier ;
     * @endcode
     *
     * @see Expression
     * */
    struct ObjectRef : public Expression {
        const char *nodeName = "ObjectRef";
        std::shared_ptr<Identifier> identifier;

        [[nodiscard]] std::string getNodeName() const override;

        [[nodiscard]] std::string toJson() const override;

        [[nodiscard]] std::string toString() const override;
    };

    /** @struct Operation
     *
     * @if zh
     * @brief AST节点Operation
     * @details Operation节点表示运算符.其包含一个Expression节点,一个Operator节点,一个Expression节点.
     * @remark 这是一个具体的节点,其包括有子节点.
     *
     * @else
     * @brief AST node Operation
     * @details The Operation node represents an operator, which contains an Expression node, an Operator node, and an Expression node.
     * @remark This is a concrete node that includes child nodes.
     * @endif
     *
     * @code{.antlr}
     * Operation : Expression Operator Expression ;
     * @endcode
     *
     * @see Expression
     * */
    struct Operation : public Expression {
        const char *nodeName = "Operation";
        std::shared_ptr<Expression> left;
        std::shared_ptr<Operator> operator_;
        std::shared_ptr<Expression> right;

        [[nodiscard]] std::string getNodeName() const override;

        [[nodiscard]] std::string toJson() const override;

        [[nodiscard]] std::string toString() const override;
    };

    /** @struct Operator
     *
     * @if zh
     * @brief AST节点Operator
     * @details Operator节点表示运算符.其包含一个字符串值.
     * @remark 这是一个具体的节点,其不包括子节点.
     *
     * @else
     * @brief AST node Operator
     * @details The Operator node represents an operator, which contains a string value.
     * @remark This is a concrete node that does not include child nodes.
     * @endif
     *
     * @code{.antlr}
     * Operator : '+' | '-' | '*' | '/' | '%' | '^' | '==' | '!=' | '<' | '>' | '<=' | '>=' | '&&' | '||' | '!' ;
     * @endcode
     *
     * @see Operation
     * */
    struct Operator : public Expression {
        const char *nodeName = "Operator";
        std::string value;

        [[nodiscard]] std::string getNodeName() const override;

        [[nodiscard]] std::string toJson() const override;

        [[nodiscard]] std::string toString() const override;
    };

    /** @struct TemplateParam
     *
     * @if zh
     * @brief AST节点TemplateParam
     * @details TemplateParam节点表示模板参数.其包含一个Identifier节点.
     * @remark 这是一个具体的节点,其包括有子节点.
     *
     * @else
     * @brief AST node TemplateParam
     * @details The TemplateParam node represents a template parameter, which contains an Identifier node.
     * @remark This is a concrete node that includes child nodes.
     * @endif
     *
     * @code{.antlr}
     * TemplateParam : '<' Identifier '>' ;
     * @endcode
     *
     * @see Expression
     * */
    struct TemplateParam : public Expression {
        const char *nodeName = "TemplateParam";
        std::shared_ptr<Identifier> identifier;

        [[nodiscard]] std::string getNodeName() const override;

        [[nodiscard]] std::string toJson() const override;

        [[nodiscard]] std::string toString() const override;
    };

    /** @struct TemplateRef
     *
     * @if zh
     * @brief AST节点TemplateRef
     * @details TemplateRef节点表示模板引用.其包含一个Identifier节点.
     * @remark 这是一个具体的节点,其包括有子节点.
     *
     * @else
     * @brief AST node TemplateRef
     * @details The TemplateRef node represents a template reference, which contains an Identifier node.
     * @remark This is a concrete node that includes child nodes.
     * @endif
     *
     * @code{.antlr}
     * TemplateRef : 'template' Identifier ;
     * @endcode
     *
     * @see Expression
     * */
    struct TemplateRef : public Expression {
        const char *nodeName = "TemplateRef";
        std::shared_ptr<Identifier> identifier;

        [[nodiscard]] std::string getNodeName() const override;

        [[nodiscard]] std::string toJson() const override;

        [[nodiscard]] std::string toString() const override;
    };

    /** @struct Literal
     *
     * @if zh
     * @brief AST节点Literal
     * @details Literal节点表示字面值.
     * @remark 这是一个抽象的节点,其不包括子节点.
     *
     * @else
     * @brief AST node Literal
     * @details The Literal node represents a literal, which contains a GUID node, a Path node, a Pair node, a Boolean node, a String node, an Integer node, a Float node, a Nil node, and a Vector
     * node.
     * @remark This is an abstract node that does not include child nodes.
     * @endif
     *
     * @pure
     *
     * @code{.antlr}
     * Literal : GUID
     *         | Path
     *         | Pair
     *         | Boolean
     *         | String
     *         | Integer
     *         | Float
     *         | Nil
     *         | Vector ;
     * @endcode
     *
     * @see Expression
     * */
    struct Literal : public Expression {
        const char *nodeName = "Literal";
    };

    /** @struct Boolean
     *
     * @if zh
     * @brief AST节点Boolean
     * @details Boolean节点表示布尔值.其包含一个布尔值.
     * @remark 这是一个具体的节点,其不包括子节点.
     *
     * @else
     * @brief AST node Boolean
     * @details The Boolean node represents a boolean value, which contains a boolean value.
     * @remark This is a concrete node that does not include child nodes.
     * @endif
     *
     * @code{.antlr}
     * Boolean : 'true' | 'false' 'True' | 'False' ;
     * @endcode
     *
     * @see Literal
     * */
    struct Boolean : public Literal {
        bool value;
        const char *nodeName = "Boolean";

        [[nodiscard]] std::string getNodeName() const override;

        [[nodiscard]] std::string toJson() const override;

        [[nodiscard]] std::string toString() const override;
    };

    /** @struct Float
     *
     * @if zh
     * @brief AST节点Float
     * @details Float节点表示浮点数.其包含一个浮点数值.
     * @remark 这是一个具体的节点,其不包括子节点.
     *
     * @else
     * @brief AST node Float
     * @details The Float node represents a float, which contains a float value.
     * @remark This is a concrete node that does not include child nodes.
     * @endif
     *
     * @code{.antlr}
     * Float : DIGIT+ '.' DIGIT+ ;
     * @endcode
     *
     * @see Literal
     * */
    struct Float : public Literal {
        const char *nodeName = "Float";
        double value;

        [[nodiscard]] std::string getNodeName() const override;

        [[nodiscard]] std::string toJson() const override;

        [[nodiscard]] std::string toString() const override;
    };

    /** @struct Integer
     *
     * @if zh
     * @brief AST节点Integer
     * @details Integer节点表示整数.其包含一个整数值.
     * @remark 这是一个具体的节点,其不包括子节点.
     *
     * @else
     * @brief AST node Integer
     * @details The Integer node represents an integer, which contains an integer value.
     * @remark This is a concrete node that does not include child nodes.
     * @endif
     *
     * @code{.antlr}
     * Integer : DIGIT+ ;
     * @endcode
     *
     * @see Literal
     */
    struct Integer : public Literal {
        const char *nodeName = "Integer";
        int value;

        [[nodiscard]] std::string getNodeName() const override;

        [[nodiscard]] std::string toJson() const override;

        [[nodiscard]] std::string toString() const override;
    };

    /** @struct GUID
     *
     * @if zh
     * @brief AST节点GUID
     * @details GUID节点表示全局唯一标识符.其包含一个字符串值.
     * @remark 这是一个具体的节点,其不包括子节点.
     *
     * @else
     * @brief AST node GUID
     * @details The GUID node represents a global unique identifier, which contains a string value.
     * @remark This is a concrete node that does not include child nodes.
     * @endif
     *
     * @code{.antlr}
     * GUID : 'GUID' ':' '{' (DIGIT | LETTER | '-'){32,36} '}' ;
     * @endcode
     *
     * @see Literal
     * */
    struct GUID : public Literal {
        const char *nodeName = "GUID";
        std::string value;

        [[nodiscard]] std::string getNodeName() const override;

        [[nodiscard]] std::string toJson() const override;

        [[nodiscard]] std::string toString() const override;
    };

    /** @struct Nil
     *
     * @if zh
     * @brief AST节点Nil
     * @details Nil节点表示空值.其包含一个字符串值.
     * @remark 这是一个具体的节点,其不包括子节点.
     *
     * @else
     * @brief AST node Nil
     * @details The Nil node represents a nil value, which contains a string value.
     * @remark This is a concrete node that does not include child nodes.
     * @endif
     *
     * @code{.antlr}
     * Nil : 'nil' ;
     * @endcode
     *
     * @see Literal
     * */
    struct Nil : public Literal {
        const char *nodeName = "Nil";
        std::string value    = "nil";

        [[nodiscard]] std::string getNodeName() const override;

        [[nodiscard]] std::string toJson() const override;

        [[nodiscard]] std::string toString() const override;
    };

    /** @struct Pair
     *
     * @if zh
     * @brief AST节点Pair
     * @details Pair节点表示键值对.其包含两个Expression节点.
     * @remark 这是一个具体的节点,其包括有子节点.
     *
     * @else
     * @brief AST node Pair
     * @details The Pair node represents a key-value pair, which contains two Expression nodes.
     * @remark This is a concrete node that includes child nodes.
     * @endif
     *
     * @code{.antlr}
     * Pair : '(' Expression ',' Expression ')' ;
     * @endcode
     *
     * @see Literal
     * */
    struct Pair : public Literal {
        const char *nodeName = "Pair";
        std::shared_ptr<Expression> first;
        std::shared_ptr<Expression> second;

        [[nodiscard]] std::string getNodeName() const override;

        [[nodiscard]] std::string toJson() const override;

        [[nodiscard]] std::string toString() const override;
    };

    /** @struct Path
     *
     * @if zh
     * @brief AST节点Path
     * @details Path节点表示路径.其包含一个字符串值.
     * @remark 这是一个具体的节点,其不包括子节点.
     *
     * @else
     * @brief AST node Path
     * @details The Path node represents a path, which contains a string value.
     * @remark This is a concrete node that does not include child nodes.
     * @endif
     *
     * @code{.antlr}
     * Path : ('$' | '~' | '.') '/' (LETTER | DIGIT | '_')+ ;
     * @endcode
     *
     * @see Literal
     * */
    struct Path : public Literal {
        const char *nodeName = "Path";
        std::string value;

        [[nodiscard]] std::string getNodeName() const override;

        [[nodiscard]] std::string toJson() const override;

        [[nodiscard]] std::string toString() const override;
    };

    /** @struct String
     *
     * @if zh
     * @brief AST节点String
     * @details String节点表示字符串.其包含一个字符串值.
     * @remark 这是一个具体的节点,其不包括子节点.
     *
     * @else
     * @brief AST node String
     * @details The String node represents a string, which contains a string value.
     * @remark This is a concrete node that does not include child nodes.
     * @endif
     *
     * @code{.antlr}
     * String : '\'' text '\''
     *        | '\"' text '\"' ;
     * @endcode
     *
     * @see Literal
     */
    struct String : public Literal {
        const char *nodeName = "String";
        std::string value;

        [[nodiscard]] std::string getNodeName() const override;

        [[nodiscard]] std::string toJson() const override;

        [[nodiscard]] std::string toString() const override;
    };

    /** @struct Vector
     *
     * @if zh
     * @brief AST节点Vector
     * @details Vector节点表示向量.其包含一个Expression节点的列表.
     * @remark 这是一个具体的节点,其包括有子节点.
     *
     * @else
     * @brief AST node Vector
     * @details The Vector node represents a vector, which contains a list of Expression nodes.
     * @remark This is a concrete node that includes child nodes.
     * @endif
     *
     * @code{.antlr}
     * Vector : '[' Expression* ']' ;
     * @endcode
     *
     * @see Literal
     * */
    struct Vector : public Literal {
        const char *nodeName = "Vector";
        std::vector<std::shared_ptr<Expression>> expressions;

        [[nodiscard]] std::string getNodeName() const override;

        [[nodiscard]] std::string toJson() const override;

        [[nodiscard]] std::string toString() const override;
    };

}  // namespace ast

#endif  // NDF_AST_H
