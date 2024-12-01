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
from abc import ABC, abstractmethod
from typing import List
from types import GenericAlias


class Base(ABC):
    base: int
    baseAttr: int = 1

    @abstractmethod
    def method(self):
        print(self.__annotations__, list(filter(lambda _: not _.startswith('__'), dir(self))),)


class Test(Base):
    attr: int
    myAttr: int = 2
    listAttr: list
    listAnnoAttr: list['Test2']
    listAnnoAttr2: List['yyy']
    listAnnoAttr3: List[int]
    listAnnoAttr4: 'Test2'

    def method(self):
        super().method()


class Test2(Base): ...


if __name__ == '__main__':
    print(Test2.__mro__)
