/**
 * @file token.ts
 * @author edocsitahw
 * @version 1.1
 * @date 2024/12/18 下午10:00
 * @desc
 * */

import { enumToStr } from "../utils";


export enum TokenType {
    NUMBER,
    INT,
    FLOAT,
    STRING,
    PATH,  

    LITERAL,     
    IDENTIFIER,  

    KW_IS,        
    KW_MAP,       
    KW_DIV,       
    KW_TEMPLATE,  
    KW_EXPORT,    
    KW_UNNAMED,   
    KW_NIL,       
    KW_PRIVATE,   
    KW_BOOLEN,    

    GUID,  

    OPERATOR,  

    ADD,     
    SUB,     
    MUL,     
    DIV,     
    MOD,     
    ASSIGN,  
    LT,      
    GT,      
    LE,      
    GE,      
    EQ,      
    NE,      
    OR,      
    NOT,     

    LPAREN,     
    RPAREN,     
    LBRACE,     
    RBRACE,     
    LBRACKET,   
    RBRACKET,   
    COMMA,      
    SEMICOLON,  
    COLON,      
    DOT,
    DOLLAR,
    QUESTION,
    TILDE,      

    COMMENT,  
    COMMENT_LINE,  
    COMMENT_BLOCK,  

    NEWLINE,  
    EOF,
    ERROR,
    UNKNOWN   

}

export class Pos {
    private _col: number = 0;
    private _line: number = 1;
    private _offset: number = 0;

    constructor() {
    }

    get col(): number { return this._col; }
    get line(): number { return this._line; }
    get offset(): number { return this._offset; }

    newLine(): void {
        this._col = 0;
        this._line++;
        this._offset++;
    }

    next(): void {
        this._col++;
        this._offset++;
    }

    move(offset: number) {
        this._offset += offset;
        this._col += offset;
    }
}

export class Token {
    super: TokenType = TokenType.UNKNOWN;
    constructor(public type: TokenType, public pos: { line: number, col: number }, public value: string = "") {
    }

    toString(): string {
        return `Token<${this.pos.line},${this.pos.col}>[${enumToStr(TokenType, this.type)}]('${this.value}')`;
    }
}
