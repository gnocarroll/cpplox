#include "Lox.hpp"

#include <fstream>
#include <iostream>
#include <string>

#include "Util.hpp"
#include "Scanner.hpp"

// refers to interpreter error, not startup problem
bool Lox::hadError = false;

// run Lox interpretere
// ret is exit code

int Lox::loxMain(int argc, char** argv) {
	if (argc > 2) {
		std::cerr << "Usage: cpplox [script]\n";
		return 64;
	}
	else if (argc == 2) {
		return runFile(argv[1]);
	}

	return runPrompt();
}

int Lox::runFile(NTStringView path) {
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

int Lox::run(const std::string_view source) {
	Scanner scanner(source);

	for (const Token& token : scanner) {
		std::cout << token << '\n';
	}

	if (hadError) return 65;

	return 0;
}

void Lox::error(size_t line, std::string_view message) {
	report(line, "", message);
}

void Lox::report(size_t line, std::string_view where,
	std::string_view message) {

	std::cerr << "[line " << line << "] Error" << where << ": " <<
		message << '\n';

	hadError = true;
}
