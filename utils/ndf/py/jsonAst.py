#! /user/bin/python3

#  Copyright (c) 2024. All rights reserved.
#  This source code is licensed under the CC BY-NC-SA
#  (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
#  This software is protected by copyright law. Reproduction, distribution, or use for commercial
#  purposes is prohibited without the author's permission. If you have any questions or require
#  permission, please contact the author: 2207150234@st.sziit.edu.cn

# -------------------------<edocsitahw>----------------------------
# file: jsonAst.py
# author: edocsitahw
# data: 2024/11/29 下午1:34
# desc:
# -------------------------<edocsitahw>----------------------------


__all__ = [
    'AST',
    'Program',
    'Statement',
    'Expression',
    'Literal',
    'Identifier',
    'ObjectDef',
    'Member',
    'Pair',
    'Vector',
    'MapRef',
    'ObjectIns',
    'TemplateRef',
    'TemplateDef',
    'TemplateParam',
    'Operator',
    'Operation',
    'Boolen',
    'Integer',
    'Float',
    'String',
    'Nil',
    'EnumRef',
    'Assignment',
    'GUID',
    'Parameter',
    'Export',
    'ObjectRef'
]


from json import loads
from typing import Literal as TLiteral, Self, final, Type, Callable, List, ForwardRef, Any, overload, Iterable, NoReturn
from abc import ABC, abstractmethod
from functools import cached_property, wraps
from types import GenericAlias
from _types import *

_supEncode = TLiteral['utf-8', 'gbk', 'utf-16', 'ascii', 'latin1', 'cp1252']


def camelCase(_s: str):
    if len(_s) <= 1:
        return _s.lower()
    return _s[0].upper() + _s[1:]


def assigmRetrun(_ins: Any, **kwargs: Any) -> Any:
    for k, v in kwargs.items():
        setattr(_ins, k, v)
    return _ins


NEWLINE = '\n'
IDT = 4 * ' '


class SyncList(list):
    def __init__(self, jsonData: list, iterable: Iterable):
        super().__init__(iterable)
        self.__dict__['_jsonData'] = self._jsonData = jsonData

    def _sync(self, _fn: Callable[[...], ...]) -> Callable[[...], Any]:
        @wraps(_fn)
        def wrapper(*args, **kwargs) -> Any:
            mth = getattr(self._jsonData, _fn.__name__)
            mth(*map(lambda _: _.data if isinstance(_, AST) else _, args), **{k: v.data if isinstance(v, AST) else v for k, v in kwargs.items()})
            return _fn(*args, **kwargs)

        return wrapper

    def __getattribute__(self, item: str) -> Any:
        if item in ['_jsonData', '_sync']:
            return super().__getattribute__(item)
        if (mthOrattr := super().__getattribute__(item)) and callable(mthOrattr):
            return self._sync(mthOrattr)
        return mthOrattr


