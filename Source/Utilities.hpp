#pragma once

#include <string>


namespace Utilities
{
	std::string to_uppercase(std::string string);
	std::string utf16_to_utf8(const std::wstring& string);
	std::wstring utf8_to_utf16(const std::string& string);

	std::string get_size_string(double bytes);
}