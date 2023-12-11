#pragma once
#include <string>

#include <nlohmann/json.hpp>
#include <curl/curl.h>

class Bc_rates {
public:
	nlohmann::json json_bc_rates();
private:
	static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
		((std::string*)userp)->append((char*)contents, size * nmemb);
		return size * nmemb;
	}
};