#pragma once

#include <future>
#include <iostream>
#include <map>
#include <set>
#include <vector>

#include <curl/curl.h>
#include "include/json.hpp"
#include "include/my_utf8_to_unicode_converter.h"

namespace json_energy {

    struct work_date {
        int day = 0;
        int mon = 0;
        int year = 0;
    };

    std::string to_str(const work_date& date);

    class HTML_reader {
    public:
        std::string out_str(const std::string& adres, const int line_n);
    private:
        static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
            ((std::string*)userp)->append((char*)contents, size * nmemb);
            return size * nmemb;
        }
    };

    class Date_Parser {
    public:
        Date_Parser(std::string& str);
        void give_str(const std::string& str);
        void parser();
        std::vector<std::pair<work_date, std::string>> get_base();
    private:
        std::pair<work_date, std::string> short_pars(const std::string& str);
        void set_local_data();
    private:
        std::string working_line;
        std::vector<std::pair<work_date, std::string>> base_dates;
        work_date date;
    };

    class Search_off {
    public:

        Search_off(const std::string& str);

        bool Get_check();

        std::set<std::wstring> Get_result();

    private:
        std::set<std::wstring> result;
    };

    nlohmann::json build();
}