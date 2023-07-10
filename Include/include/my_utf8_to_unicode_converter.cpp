#include "my_utf8_to_unicode_converter.h"

void converter::UTF8_to_unicode::conv_to(const std::string& str) {
    //if (str.empty()) return L"";

    std::wstring wstr;
    for (auto it = str.begin(); it != str.end(); ++it) {
        if (*it == -48) {
            wstr.push_back((*++it + 128) + std::pow(2, 10));
        }
        else if (*it == -47) {
            wstr.push_back((*++it + 128) + std::pow(2, 10) + 64);
        }
        else {
            wstr.push_back(*it);
        }
    }
    convert_str = wstr;
}

std::wstring_view converter::UTF8_to_unicode::get_raw() {
    return convert_str;
}

std::wstring converter::UTF8_to_unicode::get_wstr() {
    return convert_str;
}