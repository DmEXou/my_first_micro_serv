#pragma once
#include <string>
#include <vector>

#include "include/my_utf8_to_unicode_converter.h"
#include "include/json.hpp"
#include <curl/curl.h>

class Exchange_rates {
public:
	nlohmann::json json_exchange_rates(const std::vector<std::string>& id_ñurrency);
private:
	static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
		((std::string*)userp)->append((char*)contents, size * nmemb);
		return size * nmemb;
	}
};