#pragma once

#include <cstdlib>
#include <initializer_list>
#include <string>
#include <string_view>

// convert initializer list of string_view objects to string by placing
// sep (default to space) between them
std::string SVsToString(
	std::initializer_list<const std::string_view> list,
	const std::string_view sep = " "
) {
	std::string ret;
	size_t listSize = list.size();

	std::string::size_type retSize = (listSize - 1) * sep.size();

	for (const auto& sv : list) retSize += sv.size();

	ret.resize(retSize);

	std::string::size_type dstIdx = 0;
	size_t iterNo = 0;

	auto copyInSV = [&](const std::string_view sv) {
			std::memcpy(ret.data() + dstIdx, sv.data(), sv.size());

			dstIdx += sv.size();
		};

	for (const auto& sv : list) {
		copyInSV(sv);

		if (iterNo < listSize - 1) copyInSV(sep);

		iterNo++;
	}

	return ret;
}