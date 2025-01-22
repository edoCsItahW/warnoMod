/**
 * @file lexer.ts
 * @author edocsitahw
 * @version 1.1
 * @date 2024/12/18 下午10:23
 * @desc
 * */

import {Pos, Token, TokenType} from "./token";
import {isAlpha, isDigit, isSpace} from "../utils";

function ext(_pos: Pos): {line: number, col: number} {
    return {line: _pos.line, col: _pos.col};
}

const specialChars: {[key: string]: string} = {
    '\n': 'n',
    '\t': 't',
    '\r': 'r',
    '\b': 'b',
}

export class Lexer {
    private _pos: Pos = new Pos();
    private _inScope = () => this._pos.offset < this.src.length;
    constructor(readonly src: string, public debug = false) {}

    private skip() {
        while (this._inScope() && isSpace(this.curr) && this.curr !== "\n")
            this._pos.next();
    }

    private extractNumber(): Token {
        if (this.debug) console.log("extractNumber");
        let num = "";
        const start = ext(this._pos);
        while (this._inScope() && (isDigit(this.curr) || this.curr === ".")) {
            num += this.curr;
            this._pos.next();
        }
        return new Token(TokenType.NUMBER, start, num);
    }

    private extractString(): Token {
        if (this.debug) console.log("extractString");
        let str = "";
        const start = ext(this._pos);
        const quote = this.curr;
        this._pos.next();
        while (this._inScope() && this.curr !== quote) {

            if (this.curr in specialChars) {
                str += '\\' + specialChars[this.curr];
                this._pos.move(2);
            }
            else str += this.curr;
            this._pos.next();
        }
        this._pos.next();
        return new Token(TokenType.STRING, start, str);
    }
    private extractLiteral(): Token {
        if (this.debug) console.log("extractLiteral");
        let lit = "";
        const start = ext(this._pos);
        while (this._inScope() && (isAlpha(this.curr) || this.curr == '_' || isDigit(this.curr))) {
            lit += this.curr;
            this._pos.next();
        }
        return new Token(TokenType.LITERAL, start, lit);

    }
    private extractOperator(): Token {
        if (this.debug) console.log("extractOperator");
        const start = ext(this._pos);
        switch (this.curr) {
            case '+': this._pos.next(); return new Token(TokenType.ADD, start, "+");
            case '-': this._pos.next(); return new Token(TokenType.SUB, start, "-");
            case '*': this._pos.next(); return new Token(TokenType.MUL, start, "*");
            case '/': this._pos.next(); return new Token(TokenType.DIV, start, "/");
            case '%': this._pos.next(); return new Token(TokenType.MOD, start, "%");
            case '=': this._pos.next(); return new Token(TokenType.ASSIGN, start, "=");
            case '<': this._pos.next(); return new Token(TokenType.LT, start, "<");
            case '>': this._pos.next(); return new Token(TokenType.GT, start, ">");
            case '|': this._pos.next(); return new Token(TokenType.OR, start, "|");
            case '!': this._pos.next(); return new Token(TokenType.NOT, start, "!");
            case '(': this._pos.next(); return new Token(TokenType.LPAREN, start, "(");
            case ')': this._pos.next(); return new Token(TokenType.RPAREN, start, ")");
            case '{': this._pos.next(); return new Token(TokenType.LBRACE, start, "{");
            case '}': this._pos.next(); return new Token(TokenType.RBRACE, start, "}");
            case '[': this._pos.next(); return new Token(TokenType.LBRACKET, start, "[");
            case ']': this._pos.next(); return new Token(TokenType.RBRACKET, start, "]");
            case ',': this._pos.next(); return new Token(TokenType.COMMA, start, ",");
            case ';': this._pos.next(); return new Token(TokenType.SEMICOLON, start, ";");
            case ':': this._pos.next(); return new Token(TokenType.COLON, start, ":");
            case '.': this._pos.next(); return new Token(TokenType.DOT, start, ".");
            case '?': this._pos.next(); return new Token(TokenType.QUESTION, start, "?");
            case '~': this._pos.next(); return new Token(TokenType.TILDE, start, "~");
            case '$': this._pos.next(); return new Token(TokenType.DOLLAR, start, "$");
            default: this._pos.next(); return new Token(TokenType.UNKNOWN, start, this.curr);
        }
    }

    private extractLineComment(): Token {
        if (this.debug) console.log("extractLineComment");
        let comment = "";
        const start = ext(this._pos);
        this._pos.move(2);
        while (this._inScope() && this.curr !== "\n") {
            comment += this.curr;
            this._pos.next();
        }
        const tk = new Token(TokenType.COMMENT_LINE, start, comment);
        tk.super = TokenType.COMMENT;
        return tk;
    }

    private extractBlockComment(): Token {
        if (this.debug) console.log("extractBlockComment");
        let comment = "";
        const start = ext(this._pos);
        this._pos.move(2);
        while (this._inScope() && !(this.curr === "*" && this.src[this._pos.offset + 1] === "/")) {
            comment += this.curr;
            this._pos.next();
            if (this.curr === "\n") this._pos.newLine();
        }
        this._pos.move(2);
        const tk = new Token(TokenType.COMMENT_BLOCK, start, comment);
        tk.super = TokenType.COMMENT;
        return tk;
    }
    private extractPath(): Token {
        if (this.debug) console.log("extractPath");
        let path = "";
        const start = ext(this._pos);
        path += this.curr;
        this._pos.next();
        path += this.curr;
        this._pos.next();
        while (this._inScope() && (isAlpha(this.curr) || isDigit(this.curr) || this.curr === "_" || this.curr === "/" )) {
            path += this.curr;
            this._pos.next();
        }
        return new Token(TokenType.PATH, start, path);
    }

    private get curr() {
        return this.src[this._pos.offset];
    }

    next(): Token {
        this.skip();
        const start = ext(this._pos);

        if (!this._inScope())
            return new Token(TokenType.EOF, ext(this._pos));
        else if (this.curr === "\n") {
            this._pos.newLine();
            return new Token(TokenType.NEWLINE, start, "\\n");
        }
        else if (isDigit(this.curr) || (this.curr === "." && isDigit(this.src[this._pos.offset + 1])))
            return this.extractNumber();
        else if (isAlpha(this.curr) || this.curr === "_")
            return this.extractLiteral();
        else if (this.curr === '/')
            if (this.src[this._pos.offset + 1] === '/') return this.extractLineComment();
            else if (this.src[this._pos.offset + 1] === '*') return this.extractBlockComment();
            else return this.extractOperator();
        else if (this.curr === '"' || this.curr === "'")
            return this.extractString();
        else if (this.curr === '$' || this.curr === '~' || this.curr === '.')
            return this.extractPath();
        else if (!isAlpha(this.curr) && !isDigit(this.curr)) {
            const op = this.extractOperator();
            op.super = TokenType.OPERATOR;
            return op;
        }

        const tk = new Token(TokenType.UNKNOWN, ext(this._pos), this.curr);
        this._pos.next();
        return tk;
    }

    static tokenize(src: string, debug = false): Token[] {
        const lexer = new Lexer(src, debug);
        const tokens: Token[] = [];
        while (lexer._inScope()) {
            const tk = lexer.next();
            if (debug) console.log(tk.toString());
            if (tk.type !== TokenType.EOF) tokens.push(tk);
        }
        return tokens;
    }
}
