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
import jsonAst as jast
from _types import ProgramJson
from json import dumps


def findAllDecks(_ast: ProgramJson) -> list:
    decks = set()

    ast = jast.Program(_ast)

    for s in ast.statements:
        if isinstance(s, jast.ObjectDef):
            for m in s.members:
                if m.identifier.name == 'PackList':
                    for p in m.expression.pairs:
                        decks.add(p.first.value)

    return list(decks)


def findReleationOfDeckAndUnit(_ast: ProgramJson) -> dict:
    relaDict = {}

    ast = jast.Program(_ast)

    for s in ast.statements:
        key = None
        if isinstance(s, jast.ObjectDef):
            key = s.identifier.name
            for m in s.members:
                if m.identifier.name == 'Unit' and isinstance(m.expression, jast.Path):
                    relaDict[key] = m.expression.value

    return relaDict


if __name__ == '__main__':
    from ndfPyAPI import parseAST

    # decks = list(map(lambda x: x.replace('~/', ''), findAllDecks(eval(parseAST(r"E:\codeSpace\codeSet\ndf\warnoMod\GameData\Generated\Gameplay\Decks\Divisions.ndf")))))

    maps = findReleationOfDeckAndUnit(eval(parseAST(r"E:\codeSpace\codeSet\ndf\warnoMod\GameData\Generated\Gameplay\Decks\DivisionPacks.ndf")))

    # with open('releationOfDeckAndUnit.json', 'w', encoding='utf-8') as file:
    #     file.write(dumps(maps, indent=4))

    # for deck in decks:
    #     if deck not in maps:
    #         print(f"deck {deck} not found in maps")

    for v in maps.values():
        node = jast.ObjectIns(identifier=jast.Identifier(name='TDeckUniteRule'), members=[
            jast.Member(identifier=jast.Identifier(name='UnitDescriptor'), expression=jast.Path(value=v)),
            jast.Member(identifier=jast.Identifier(name='AvailableWithoutTransport'), expression=jast.Boolen(value=True)),
            jast.Member(identifier=jast.Identifier(name='NumberOfUnitInPack'), expression=jast.Integer(value=999)),
            jast.Member(identifier=jast.Identifier(name='NumberOfUnitInPackXPMultiplier'), expression=jast.Vector(expressions=[jast.Integer(value=1)] * 4))
        ])

        print(node.code())
