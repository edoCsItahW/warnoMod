import { Token, TokenType } from './token';

export class TokenProcessor {
	private _idx: number = 0;

	constructor(private _tokens: Token[]) {}

	private handleNumber(): Token {
		const num = this._tokens[this._idx++];
		if (num.value.includes('.')) num.type = TokenType.FLOAT;
		else num.type = TokenType.INT;
		num.super = TokenType.NUMBER;
		return num;
	}

	private handleLiteral(): Token {
		const lit = this._tokens[this._idx++];

		switch (lit.value) {
			case 'true':
			case 'false':
			case 'True':
			case 'False':
				lit.type = TokenType.KW_BOOLEN; break;
			case 'nil': lit.type = TokenType.KW_NIL; break;
			case 'is': lit.type = TokenType.KW_IS; break;
			case 'MAP': lit.type = TokenType.KW_MAP; break;
			case 'div': lit.type = TokenType.KW_DIV; break;
			case 'template': lit.type = TokenType.KW_TEMPLATE; break;
			case 'export': lit.type = TokenType.KW_EXPORT; break;
			case 'unnamed': lit.type = TokenType.KW_UNNAMED; break;
			case 'private': lit.type = TokenType.KW_PRIVATE; break;
			case 'GUID':
				if (this._tokens[this._idx].type === TokenType.COLON && this._tokens[this._idx + 1].type === TokenType.LBRACE) {
					let pos: number;
					let guid = "";
					for (pos = this._idx + 2; pos < this._tokens.length; pos++)
						if (this._tokens[pos].type === TokenType.RBRACE) break;
						else guid += this._tokens[pos].value;
					if (pos === this._tokens.length) throw new Error("Unclosed GUID");
					this._idx = pos + 1;
					lit.type = TokenType.GUID;
					lit.value = guid;
				}
				break;
			default: lit.type = TokenType.IDENTIFIER; break;
		}
		
		if (lit.type !== TokenType.IDENTIFIER) 
			lit.super = TokenType.LITERAL;
	
		return lit;
	}

	private handleOperator(): Token {
		const op = this._tokens[this._idx++];

		switch (op.value) {
			case '=': 
				if (this._tokens[this._idx].value === '=') {
					this._idx++;
					return new Token(TokenType.EQ, op.pos, "==");
				}
			case '<': 
				if (this._tokens[this._idx].value === '=') {
					this._idx++;
					return new Token(TokenType.LE, op.pos, "<=");
				}
			case '>': 
				if (this._tokens[this._idx].value === '=') {
					this._idx++;
					return new Token(TokenType.GE, op.pos, ">=");
				}
			case '!':
				if (this._tokens[this._idx].value === '=') {
					this._idx++;
					return new Token(TokenType.NE, op.pos, "!=");
				}
		}

		op.super = TokenType.OPERATOR;
		return op;
	}
	
	next(): Token | null {
		if (this._idx >= this._tokens.length) return null;
		else if (this._tokens[this._idx].type === TokenType.NUMBER) return this.handleNumber();
		else if (this._tokens[this._idx].type === TokenType.LITERAL) return this.handleLiteral();
		else if (['{', '<', '=', '>', '!'].includes(this._tokens[this._idx].value[0])) return this.handleOperator();
		else if (this._tokens[this._idx].type === TokenType.COMMENT_LINE || this._tokens[this._idx].type === TokenType.COMMENT_BLOCK) {
			this._tokens[this._idx].super = TokenType.COMMENT;
			return this._tokens[this._idx++];
		}
		return this._tokens[this._idx++];
	}

	static process(tokens: Token[], comment: boolean = false): Token[] {
		const procTokens: Token[] = [];
		const processor = new TokenProcessor(tokens);

		while (processor._idx < processor._tokens.length) {
			const token = processor.next();

			if (token === null) 
				throw new Error(`Wrong token located at index '${processor._idx}'`);

			if (token.super !== TokenType.COMMENT || comment)
				procTokens.push(token);
		}

		return procTokens;
	}
}