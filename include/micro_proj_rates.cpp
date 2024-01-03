#include "micro_proj_rates.h"

nlohmann::json Exchange_rates::json_exchange_rates(const std::vector<std::string>& id_currency) {
    std::string readBuffer_UTF8;
    readBuffer_UTF8.reserve(82000);
    CURL* curl;
    CURLcode res_inter;
    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://www.cbr-xml-daily.ru/daily_json.js");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer_UTF8);
        res_inter = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    buffer_check check;
    nlohmann::json json_full;
    if (check.check(readBuffer_UTF8)) {
        try {
            json_full = nlohmann::json::parse(readBuffer_UTF8);
        }
        catch (std::exception& e) {
            std::cout << "micro_proj_rates.cpp json-parse-error\n" << e.what();
        }
    }
    else {
        std::cout << "<<Castom error>> - micro_proj_rates.cpp The Buffer does not contain json or there are errors in it.\n";
        return nlohmann::json::parse("{}");
    }
    nlohmann::json result;
    auto it = json_full.find("Valute");
    if (it != json_full.end()) {
        auto arr_exchange_rates = it.value();
        for (const auto& сurrency : id_currency) {
            auto& arr_jvaluta = arr_exchange_rates.find(сurrency).value();
            result.push_back(arr_jvaluta);
        }

	//??????
        for (auto& valuta : result) {
            auto& str_jname = valuta.find("Name").value();
	        str_jname = "\xD0\x94";
        }
	
    }
    return result;
}
