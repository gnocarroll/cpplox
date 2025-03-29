#include "Lox.hpp"

#include <fstream>
#include <iostream>
#include <string>

#include "Util.hpp"

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

int Lox::runFile(const std::string_view path) {
    auto source = Util::slurp(path);

    if (!source) return -1;
	
    // run code in file

	return run(*source);
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
