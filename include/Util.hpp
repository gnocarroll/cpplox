#pragma once

#include <initializer_list>
#include <optional>
#include <ostream>
#include <string>
#include <string_view>

namespace Util {

	// convert initializer list of string_view objects to string by placing
	// sep (default to space) between them
	std::string SVsToString(
		std::initializer_list<const std::string_view> list,
		const std::string_view sep = " "
	);

	std::ostream& outputSVs(
		std::ostream& os,
		std::initializer_list<const std::string_view> list,
		const std::string_view sep = " "
	);

	// read entire file into memory
	std::optional<std::string> slurp(const std::string_view path);

}
