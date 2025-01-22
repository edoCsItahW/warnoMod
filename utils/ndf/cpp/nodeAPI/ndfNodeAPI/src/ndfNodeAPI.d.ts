declare module "ndf-node-api" {
    export abstract class AST {
        abstract nodeName: string;

        abstract pos(): number;

        abstract toString(): string;

        abstract toJson(): string;
    }

    export class Program extends AST {
        nodeName: string;

        pos(): number;

        constructor(statements: Statement[]);

        statements: Statement[];

        toString(): string;

        toJson(): string;
    }

    export class Member extends AST {
        nodeName: string;

        pos(): number;

        identifier: Identifier;
        expression: Expression;

        toString(): string;

        toJson(): string;
    }

    export class Parameter extends AST {
        nodeName: string;

        pos(): number;

        identifier: Identifier;
        type: Identifier;
        expression: Expression;

        toString(): string;

        toJson(): string;
    }

    export abstract class Statement extends AST {
    }

    export class Export extends Statement {
        nodeName: string;

        pos(): number;

        statement: Statement;

        toString(): string;

        toJson(): string;
    }

    export class MapDef extends Statement {
        nodeName: string;

        pos(): number;

        pairs: Pair[];

        toString(): string;

        toJson(): string;
    }

    export class ObjectDef extends Statement {
        nodeName: string;

        pos(): number;

        identifier: Identifier;
        type: Identifier;
        members: Member[];

        toString(): string;

        toJson(): string;
    }

    export class Assignment extends Statement {
        nodeName: string;

        pos(): number;

        identifier: Identifier;
        expression: Expression;

        toString(): string;

        toJson(): string;
    }

    export class TemplateDef extends Statement {
        nodeName: string;

        pos(): number;

        identifier: Identifier;
        parameters: Parameter[];
        members: Member[];

        toString(): string;

        toJson(): string;
    }

    export class Private extends Statement {
        nodeName: string;

        pos(): number;

        statement: Statement;

        toString(): string;

        toJson(): string;
    }

    export abstract class Expression extends AST {
    }

    export class Identifier extends Expression {
        nodeName: string;

        pos(): number;

        name: string;

        toString(): string;

        toJson(): string;
    }

    export class EnumRef extends Expression {
        nodeName: string;

        pos(): number;

        enumName: Identifier;
        enumValue: Identifier;

        toString(): string;

        toJson(): string;
    }

    export class MapRef extends Expression {
        nodeName: string;

        pos(): number;

        pairs: Pair[];

        toString(): string;

        toJson(): string;
    }

    export class ObjectIns extends Expression {
        nodeName: string;

        pos(): number;

        identifier: Identifier;
        members: Member[];

        toString(): string;

        toJson(): string;
    }

    export class ObjectRef extends Expression {
        nodeName: string;

        pos(): number;

        identifier: Identifier;

        toString(): string;

        toJson(): string;
    }

    export class Operation extends Expression {
        nodeName: string;

        pos(): number;

        left: Expression;
        operator_: Operator;
        right: Expression;

        toString(): string;

        toJson(): string;
    }

    export class Operator extends Expression {
        nodeName: string;

        pos(): number;

        value: string;

        toString(): string;

        toJson(): string;
    }

    export class TemplateParam extends Expression {
        nodeName: string;

        pos(): number;

        identifier: Identifier;

        toString(): string;

        toJson(): string;
    }

    export class TemplateRef extends Expression {
        nodeName: string;

        pos(): number;

        identifier: Identifier;

        toString(): string;

        toJson(): string;
    }

    export abstract class Literal extends Expression {
    }

    export class Bool extends Literal {
        nodeName: string;

        pos(): number;

        value: boolean;

        toString(): string;

        toJson(): string;
    }

    export class Float extends Literal {
        nodeName: string;

        pos(): number;

        value: number;

        toString(): string;

        toJson(): string;
    }

    export class Integer extends Literal {
        nodeName: string;

        pos(): number;

        value: number;

        toString(): string;

        toJson(): string;
    }

    export class GUID extends Literal {
        nodeName: string;

        pos(): number;

        value: string;

        toString(): string;

        toJson(): string;
    }

    export class Nil extends Literal {
        nodeName: string;

        pos(): number;

        value: null;

        toString(): string;

        toJson(): string;
    }

    export class Pair extends Literal {
        nodeName: string;

        pos(): number;

        first: Expression;
        second: Expression;

        toString(): string;

        toJson(): string;
    }

    export class Path extends Literal {
        nodeName: string;

        pos(): number;

        value: string;

        toString(): string;

        toJson(): string;
    }

    export class String extends Literal {
        nodeName: string;

        pos(): number;

        value: string;

        toString(): string;

        toJson(): string;
    }

    export class Vector extends Literal {
        nodeName: string;

        pos(): number;

        expressions: Expression[];

        toString(): string;

        toJson(): string;
    }

    export function parse(code: string, debug?: boolean): Program;
}