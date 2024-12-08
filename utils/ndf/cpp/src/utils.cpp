// Copyright (c) 2024. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file utils.cpp
 * @author edocsitahw
 * @version 1.1
 * @date 2024/11/25 下午3:57
 * @brief
 * */
#include "utils.h"

std::string wstr2str(const std::wstring& wstr, int codepage) {
#if __GUNC__ || __clang__
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(wstr);
#elif _MSC_VER
    int size = WideCharToMultiByte(codepage, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    char* mbstr = new char[size];
    WideCharToMultiByte(codepage, 0, wstr.c_str(), -1, mbstr, size, NULL, NULL);
    std::string str(mbstr);
    delete[] mbstr;
    return str;
#endif
}
