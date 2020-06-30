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


// Returns a string displaying the given number of bytes in its most readable unit.
std::string Utilities::get_size_string(double bytes)
{
	static const std::string unit_strings[]{"B", "KB", "MB", "GB", "TB", "PB"};
	int unit{0};

	// While the number is greater than or equal to 1024.
	while(bytes >= 1024)
	{
		// Divide by 1024 and increment the unit.
		bytes /= 1024;
		++unit;

		// If the unit has exceeded petabytes, return an error string.
		if(unit > 5) return "?";
	}

	// Calculate the number of fractional digits to display.
	std::ostringstream stream;
	stream<<std::fixed<<std::setprecision(0)<<bytes;
	std::string whole_part{stream.str()};
	int fractional_digits{3-std::min(static_cast<int>(whole_part.size()), 3)};

	// Generate the string.
	stream.str(std::string{});
	stream<<std::fixed<<std::setprecision(fractional_digits)
		<<bytes<<" "<<unit_strings[unit];
	return stream.str();
}