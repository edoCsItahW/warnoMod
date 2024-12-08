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
from json import dumps, loads, load
from typing import Any


def findAllDecks(_ast: ProgramJson) -> list:
    """
    zh: 找到所有的卡组
    en: Find all decks

    :param _ast: {zh: json格式的AST, en: json format AST}
    :return: {zh: 所有卡组的列表, en: list of all decks}
    """
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
    """
    zh: 找到卡组与单位的关系
    en: Find the relationship between decks and units

    :param _ast: {zh: json格式的AST, en: json format AST}
    :return: {zh: 卡组与单位的关系字典, en: dictionary of deck and unit relationship}
    """
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


def getAllTransports(_ast: ProgramJson) -> list:
    """
    zh: 找到所有的运输方式
    en: Find all transports

    :param _ast: {zh: json格式的AST, en: json format AST}
    :return: {zh: 所有运输方式的列表, en: list of all transports}
    """
    ast = jast.Program(_ast)

    transports = set()
    # AvailableTransportList

    rules: jast.MapRef = ast.statements[0].statement.members[0].expression

    for p in rules.pairs:
        if isinstance(p.second, jast.ObjectIns):
            UnitTransportList: jast.Vector = p.second.members[0].expression
            for e in UnitTransportList.expressions:
                if isinstance(e, jast.ObjectIns):
                    for m in e.members:
                        if m.identifier.name == 'AvailableTransportList':
                            for e2 in m.expression.expressions:
                                transports.add(e2.value)

    return list(transports)


