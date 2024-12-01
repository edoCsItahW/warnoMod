// Copyright (c) 2024. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file tokenProc.h
 * @author edocsitahw
 * @version 1.1
 * @date 2024/11/25 下午3:55
 * @brief
 * */
#ifndef NDF_TOKENPROC_H
#define NDF_TOKENPROC_H
#pragma once

#include "lexer.h"

class TokenProcessor {
private:
    TokenPtrs _tokens;
    size_t _idx = 0;
    TokenPtr handleNumber();
    TokenPtr handleLiteral();
    TokenPtr handleOperator();

public:
    explicit TokenProcessor(TokenPtrs tokens);
    TokenPtr operator*();
    static TokenPtrs process(TokenPtrs tokens, bool comment = false);
};

#endif  // NDF_TOKENPROC_H
