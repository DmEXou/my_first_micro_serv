#pragma once
#include <algorithm>
#include <iostream>
#include <string>

class buffer_check {
public:
	bool check(const std::string& buffer) {
		auto con_f = std::count_if(buffer.begin(), buffer.end(), [](const char& ch) {
			return (ch == '[' || ch == '{');
			});
		auto con_l = std::count_if(buffer.begin(), buffer.end(), [](const char& ch) {
			return (ch == ']' || ch == '}');
			});
		return (con_f == con_l);
	}
};