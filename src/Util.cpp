#include "Util.hpp"

#include <cstdlib>
#include <fstream>

namespace Util {

	// convert initializer list of string_view objects to string by placing
	// sep (default to space) between them
	std::string SVsToString(
		std::initializer_list<const std::string_view> list,
		const std::string_view sep
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

	std::ostream& outputSVs(
		std::ostream& os,
		std::initializer_list<const std::string_view> list,
		const std::string_view sep = " "
	) {
		size_t listSize = list.size();
		size_t iterNo = 0;

		for (const auto& sv : list) {
			os << sv;

			if (iterNo < listSize - 1) os << sep;

			iterNo++;
		}

		return os;
	}

	std::optional<std::string> slurp(const std::string_view path) {
		// will read entire file into memory buffer

		const char* cPath = nullptr;
		std::optional<std::string> strPath;

		// can only use string view directly for filepath if it is null-terminated

		if (path[path.size()] == '\0') cPath = path.data();
		else {
			strPath = std::string(path);
			cPath = strPath->data();
		}

		std::ifstream ifs(cPath, std::ios::binary | std::ios::ate);

		if (!ifs) return {};

		std::streamsize size = ifs.tellg();

		ifs.seekg(0, std::ios::beg);

		std::string source(size, 0);

		if (!ifs.read(source.data(), size)) return {};

		return source;
	}

}
