/*
	Copyright 2020 Myles Trevino
	Licensed under the Apache License, Version 2.0
	http://www.apache.org/licenses/LICENSE-2.0
*/


#include "Utilities.hpp"

#include <codecvt>
#include <algorithm>
#include <sstream>
#include <iomanip>


namespace
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
}


// Converts the given string to uppercase.
std::string Utilities::to_uppercase(std::string string)
{
	std::transform(string.begin(), string.end(), string.begin(), toupper);
	return string;
}


// Converts the given UTF-16 string to a UTF-8 one.
std::string Utilities::utf16_to_utf8(const std::wstring& string)
{ return convert.to_bytes(string); }


// Converts the given UTF-8 string to a UTF-16 one.
std::wstring Utilities::utf8_to_utf16(const std::string& string)
{ return convert.from_bytes(string); }