#include <chrono>
#include <ctime>

#include <crow.h>

#include "Include/micro_parser_tuva_energo.h"
#include "Include/micro_proj_rates.h"
#include "include/micro_bc_game.h"

void update_json(nlohmann::json& final) {
	const auto now = std::chrono::system_clock::now();
	const std::time_t t_c = std::chrono::system_clock::to_time_t(now);
	std::string upd_time = std::ctime(&t_c);
	upd_time.pop_back();
	final["Time update"] = upd_time;
	final["energy"] = json_energy::build();
	Exchange_rates er;
	final["rates"] = er.json_exchange_rates({"USD", "EUR", "CNY"});
	Bc_rates bc;
	//auto a = bc.json_bc_rates();
	final["bc_game"] = bc.json_bc_rates();
}

int main() {
	nlohmann::json final;
	update_json(final);
    crow::SimpleApp app;
	std::chrono::time_point<std::chrono::steady_clock> old_t_point = std::chrono::steady_clock::now();

    CROW_ROUTE(app, "/")([&final, &old_t_point]() {
		std::chrono::time_point<std::chrono::steady_clock> new_t_point = std::chrono::steady_clock::now();

		if (std::chrono::duration_cast<std::chrono::hours>(new_t_point - old_t_point) >= std::chrono::hours(3)) {
			update_json(final);
			old_t_point = std::chrono::steady_clock::now();
		}
		//CROW_LOG_INFO << 
        return final.dump(4);
        });

    app.port(80).multithreaded().run();
	return 0;
}