def buildDivisionRules() -> str:
    """
    zh: 构建师规则
    en: Build division rules

    :param _ast: {zh: json格式的AST, en: json format AST}
    :return: {zh: 师规则的字符串, en: string of division rules}
    """
    relaDict = {v: {} for k, v in findReleationOfDeckAndUnit(eval(parseAST(r"E:\codeSpace\codeSet\ndf\warnoMod\GameData\Generated\Gameplay\Decks\DivisionPacks.ndf"))).items()}

    # allTransports = ['$/GFX/Unit/Descriptor_Unit_Alouette_II_trans_FR', '$/GFX/Unit/Descriptor_Unit_BTR_60_SOV', '$/GFX/Unit/Descriptor_Unit_VLRA_HMG_FR', '$/GFX/Unit/Descriptor_Unit_BMP_1P_reco_DDR', '$/GFX/Unit/Descriptor_Unit_M1038_Humvee_US', '$/GFX/Unit/Descriptor_Unit_UH1D_RFA', '$/GFX/Unit/Descriptor_Unit_UAZ_469_MP_SOV', '$/GFX/Unit/Descriptor_Unit_FV120_Spartan_MCT_UK', '$/GFX/Unit/Descriptor_Unit_Mi_2_trans_SOV', '$/GFX/Unit/Descriptor_Unit_Mi_24D_Desant_SOV', '$/GFX/Unit/Descriptor_Unit_Marder_1A3_RFA', '$/GFX/Unit/Descriptor_Unit_BTR_70_MP_SOV', '$/GFX/Unit/Descriptor_Unit_GAZ_66_SOV', '$/GFX/Unit/Descriptor_Unit_LuAZ_967M_AA_VDV_SOV', '$/GFX/Unit/Descriptor_Unit_Mi_8TV_non_arme_SOV', '$/GFX/Unit/Descriptor_Unit_M2_Bradley_IFV_NG_US', '$/GFX/Unit/Descriptor_Unit_UAZ_469_trans_DDR', '$/GFX/Unit/Descriptor_Unit_M1025_Humvee_AGL_US', '$/GFX/Unit/Descriptor_Unit_Gama_Goat_trans_US', '$/GFX/Unit/Descriptor_Unit_LandRover_UK', '$/GFX/Unit/Descriptor_Unit_GAZ_66B_SOV', '$/GFX/Unit/Descriptor_Unit_Marder_1A2_MILAN_RFA', '$/GFX/Unit/Descriptor_Unit_BMP_2D_reco_SOV', '$/GFX/Unit/Descriptor_Unit_AMX_13_VCI_20mm_FR', '$/GFX/Unit/Descriptor_Unit_BMP_2D_SOV', '$/GFX/Unit/Descriptor_Unit_BTR_70_AGS_SOV', '$/GFX/Unit/Descriptor_Unit_M2A1_Bradley_IFV_US', '$/GFX/Unit/Descriptor_Unit_M151A2_scout_US', '$/GFX/Unit/Descriptor_Unit_BMD_2_SOV', '$/GFX/Unit/Descriptor_Unit_BTR_70_S5_SOV', '$/GFX/Unit/Descriptor_Unit_PSzH_IV_DDR', '$/GFX/Unit/Descriptor_Unit_M113_ACAV_US', '$/GFX/Unit/Descriptor_Unit_Star_266_POL', '$/GFX/Unit/Descriptor_Unit_UAZ_469_Reco_SOV', '$/GFX/Unit/Descriptor_Unit_Mi_2_trans_DDR', '$/GFX/Unit/Descriptor_Unit_FV603_Saracen_UK', '$/GFX/Unit/Descriptor_Unit_VAB_Reserve_FR', '$/GFX/Unit/Descriptor_Unit_BMP_1P_DDR', '$/GFX/Unit/Descriptor_Unit_BTR_D_Robot_SOV', '$/GFX/Unit/Descriptor_Unit_GTMU_1D_SOV', '$/GFX/Unit/Descriptor_Unit_MCV_80_Warrior_MILAN_ERA_UK', '$/GFX/Unit/Descriptor_Unit_M35_trans_DDR', '$/GFX/Unit/Descriptor_Unit_VAB_T20_FR', '$/GFX/Unit/Descriptor_Unit_Lynx_AH_Mk7_SNEB_UK', '$/GFX/Unit/Descriptor_Unit_FV432_UK', '$/GFX/Unit/Descriptor_Unit_M1025_Humvee_AGL_nonPara_US', '$/GFX/Unit/Descriptor_Unit_UAZ_469_SOV', '$/GFX/Unit/Descriptor_Unit_BTR_70_DDR', '$/GFX/Unit/Descriptor_Unit_M113A1G_RFA', '$/GFX/Unit/Descriptor_Unit_MTLB_transp_SOV', '$/GFX/Unit/Descriptor_Unit_Lynx_AH_Mk1_UK', '$/GFX/Unit/Descriptor_Unit_MH47D_Super_Chinook_US', '$/GFX/Unit/Descriptor_Unit_Sonderwagen_4_RFA', '$/GFX/Unit/Descriptor_Unit_BMP_3_SOV', '$/GFX/Unit/Descriptor_Unit_GTMU_1D_AGS_SOV', '$/GFX/Unit/Descriptor_Unit_Lynx_AH_Mk1_LBH_UK', '$/GFX/Unit/Descriptor_Unit_MTLB_trans_DDR', '$/GFX/Unit/Descriptor_Unit_BTR_D_SOV', '$/GFX/Unit/Descriptor_Unit_M1025_Humvee_scout_US', '$/GFX/Unit/Descriptor_Unit_M113A1G_MILAN_RFA', '$/GFX/Unit/Descriptor_Unit_M998_Humvee_US', '$/GFX/Unit/Descriptor_Unit_BMP_2AG_SOV', '$/GFX/Unit/Descriptor_Unit_FV103_Spartan_UK', '$/GFX/Unit/Descriptor_Unit_VAB_MILAN_FR', '$/GFX/Unit/Descriptor_Unit_BTR_60_DDR', '$/GFX/Unit/Descriptor_Unit_M113A1_NG_US', '$/GFX/Unit/Descriptor_Unit_UAZ_469_AGL_Grenzer_DDR', '$/GFX/Unit/Descriptor_Unit_Mi_8T_DDR', '$/GFX/Unit/Descriptor_Unit_BTR_ZD_Skrezhet_SOV', '$/GFX/Unit/Descriptor_Unit_Gazelle_trans_UK', '$/GFX/Unit/Descriptor_Unit_CH47_Chinook_US', '$/GFX/Unit/Descriptor_Unit_BMP_1P_SOV', '$/GFX/Unit/Descriptor_Unit_MAN_Kat_6x6_trans_RFA', '$/GFX/Unit/Descriptor_Unit_BMP_2_reco_SOV', '$/GFX/Unit/Descriptor_Unit_VAB_FR', '$/GFX/Unit/Descriptor_Unit_M113A1G_reco_RFA', '$/GFX/Unit/Descriptor_Unit_BMP_2_SOV', '$/GFX/Unit/Descriptor_Unit_Alouette_III_FR', '$/GFX/Unit/Descriptor_Unit_BMP_1PG_SOV', '$/GFX/Unit/Descriptor_Unit_VIB_FR', '$/GFX/Unit/Descriptor_Unit_BTR_60_reco_SOV', '$/GFX/Unit/Descriptor_Unit_GTMU_1D_SPG9_SOV', '$/GFX/Unit/Descriptor_Unit_Marder_1A3_MILAN_RFA', '$/GFX/Unit/Descriptor_Unit_BMD_1P_SOV', '$/GFX/Unit/Descriptor_Unit_AMX_13_VCI_12_7mm_FR', '$/GFX/Unit/Descriptor_Unit_Mi_8MTV_SOV', '$/GFX/Unit/Descriptor_Unit_BTR_152A_DDR', '$/GFX/Unit/Descriptor_Unit_BTR_70_S8_SOV', '$/GFX/Unit/Descriptor_Unit_M1025_Humvee_MP_US', '$/GFX/Unit/Descriptor_Unit_MCV_80_Warrior_UK', '$/GFX/Unit/Descriptor_Unit_M274_Mule_M2HB_US', '$/GFX/Unit/Descriptor_Unit_TPZ_Fuchs_1_RFA', '$/GFX/Unit/Descriptor_Unit_BTR_80_SOV', '$/GFX/Unit/Descriptor_Unit_LUAZ_967M_AGL_VDV_SOV', '$/GFX/Unit/Descriptor_Unit_BRDM_1_DDR', '$/GFX/Unit/Descriptor_Unit_Marder_1A2_RFA', '$/GFX/Unit/Descriptor_Unit_UH60A_Black_Hawk_US', '$/GFX/Unit/Descriptor_Unit_Iltis_trans_RFA', '$/GFX/Unit/Descriptor_Unit_Ural_4320_Metla_SOV', '$/GFX/Unit/Descriptor_Unit_LUAZ_967M_AGL_SOV', '$/GFX/Unit/Descriptor_Unit_TRM_10000_FR', '$/GFX/Unit/Descriptor_Unit_W50_LA_A_DDR', '$/GFX/Unit/Descriptor_Unit_Mi_8T_non_arme_DDR', '$/GFX/Unit/Descriptor_Unit_M2A2_Bradley_IFV_US', '$/GFX/Unit/Descriptor_Unit_BMP_1_SP1_DDR', '$/GFX/Unit/Descriptor_Unit_OT_62_TOPAS_POL', '$/GFX/Unit/Descriptor_Unit_VLRA_trans_FR', '$/GFX/Unit/Descriptor_Unit_M201_FR', '$/GFX/Unit/Descriptor_Unit_M113_Dragon_US', '$/GFX/Unit/Descriptor_Unit_Ural_4320_trans_SOV', '$/GFX/Unit/Descriptor_Unit_M113A3_US', '$/GFX/Unit/Descriptor_Unit_BTR_D_reco_SOV', '$/GFX/Unit/Descriptor_Unit_Super_Puma_FR', '$/GFX/Unit/Descriptor_Unit_AMX_10_P_MILAN_FR', '$/GFX/Unit/Descriptor_Unit_BTR_50_DDR', '$/GFX/Unit/Descriptor_Unit_UAZ_469_Reco_DDR', '$/GFX/Unit/Descriptor_Unit_BTR_70_Rys_SOV', '$/GFX/Unit/Descriptor_Unit_GTMU_1D_ZU_SOV', '$/GFX/Unit/Descriptor_Unit_M151_MUTT_trans_US', '$/GFX/Unit/Descriptor_Unit_UH1H_Huey_US', '$/GFX/Unit/Descriptor_Unit_Saxon_UK', '$/GFX/Unit/Descriptor_Unit_BTR_70D_SOV', '$/GFX/Unit/Descriptor_Unit_LUAZ_967M_SOV', '$/GFX/Unit/Descriptor_Unit_Unimog_trans_RFA', '$/GFX/Unit/Descriptor_Unit_Rover_101FC_UK', '$/GFX/Unit/Descriptor_Unit_M113_ACAV_NG_US', '$/GFX/Unit/Descriptor_Unit_AMX_10_P_FR', '$/GFX/Unit/Descriptor_Unit_M35_trans_US', '$/GFX/Unit/Descriptor_Unit_BMD_1_SOV', '$/GFX/Unit/Descriptor_Unit_T813_trans_DDR', '$/GFX/Unit/Descriptor_Unit_Bedford_MJ_4t_trans_UK', '$/GFX/Unit/Descriptor_Unit_TPZ_Fuchs_MILAN_RFA', '$/GFX/Unit/Descriptor_Unit_FV103_Spartan_BEL', '$/GFX/Unit/Descriptor_Unit_Mi_8TV_SOV', '$/GFX/Unit/Descriptor_Unit_CH47_Chinook_UK', '$/GFX/Unit/Descriptor_Unit_OT_65_DDR', '$/GFX/Unit/Descriptor_Unit_M201_MG_FR', '$/GFX/Unit/Descriptor_Unit_MCV_80_Warrior_MILAN_UK', '$/GFX/Unit/Descriptor_Unit_SPW_152K_DDR', '$/GFX/Unit/Descriptor_Unit_VLTT_P4_FR', '$/GFX/Unit/Descriptor_Unit_BMP_1_SP2_DDR', '$/GFX/Unit/Descriptor_Unit_BMP_1P_reco_SOV', '$/GFX/Unit/Descriptor_Unit_TRM_2000_FR', '$/GFX/Unit/Descriptor_Unit_KrAZ_255B_SOV', '$/GFX/Unit/Descriptor_Unit_M998_Humvee_Delta_US']

    ast = jast.Program(loads(parseAST(r"E:\codeSpace\codeSet\ndf\warnoMod\GameData\Generated\Gameplay\Decks\DivisionRules.ndf")))

    rules: jast.MapRef = ast.statements[0].statement.members[0].expression

    def _(members: list[jast.Member], *, name: str) -> jast.AST:
        key = None
        for m in members:
            if m.identifier.name == name:
                key = m.expression
                break
        return key

    for p in rules.pairs:
        if isinstance(p.second, jast.ObjectIns):
            UnitTransportList: jast.Vector = p.second.members[0].expression
            for e in UnitTransportList.expressions:
                if isinstance(e, jast.ObjectIns) and (n := _(e.members, name='UnitDescriptor').value):
                    relaDict[n] = {
                        'AvailableWithoutTransport': v if (v := _(e.members, name='AvailableWithoutTransport')) else jast.Boolean(value=False),
                        'AvailableTransportList': _(e.members, name='AvailableTransportList')
                    }

    return jast.Vector(expressions=[
        jast.ObjectIns(identifier=jast.Identifier(name='TDeckUniteRule'), members=[
            jast.Member(identifier=jast.Identifier(name='UnitDescriptor'), expression=jast.Path(value=k)),
            jast.Member(identifier=jast.Identifier(name='AvailableWithoutTransport'), expression=v.get('AvailableWithoutTransport', jast.Boolean(value=False))),
            jast.Member(identifier=jast.Identifier(name='NumberOfUnitInPack'), expression=jast.Integer(value=999)),
            jast.Member(identifier=jast.Identifier(name='NumberOfUnitInPackXPMultiplier'), expression=jast.Vector(expressions=[jast.Integer(value=1)] * 4))
        ] + ([jast.Member(identifier=jast.Identifier(name='AvailableTransportList'), expression=v['AvailableTransportList'])] if v.get('AvailableTransportList', None) else []))

        for k, v in relaDict.items()
    ]).code()


if __name__ == '__main__':
    from ndfPyAPI import parseAST

    with open('temp.txt', 'w', encoding='utf-8') as f:
        f.write(buildDivisionRules())
