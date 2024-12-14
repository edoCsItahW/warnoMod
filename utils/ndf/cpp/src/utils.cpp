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
#include <filesystem>
#include <fstream>

namespace dbg {
    bool First = true;
    bool DEBUG = false;
}  // namespace dbg


namespace os {
    std::string completePath(const std::string& _str) {
        std::filesystem::path path(_str);

        if (!path.is_absolute())  // 不是绝对路径,对于相对路径和文件名都可以转成绝对路径
            path = std::filesystem::absolute(path);

        if (!std::filesystem::exists(path)) throw std::filesystem::filesystem_error("File not found: " + _str, std::make_error_code(std::errc::no_such_file_or_directory));

        return path.string();
    }

    AnyStr readFile(const std::string& _filePath, Type _type, std::string cwd) {
        if (!cwd.empty()) std::filesystem::current_path(cwd);

        const auto filePath = completePath(_filePath);
        switch (_type) {
            using enum Type;
            case UTF8: {
                std::ifstream file(filePath, std::ios::in);
                if (file.is_open()) {
                    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                    file.close();
                    return content;
                } else {
                    file.close();
                    throw std::runtime_error("Failed to open file: " + filePath);
                }
            }
            case GBK: {
                std::wifstream file(filePath, std::ios::in);
                if (file.is_open()) {
                    std::wstring content((std::istreambuf_iterator<wchar_t>(file)), std::istreambuf_iterator<wchar_t>());
                    file.close();
                    return content;
                } else {
                    file.close();
                    throw std::runtime_error("Failed to open file: " + filePath);
                }
            }
            default: throw std::runtime_error("Unsupported encoding type!");
        }
    }
}  // namespace os
