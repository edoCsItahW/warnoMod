#! /user/bin/python3

#  Copyright (c) 2024. All rights reserved.
#  This source code is licensed under the CC BY-NC-SA
#  (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
#  This software is protected by copyright law. Reproduction, distribution, or use for commercial
#  purposes is prohibited without the author's permission. If you have any questions or require
#  permission, please contact the author: 2207150234@st.sziit.edu.cn

# -------------------------<edocsitahw>----------------------------
# file: _type.py
# author: edocsitahw
# data: 2024/12/2 下午2:33
# desc:
# -------------------------<edocsitahw>----------------------------

from typing import TypedDict, Generic, TypeVar, List, Optional, Union, ForwardRef


class ASTJson(TypedDict):
    nodeName: str


class IdentifierJson(ASTJson):
    name: str


ExpressionJson = Union[
    IdentifierJson
    | ForwardRef('EnumRefJson')
    | ForwardRef('MapRefJson')
    | ForwardRef('TemplateParamJson')
    | ForwardRef('OperatorJson')
    | ForwardRef('OperationJson')
    | ForwardRef('ObjectRefJson')
    | ForwardRef('ObjectInsJson')
    | ForwardRef('TemplateRefJson')
    ]


class PairJson(ASTJson):
    first: ExpressionJson
    second: ExpressionJson


class MapRefJson(ASTJson):
    pairs: List[PairJson]


class EnumRefJson(ASTJson):
    enumName: IdentifierJson
    enumValue: IdentifierJson


class ParameterJson(ASTJson):
    identifier: IdentifierJson
    type: Optional[IdentifierJson]
    expression: Optional[ExpressionJson]


class TemplateParamJson(ASTJson):
    identifier: IdentifierJson


class OperatorJson(ASTJson):
    value: str


class OperationJson(ASTJson):
    left: ExpressionJson
    operator: OperatorJson
    right: ExpressionJson


class ObjectRefJson(ASTJson):
    identifier: IdentifierJson


class TemplateRefJson(ASTJson):
    template: TemplateParamJson


class MemberJson(ASTJson):
    identifier: IdentifierJson
    expression: ExpressionJson


class ObjectInsJson(ASTJson):
    identifier: IdentifierJson
    members: List[MemberJson]


class AssignmentJson(ASTJson):
    identifier: IdentifierJson
    expression: ExpressionJson


class MapDefJson(ASTJson):
    pairs: List[PairJson]


class TemplateDefJson(ASTJson):
    identifier: IdentifierJson
    type: IdentifierJson
    parameters: List[ParameterJson]
    members: List[MemberJson]


class ObjectDefJson(ASTJson):
    identifier: IdentifierJson
    type: IdentifierJson
    members: List[MemberJson]


class ExportJson(ASTJson):
    statement: ObjectDefJson | AssignmentJson | MapDefJson | TemplateDefJson | ForwardRef('ExportJson')


class ProgramJson(ASTJson):
    statements: ObjectDefJson | AssignmentJson | MapDefJson | TemplateDefJson | ExportJson


class PathJson(ASTJson):
    value: str


class StringJson(ASTJson):
    value: str


class IntegerJson(ASTJson):
    value: int


class FloatJson(ASTJson):
    value: float


class BooleanJson(ASTJson):
    value: bool


class NilJson(ASTJson):
    value: str


class GUIDJson(ASTJson):
    value: str


class VectorJson(ASTJson):
    value: List[ExpressionJson]