class AST(ABC):
    """
    AST各节点情况分析:

    1. 抽象节点类:
        如: Statement, Expression, Literal

        * 该类保持抽象,从不实例化,当涉及该类时则解析其子节点
        * 无需实现parse方法
        * 在任何parse中都不应直接实例化该类,而是通过读取传入json数据的nodeName字段来确定具体的子类

    2. 具体节点类:
        如: Program, ObjectDef, Identifier

        * 该类实现parse方法,解析其子节点

        * 子类情况分析:
            1. 表类型属性:
                如: Program.statements, ObjectDef.members
                传入autoHandle(在autoHandle中具体分析):

                1. 无需更名情况: autoHandle('某个子类名', jsonData, statements=list)
                2. 需更名情况: autoHandle('某个子类名', jsonData, members=List['memberList'])

            2. 单类型属性:
                1. 对应抽象节点类: 如: expression
                    传入autoHandle: autoHandle('expression', jsonData)  # 直接不传如关键字参数
                2. 对应具体节点类: 如: identifier
                    传入autoHandle: autoHandle('identifier', jsonData, identifier=Identifier)
    """
    Vector = True
    MapRef = True
    ObjectIns = True

    @final
    @property
    def nodeName(self) -> str:
        if r := self._orgData.get('nodeName'):
            return r
        raise AttributeError(
            "No 'nodeName' found in the json data.")

    @final
    @property
    def data(self) -> dict:
        return self._orgData

    @overload
    def __init__(self, jsonData: ASTJson): ...

    @overload
    def __init__(self, **kwargs: 'AST'): ...

    def __init__(self, *args, **kwargs) -> None:
        # 尽量在该条件语句后定义其它属性,以免出现__setattr__时未定义的属性
        if len(args) == 1 and isinstance(args[0], dict):
            self.__dict__['_orgData'] = self._orgData = args[0]
        elif len(kwargs) > 0:
            self.__dict__['_orgData'] = self._orgData = {}
            for k, v in kwargs.items():
                setattr(self, k, v)
                self._orgData[k] = v.data if isinstance(v, AST) else v
            self._orgData['nodeName'] = self.__class__.__name__
        else:
            raise TypeError(
                "Either a json data or keyword arguments must be provided.")

        self.indent = getattr(self, 'indent', 0)

    @final
    def __getitem__(self, item: str) -> Self | list[Self]:
        if item in (v := self.__dict__['_orgData']):
            return self.parse(item)
        return getattr(self, item)

    @final
    def __getattr__(self, item: str) -> Self | list[Self]:
        return self.parse(item)

    @final
    def __setattr__(self, key: str, value: Any) -> None:
        if key != '_orgData' and key in self._orgData:
            self._orgData[key] = value
        super().__setattr__(key, value)

    @final
    def __str__(self) -> str:
        return self.__repr__() if isinstance(self._orgData, dict) else str(self._orgData)

    @final
    def __repr__(self) -> str:
        return f"<{self.__class__.__mro__[1].__name__}::{self.__class__.__name__} at {hex(id(self)).upper()}>"

    def createIns(self, _type: type | str, _data: dict):
        if isinstance(_type, str):
            return globals().get(camelCase(_type))(_data)

        elif isinstance(_type, type):
            if not hasattr(_type, '__annotations__'):
                return _data
            elif _type.__annotations__:  # 具体类
                return _type(_data)
            else:  # 抽象类 {nodeName: 'MapRef', pairs: [{nodeName: 'Pair', first: {...}, second: {...}}]}
                if cls := globals().get(_data['nodeName']):
                    return cls(_data)
                raise NotImplementedError(
                    f"No implementation for class name '{_data['nodeName']}' yet.")

    def parse(self, key: str):
        """
        1. attr: T
            {nodeName: 'Assignment', identifier: {...}}
        2. attr: List[T]
            {nodeName: 'ObjectDef', members: [T, T, ...]}
        3. attr: str
            {nodeName: 'Identifier', name: 'foo'}
        :param key:
        :return:
        """
        if key in self.__class__.__annotations__:  # 注册了属性
            typeOrAnno: type | Type[list] | str = self.__class__.__annotations__[key]

            if typeOrAnno.__class__.__name__ == '_GenericAlias':  # 当属性类型为List[T]时
                if typeOrAnno.__args__[0].__class__ == ForwardRef:  # 若T为ForwardRef,且保留IndexError
                    # {nodeName: 'ObjectDef', members: [{nodeName: 'Member', identifier: {...}, expression: {...}}]}
                    return SyncList(self._orgData[key], (assigmRetrun(self.createIns(typeOrAnno.__args__[0].__forward_arg__, _), indent=self._indent()) for _ in self._orgData[key]))  # {nodeName: 'Member', identifier: {...}, expression: {...}}

                elif isinstance(typeOrAnno.__args__[0], type):  # 若T为具体类
                    # {nodeName: 'ObjectDef', members: [{nodeName: 'Member', identifier: {...}, expression: {...}}]}
                    return SyncList(self._orgData[key], (assigmRetrun(self.createIns(typeOrAnno.__args__[0], _), indent=self._indent()) for _ in self._orgData[key]))  # {nodeName: 'Member', identifier: {...}, expression: {...}}

            elif isinstance(typeOrAnno, GenericAlias):  # 当属性类型为list[T]时
                raise TypeError(
                    f"Please use 'List[T]' instead of 'list[T]' for attribute '{key}' in '{self.__class__.__name__}'.")

            elif isinstance(typeOrAnno, type):  # str
                # {nodeName: 'Member', identifier: {...}, expression: {...}}
                res = self.createIns(typeOrAnno, self._orgData[key])
                if isinstance(res, AST):
                    res.indent = self._indent()
                return res  # {nodeName: 'Identifier', name: 'foo'}

        elif key in (v := self.__dict__['_orgData']):
            return v[key]

        else:
            raise AttributeError(
                f"No implementation for attribute '{key}' in '{self.__class__.__name__}'.")

    @abstractmethod
    def code(self) -> str:
        ...

    def _indent(self) -> int:
        return self.indent + 1

    def _newline(self) -> str:
        return NEWLINE if getattr(AST, self.__class__.__name__, False) else ''

    @final
    def _idt(self) -> str:
        return IDT * self.indent


