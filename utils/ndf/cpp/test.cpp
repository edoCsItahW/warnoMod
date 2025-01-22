/**
 * @file test.cpp
 * @author edocsitahw
 * @version 1.1
 * @date 2024/12/05 下午3:41
 * @brief
 * */

#include <iostream>
#include <codecvt>
#include <locale>
#include <windows.h>

std::string wstr2str(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(wstr);
}

namespace t1 {
    namespace t2 {
        void func() {}
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    std::setlocale(LC_ALL, "");
    t1::t2::func();
    auto text = std::wstring(L"Ne pas éditer, ce fichier est généré par DivisionRulesFileWriter");

    for (wchar_t c : text)
        std::wcout << c << std::endl;

    return 0;
}
