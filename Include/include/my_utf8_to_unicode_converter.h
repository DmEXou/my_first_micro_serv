#pragma once
#include <cmath>
#include <string>
#include <string_view>

namespace converter {
class UTF8_to_unicode {
public:
	void conv_to(const std::string& str);

	std::wstring_view get_raw();

	std::wstring get_wstr();

private:
	std::wstring convert_str;
};
}
