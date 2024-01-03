#include "micro_bc_game.h"

nlohmann::json Bc_rates::json_bc_rates() {
    std::string readBuffer_UTF8;
    readBuffer_UTF8.reserve(82000);
    CURL* curl;
    CURLcode res_inter;
    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://www.blockchain.com/ru/ticker");
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
            std::cout << "micro_bc_game.cpp json-parse-error\n" << e.what();
        }
    }
    else {
        std::cout << "<<Castom error>> - micro_bc_game.cpp The Buffer does not contain json or there are errors in it.\n";
        return nlohmann::json::parse("{}");
    }
    auto it = json_full.find("RUB");
    nlohmann::json result;
    if (it != json_full.end()) {
        auto int_it = it.value().find("buy");
        result = int_it.value();
    }
    return result;
}
