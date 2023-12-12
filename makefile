program: micro_parser_tuva_energo.o micro_proj_rates.o micro_bc_game.o my_utf8_to_unicode_converter.o
	clang++ micro_collector.cpp -o serv_micro micro_parser_tuva_energo.o micro_proj_rates.o micro_bc_game.o my_utf8_to_unicode_converter.o -std=c++20 -lcurl -lpthread

my_utf8_to_unicode_converter.o: include/include/my_utf8_to_unicode_converter.cpp include/include/my_utf8_to_unicode_converter.h
	clang++ -c include/include/my_utf8_to_unicode_converter.cpp

micro_proj_rates.o: include/micro_proj_rates.cpp include/micro_proj_rates.h
	clang++ -c  include/micro_proj_rates.cpp

micro_parser_tuva_energo.o: include/micro_parser_tuva_energo.cpp include/micro_parser_tuva_energo.h
	clang++ -c include/micro_parser_tuva_energo.cpp

micro_bc_game.o: include/micro_bc_game.cpp include/micro_bc_game.h
	clang++ -c include/micro_bc_game.cpp

clean:
	rm micro_parser_tuva_energo.o micro_proj_rates.o my_utf8_to_unicode_converter.o micro_bc_game.o
