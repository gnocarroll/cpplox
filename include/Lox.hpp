#pragma once

#include <string_view>
#include <vector>

#include "NTStringView.hpp"

class Lox {
public:
	static bool hadError;

	// porting from Java so will have this as "main func"
	static int loxMain(int argc, char** argv);

	static void error(size_t line, std::string_view message);

private:

	// run interpreter on file
	static int runFile(NTStringView path);

	// run interactively
	static int runPrompt();

	// takes buffer full of bytes and will read + run
	static int run(const std::string_view source);

	static void report(size_t line, std::string_view where,
		std::string_view message);
};
