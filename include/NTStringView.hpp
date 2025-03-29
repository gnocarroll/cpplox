#pragma once

#include <string>
#include <string_view>

// NT == Null-Terminated

class NTStringView : private std::string_view {
	
#define USING(m) using std::string_view::m;
	
	USING(basic_string_view)

public:
	USING(begin)
	USING(cbegin)

	USING(end)
	USING(cend)

	USING(front)
	USING(back)

	USING(data)

	USING(size)
	USING(length)

	USING(empty)

#undef USING

	NTStringView(const char* s) :
		std::string_view(s) {};
	NTStringView(const std::string& s) :
		std::string_view(s) {};

	operator std::string_view() {
		return std::string_view(data(), size());
	}
};