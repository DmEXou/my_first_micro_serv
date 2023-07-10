#include <crow.h>

#include "Include/micro_parser_tuva_energo.h"
#include "Include/micro_proj_rates.h"

int main() {
	nlohmann::json final;
	final["energy"] = json_energy::build();
	Exchange_rates er;
	final["rates"] = er.json_exchange_rates({"USD", "EUR"});
	//std::cout << final.dump(4);
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([&final]() {
        return final.dump(4);
        });

    app.port(80).multithreaded().run();
	return 0;
}
