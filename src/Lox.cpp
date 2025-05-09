#include "Lox.hpp"

#include <fstream>
#include <iostream>
#include <string>

#include "AstPrinter.hpp"
#include "Parser.hpp"
#include "Scanner.hpp"
#include "Util.hpp"


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

	if (hadError) return 65;

	Parser parser(scanner.data());
	ExprPtr exprPtr = parser.parse();

	if (hadError || !exprPtr) return 65;

	std::cout << AstPrinter().print(**exprPtr)
		<< '\n';

	return 0;
}

void Lox::error(size_t line, std::string_view message) {
	report(line, "", message);
}

void Lox::error(const Token& token, std::string_view message) {
	if (token.type == TokenType::TOK_EOF) {
		report(token.line, " at end", message);
		return;
	}

	std::string where;
	constexpr static char beforeLexeme[] = " at '";
	constexpr static char afterLexeme[] = "'";

#define STRLEN_ARR(arr) (sizeof(arr) - 1)

	where.reserve(STRLEN_ARR(beforeLexeme) + token.lexeme.size() +
		STRLEN_ARR(afterLexeme));

#undef STRLEN_ARR

	// apparently can't do std::string + std::string_view ???
	const char* lexeme = token.lexeme.data();

	where = where + beforeLexeme + lexeme + afterLexeme;

	report(token.line, where, message);
}

void Lox::report(size_t line, std::string_view where,
	std::string_view message) {

	std::cerr << "[line " << line << "] Error" << where << ": " <<
		message << '\n';

	hadError = true;
}
