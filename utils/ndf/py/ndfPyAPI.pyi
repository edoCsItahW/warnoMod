#  Copyright (c) 2024. All rights reserved.
#  This source code is licensed under the CC BY-NC-SA
#  (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
#  This software is protected by copyright law. Reproduction, distribution, or use for commercial
#  purposes is prohibited without the author's permission. If you have any questions or require
#  permission, please contact the author: 2207150234@st.sziit.edu.cn

from enum import Enum
from typing import Self, Any
from _types import ProgramJson

class TokenType(Enum):
    NUMBER: int
    INT: int
    FLOAT: int
    STRING: int
    PATH: int
    LITERAL: int
    IDENTIFIER: int
    KW_IS: int
    KW_MAP: int
    KW_DIV: int
    KW_TEMPLATE: int
    KW_EXPORT: int
    KW_UNNAMED: int
    KW_NIL: int
    KW_PRIVATE: int
    KW_BOOLEN: int
    GUID: int
    OPERATOR: int
    ADD: int
    SUB: int
    MUL: int
    DIV: int
    MOD: int
    ASSIGN: int
    LT: int
    GT: int
    LE: int
    GE: int
    EQ: int
    NE: int
    OR: int
    NOT: int
    LPAREN: int
    RPAREN: int
    LBRACKET: int
    RBRACKET: int
    LBRACE: int
    RBRACE: int
    COMMA: int
    SEMICOLON: int
    COLON: int
    DOT: int
    DOLLAR: int
    QUESTION: int
    TILDE: int
    COMMENT: int
    NEWLINE: int
    EOF: int
    ERROR: int
    UNKNOWN: int

def dereference(t: Any) -> Any: ...

class Pos:
    def __init__(self, line: int, col: int, value: int): ...

    lind = property(lambda self: int)
    col = property(lambda self: int)

    def __call__(self) -> int: ...
    def increment(self) -> Self: ...
    def incrementPostfix(self) -> Self: ...
    def decrement(self) -> Self: ...
    def decrementPostfix(self) -> Self: ...
    def newLine(self) -> Self: ...
    def __iadd__(self, other: Self | int) -> Self: ...
    def __isub__(self, other: Self | int) -> Self: ...
    def __lt__(self, other: Self | int) -> bool: ...
    def __gt__(self, other: Self | int) -> bool: ...
    def __le__(self, other: Self | int) -> bool: ...
    def __ge__(self, other: Self | int) -> bool: ...
    def __eq__(self, other: Self | int) -> bool: ...
    def __ne__(self, other: Self | int) -> bool: ...
    def __add__(self, other: int) -> int: ...
    def __sub__(self, other: int) -> int: ...


class Token:
    type: TokenType
    pos: Pos
    value: str
    super: TokenType

g_Level10p: set[str]


class Lexer:
    def __init__(self, source: str): ...
    def next(self) -> Token: ...
    @staticmethod
    def tokenize(text: str) -> list[Token]: ...

class TokenProcessor:
    def __init__(self, tokens: list[Token]): ...
    def next(self) -> Token: ...
    @staticmethod
    def process(tokens: list[Token]) -> list[Token]: ...



class Statement: ...


class Program:
    statements: list[Statement]


class Parser:
    def __init__(self, tokens: list[Token]): ...
    def parse(self, debug: bool = False) -> Program: ...


def parse(text: str) -> Program: ...

def parseAST(_target: str, _cwd: str = ".") -> str: ...
