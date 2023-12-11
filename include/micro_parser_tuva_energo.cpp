#define _CRT_SECURE_NO_WARNINGS

#include "micro_parser_tuva_energo.h"

using namespace json_energy;

std::string HTML_reader::out_str(const std::string& adres, const int line_n) {
    std::string readBuffer_UTF8; // Достаточно стека.
    readBuffer_UTF8.reserve(82000);
    CURL* curl;
    CURLcode res_inter;
    curl = curl_easy_init();
    
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, adres.data());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer_UTF8);//
        res_inter = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    else {
        return "";
    }
    if (res_inter != 0) {
        return "";
    }
    
    for (size_t i = 0; i < (size_t)line_n - 1; ++i) {
        readBuffer_UTF8.erase(0, readBuffer_UTF8.find('\n') + 1);
    }
    auto result = readBuffer_UTF8.substr(0, readBuffer_UTF8.find('\n'));
    return result;
}

Date_Parser::Date_Parser(std::string& str) {
    if (!working_line.empty()) {
        std::cout << "<<Castom Error >> The string with power outage dates is empty.";
    }
    else {
        working_line.swap(str);
        set_local_data();
        parser();
    }
}

void Date_Parser::give_str(const std::string& str) {
    working_line = str;
}

void Date_Parser::parser() { //
    size_t count = 0;
    while (count < 20) {
        int pos = working_line.rfind("pof.php?dt=");
        if (pos == -1) {
            break;
        }
        pos += 11;
        auto check_date = short_pars(working_line.substr(pos, 17));
        if (check_date.second == "") {
            pos -= 11;
            working_line.erase(pos, working_line.length() - pos - 1);
            continue;
        }
        if (check_date.first.year == date.year) { // Проверка дат. Нужны только даты текущего дня и будущие. Не больше 7ми дней.
            if (check_date.first.mon == date.mon) {
                if (check_date.first.day >= date.day) {
                    count++;
                    base_dates.push_back(check_date);
                }
            }
            else if (check_date.first.mon > date.mon) {
                count++;
                base_dates.push_back(check_date);
            }
        }
        else if (check_date.first.year > date.year) {
            count++;
            base_dates.push_back(check_date);
        }
        pos -= 11;
        working_line.erase(pos, working_line.length() - pos - 1);
    }
}

std::vector<std::pair<work_date, std::string>> Date_Parser::get_base() {
    return base_dates;
}

std::pair<work_date, std::string> Date_Parser::short_pars(const std::string& str) {
    std::string date_str = str.substr(str.find_first_of('>') + 1, str.length() - 1);
    work_date date;
    try {
        date.day = stoi(date_str.substr(0, 2));
        date.mon = stoi(date_str.substr(3, 2));
        date.year = stoi(date_str.substr(6, 4));
    }
    catch (std::exception& e) {
        std::cout << "<<Exception Error : >>" << e.what() << std::endl;
        return {};
    }
    auto tmp_wstr = str.substr(0, str.find_first_of('"'));
    return { date, {tmp_wstr.begin(), tmp_wstr.end() } };
}

void Date_Parser::set_local_data() {
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    date.day = timeinfo->tm_mday;
    date.mon = timeinfo->tm_mon + 1;
    date.year = timeinfo->tm_year - 100 + 2000;
}

Search_off::Search_off(const std::string& str) {
    using namespace std::literals;

    converter::UTF8_to_unicode conv;
    conv.conv_to(str);
    auto wstr = conv.get_wstr();

    if (wstr.find(L"р-н Сут-Хольский, с Суг-Аксы") != std::string::npos) result.insert(L"р-н Сут-Хольский, с Суг-Аксы");
    if (wstr.find(L"р-н Чаа-Хольский, с Чаа-Холь, м Казанак") != std::string::npos) result.insert(L"р-н Чаа-Хольский, с Чаа-Холь, м Казанак");
    if (wstr.find(L"р-н Дзун-Хемчикский, г Чадан") != std::string::npos) result.insert(L"р-н Дзун-Хемчикский, г Чадан");
    if (wstr.find(L"Бажин-Алаак") != std::string::npos) result.insert(L"Бажин-Алаак");
    if (wstr.find(L"Арыг-Узю") != std::string::npos) result.insert(L"Арыг-Узю");
    if (wstr.find(L"Шагонар") != std::string::npos) result.insert(L"Шагонар");
}

bool Search_off::Get_check() {
    return !result.empty();
}

std::set<std::wstring> Search_off::Get_result() {
    return result;
}

nlohmann::json json_energy::build() {
    json_energy::HTML_reader h;
    std::string adress = "http://www.tuvaenergo.ru/clients/offlist_p/index.php";
    auto str_dates = h.out_str(adress, 1625);
    Date_Parser d(str_dates);

    std::map<std::string, std::set<std::wstring>> date_name_off_ener;
    adress = "http://www.tuvaenergo.ru/clients/offlist_p/pof.php?dt=";
    std::vector<std::future<std::string>> future_bufer;
    std::vector<std::string> date;
    for (const auto& pair : d.get_base()) {
        std::string tmp_adress = adress + pair.second;
        future_bufer.push_back(std::async(&HTML_reader::out_str, &h, tmp_adress, 1626));
        date.push_back(to_str(pair.first));
    }
    for (int i = 0; i < future_bufer.size(); ++i) {
        std::string str_to_searcer = future_bufer[i].get();
        if (str_to_searcer.empty()) {
            std::cout << "<<Castom Error>> : String HTML is empty.";
            continue;
        }
        Search_off oblect_off(str_to_searcer);
        if (oblect_off.Get_check()) {
            date_name_off_ener[date[i]] = oblect_off.Get_result();
        }
    }
    nlohmann::json finish_json;
    finish_json["tuva_energo"] = date_name_off_ener;
    return finish_json;
}

std::string json_energy::to_str(const work_date& date) {
    return std::string(std::to_string(date.day) + '.' + std::to_string(date.mon) + '.' + std::to_string(date.year));
}