def readJson(pth: str, *, encoding: _supEncode = 'utf-8') -> dict:
    with open(pth, 'r', encoding=encoding) as jf:
        return loads(jf.read())


class Statement(AST): ...


class Program(AST):
    """
    预分析:
        - 对于

    """
    statements: List[Statement]

    @overload
    def __init__(self, jsonData: ProgramJson): ...

    @overload
    def __init__(self, *, statements: List[Statement]): ...

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def _indent(self) -> int:
        return self.indent

    def code(self) -> str:
        return ''.join(s.code() for s in self.statements)


class Expression(AST): ...


class Identifier(Expression):
    name: str

    @overload
    def __init__(self, jsonData: IdentifierJson): ...

    @overload
    def __init__(self, *, name: str): ...

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def code(self) -> str:
        return self.name


class EnumRef(Expression):
    enumName: Identifier
    enumValue: Identifier

    @overload
    def __init__(self, jsonData: EnumRefJson): ...

    @overload
    def __init__(self, *, enumName: Identifier, enumValue: Identifier): ...

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def code(self) -> str:
        return f"{self.enumName.code()}/{self.enumValue.code()}"


class Member(AST):
    identifier: Identifier
    expression: Expression

    @overload
    def __init__(self, jsonData: MemberJson): ...

    @overload
    def __init__(self, *, identifier: Identifier, expression: Expression): ...

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def _indent(self) -> int:
        return self.indent

    def code(self) -> str:
        return f"{self._idt()}{self.identifier.code()} = {self.expression.code()}"


class ObjectDef(Statement):
    identifier: Identifier
    type: Identifier
    members: List[Member]

    @overload
    def __init__(self, jsonData: ObjectDefJson): ...

    @overload
    def __init__(self, *, identifier: Identifier, type: Identifier, members: List[Member]): ...

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def code(self) -> str:
        return f"{self.identifier.code()} is {self.type.code()}(\n{NEWLINE.join(m.code() for m in self.members)}\n)\n"


class Assignment(Statement):
    identifier: Identifier
    expression: Expression

    @overload
    def __init__(self, jsonData: AssignmentJson): ...

    @overload
    def __init__(self, *, identifier: Identifier, expression: Expression): ...

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def code(self) -> str:
        return f"{self.identifier.code()} is {self.expression.code()}\n"


class Pair(AST):
    first: Expression
    second: Expression

    @overload
    def __init__(self, jsonData: PairJson): ...

    @overload
    def __init__(self, *, first: Expression, second: Expression): ...

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def code(self) -> str:
        return f"{self._idt()}({self.first.code()}, {self.second.code()})"


class MapRef(Expression):
    pairs: List[Pair]

    @overload
    def __init__(self, jsonData: MapRefJson): ...

    @overload
    def __init__(self, *, pairs: List[Pair]): ...

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def _newline(self) -> str:
        return super()._newline() if len(self._orgData['pairs']) > 3 else ''

    def code(self) -> str:
        return f"MAP [{self._newline()}{(', ' + self._newline()).join(p.code() for p in self.pairs)}{self._newline()}{self._idt()}]"


class Literal(Expression): ...


class Path(Literal):
    value: str

    @overload
    def __init__(self, jsonData: PathJson): ...

    @overload
    def __init__(self, *, value: str): ...

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def code(self) -> str:
        return self.value


class String(Literal):
    value: str

    @overload
    def __init__(self, jsonData: StringJson): ...

    @overload
    def __init__(self, *, value: str): ...

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def code(self) -> str:
        return f'"{self.value}"'


class Integer(Literal):
    value: int

    @overload
    def __init__(self, jsonData: IntegerJson): ...

    @overload
    def __init__(self, *, value: int): ...

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def code(self) -> str:
        return str(self.value)


class Float(Literal):
    value: float

    @overload
    def __init__(self, jsonData: FloatJson): ...

    @overload
    def __init__(self, *, value: float): ...

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def code(self) -> str:
        return str(self.value)


class GUID(Literal):
    value: str

    @overload
    def __init__(self, jsonData: GUIDJson): ...

    @overload
    def __init__(self, *, value: str): ...

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def code(self) -> str:
        return f"GUID:{{{self.value}}}"


