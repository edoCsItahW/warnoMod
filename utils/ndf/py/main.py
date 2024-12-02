#! /user/bin/python3

#  Copyright (c) 2024. All rights reserved.
#  This source code is licensed under the CC BY-NC-SA
#  (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
#  This software is protected by copyright law. Reproduction, distribution, or use for commercial
#  purposes is prohibited without the author's permission. If you have any questions or require
#  permission, please contact the author: 2207150234@st.sziit.edu.cn

# -------------------------<edocsitahw>----------------------------
# file: main.py
# author: edocsitahw
# data: 2023/10/20 20:01
# desc:
# -------------------------<edocsitahw>----------------------------
from jsonAst import AST, Program, Member, ObjectDef, readJson, SyncList


def modifyByJson(_ast: dict) -> str:
    divDict = {}

    for s in _ast['statements']:
        for m in s['members']:
            if m['identifier']['name'] == 'PackList':
                for p in m['expression']['pairs']:
                    divDict[p['first']['value']] = p

    _ast['statements'][0]['members'][7]['expression']['pairs'] = []

    for i, p in enumerate(divDict.values(), 1):
        p['second']['value'] = i
        _ast['statements'][0]['members'][7]['expression']['pairs'].append(p)

    _ast = Program(_ast)

    return _ast.code()


def modifyByAst(_ast: dict) -> str:
    _ast = Program(_ast)

    divDict = {}

    for s in _ast.statements:
        if isinstance(s, ObjectDef):
            for m in s.members:
                if m.identifier.name == 'PackList':
                    for p in m.expression.pairs:
                        divDict[p.first.value] = p

    _ast.statements[0].members[7].expression.pairs = []

    for i, p in enumerate(divDict.values(), 1):
        p.second.value = i
        _ast.statements[0].members[7].expression.pairs.append(p)

    return _ast.code()


if __name__ == '__main__':
    ast = readJson(r"..\ast.json")

    print(modifyByAst(ast))
