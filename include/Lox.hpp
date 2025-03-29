#pragma once

#include <string_view>
#include <vector>

class Lox {
public:
	static bool hadError;

	// porting from Java so will have this as "main func"
	static int loxMain(const std::vector<std::string_view>& args);

	static void error(int line, std::string_view message);

private:

	// run interpreter on file
	static int runFile(const std::string_view path);

	// run interactively
	static int runPrompt();

	// takes buffer full of bytes and will read + run
	static int run(std::string_view source);

	static void report(int line, std::string_view where,
		std::string_view message);
};
