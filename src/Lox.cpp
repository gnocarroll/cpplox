#include "Lox.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
	
// refers to interpreter error, not startup problem
bool Lox::hadError = false;

// run Lox interpretere
// ret is exit code

int Lox::loxMain(const std::vector<std::string_view>& args) {
	
	if (args.size() > 2) {
		std::cerr << "Usage: cpplox [script]\n";
		return 64;
	}
	else if (args.size() == 2) {
		return runFile(args[1]);
	}

	return runPrompt();
}

int Lox::runFile(std::string_view path) {
	assert(path[path.size()] == '\0' && "path not null-terminated");

	// will read entire file into memory buffer

	std::ifstream ifs(path.data(), std::ios::binary | std::ios::ate);

	std::streamsize size = ifs.tellg();

	ifs.seekg(0, std::ios::beg);

	std::string source(size, 0);

	if (!ifs.read(source.data(), size)) {
		return -1; // reading failed
	}

	// run code in file

	return run(source);
}

int Lox::runPrompt() {
	std::string line;

	// repeatedly get line if possible, then run it

	while (true) {
		std::cout << "> ";

		if (!std::getline(std::cin, line)) break;

		run(line);

		hadError = false;
	}

	return 0;
}

int Lox::run(std::string_view source) {

	if (hadError) return 65;

	return 0;
}

void Lox::error(int line, std::string_view message) {
	report(line, "", message);
}

void Lox::report(int line, std::string_view where,
	std::string_view message) {

	std::cerr << "[line " << line << "] Error" << where << ": " <<
		message << '\n';

	hadError = true;
}