class Vector(Literal):
    expressions: List[Expression]

    @overload
    def __init__(self, jsonData: VectorJson): ...

    @overload
    def __init__(self, *, expressions: List[Expression]): ...

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def _newline(self) -> str:
        return super()._newline() if len(self._orgData['expressions']) > 10 or any(k['nodeName'] == 'ObjectIns' for k in self._orgData['expressions']) else ''

    def code(self) -> str:
        return f"[{self._newline()}{', '.join(e.code() for e in self.expressions)}{self._newline()}{self._idt() if self._newline() else ''}]"


class MapDef(Statement):
    pairs: List[Pair]

    @overload
    def __init__(self, jsonData: MapDefJson): ...

    @overload
    def __init__(self, *, pairs: List[Pair]): ...

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def code(self) -> str:
        return f"MAP [\n{f',{NEWLINE}'.join(p.code() for p in self.pairs)}]"


class Parameter(AST):
    identifier: Identifier
    type: Optional[Identifier]
    expression: Optional[Expression]

    @overload
    def __init__(self, jsonData: ProgramJson): ...

    @overload
    def __init__(self, *, identifier: Identifier, type: Optional[Identifier] = None, expression: Optional[Expression] = None): ...

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def code(self) -> str:
        return f"{self._idt()}{self.identifier.code()}{f': {self.type.code()}' if self.type else ''}{f' = {self.expression.code()}' if self.expression else ''}"


class TemplateDef(Statement):
    identifier: Identifier
    type: Identifier
    parameters: List[Parameter]
    members: List[Member]

    @overload
    def __init__(self, jsonData: TemplateDefJson): ...

    @overload
    def __init__(self, *, identifier: Identifier, type: Identifier, parameters: List[Parameter], members: List[Member]): ...

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def code(self) -> str:
        return f"template {self.identifier.code()} [\n{f',{NEWLINE}'.join(p.code() for p in self.parameters)}\n] is (\n{NEWLINE.join(m.code() for m in self.members)})"


class Export(Statement):
    statement: Statement

    @overload
    def __init__(self, jsonData: ExportJson): ...

    @overload
    def __init__(self, *, statement: Statement): ...

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def code(self) -> str:
        return f"export {self.statement.code()}"


class Boolen(Literal):
    value: bool

    @overload
    def __init__(self, jsonData: BooleanJson): ...

    @overload
    def __init__(self, *, value: bool): ...

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def code(self) -> str:
        return str(self.value)


class Nil(Literal):
    value: str

    @overload
    def __init__(self, jsonData: NilJson): ...

    @overload
    def __init__(self, *, value: str): ...

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def code(self) -> str:
        return "nil"


class TemplateParam(Expression):
    identifier: Identifier

    @overload
    def __init__(self, jsonData: TemplateParamJson): ...

    @overload
    def __init__(self, *, identifier: Identifier): ...

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def code(self) -> str:
        return f"<{self.identifier.code()}>"


class Operator(Expression):
    value: str

    @overload
    def __init__(self, jsonData: OperatorJson): ...

    @overload
    def __init__(self, *, value: str): ...

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def code(self) -> str:
        return self.value


class Operation(Expression):
    left: Expression
    operator: Operator
    right: Expression

    @overload
    def __init__(self, jsonData: OperationJson): ...

    @overload
    def __init__(self, *, left: Expression, operator: Operator, right: Expression): ...

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def code(self) -> str:
        return f"{self.left.code()} {self.operator.code()} {self.right.code()}"


class ObjectRef(Expression):
    identifier: Identifier

    @overload
    def __init__(self, jsonData: ObjectRefJson): ...

    @overload
    def __init__(self, *, identifier: Identifier): ...

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def code(self) -> str:
        return f"$/{self.identifier.code()}"


class ObjectIns(Expression):
    identifier: Identifier
    members: List[Member]

    @overload
    def __init__(self, jsonData: ObjectInsJson): ...

    @overload
    def __init__(self, *, identifier: Identifier, members: List[Member]): ...

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def _newline(self) -> str:
        return super()._newline() if len(self._orgData['members']) else ''

    def code(self) -> str:
        return f"{self._idt()}{self.identifier.code()}({self._newline()}{NEWLINE.join(m.code() for m in self.members)}{self._newline()}{self._idt() if self.members else ''})"


class TemplateRef(Expression):
    identifier: Identifier

    @overload
    def __init__(self, jsonData: TemplateRefJson): ...

    @overload
    def __init__(self, *, identifier: Identifier): ...

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def code(self) -> str:
        return f"template {self.identifier.code()}"

def sepcialSum(_start: int, _end: int) -> int:
    if _end < _start: raise ValueError
