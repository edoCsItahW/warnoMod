# 工具集文件结构与说明

* utils: 工具集文件夹
    * ndf: ndf语言相关工具集
        * cpp: c++语言相关工具集
            * pyAPI: 基于`pybind11`的python接口生成文件夹
                * build.cpp: 编译脚本
                * CMakeLists.txt: pybind11的cmake编译脚本
            * src: ndf词法和语法分析器源码文件夹
                * lexer: ndf词法分析器源码文件夹
                    * token.h | token.cpp | token.hpp: 词法分析器文件
                    * lexer.h | lexer.cpp: 词法分析器文件
                    * tokenProc.h | tokenProc.cpp: 词法分析器预处理文件
                * parser: ndf语法分析器源码文件夹
                * expection.h: 自定义异常类头文件
                * utils.h | utils.cpp | utils.hpp: 工具类文件
            * CMakeLists.txt: 分析器的cmake编译脚本
            * Doxyfile: doxygen配置文件
            * main.cpp: 示例程序
        * py: python语言相关工具集
            * _types.py: AST节点对应JSON字段的类型定义文件
            * genGUID.py: 生成随机GUID的工具脚本
            * jsonAST.py: 解析JSON格式的AST并生成AST对象
            * main.py: 示例程序
            * ndfPyAPI.pyd: pybind11生成的python接口文件
            * ndfPyAPI.pyi: pybind11生成的python接口类型定义文件
