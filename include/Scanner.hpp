#pragma once

#include <atomic>
#include <optional>
#include <string_view>
#include <vector>

#include "LoxObject.hpp"
#include "NTStringView.hpp"
#include "Token.hpp"

class Scanner {
	const std::string_view source;
	std::vector<Token> tokens;

	bool hasScanned = false;

	size_t start = 0;
	size_t current = 0;
	size_t line = 1;
	size_t sourceNChars;

	// utilities

	static bool isDigit(char c);
	static bool isLower(char c);
	static bool isUpper(char c);
	static bool isLetter(char c);
	static bool isAlpha(char c);

	static std::optional<double> parseNumber(NTStringView sv);

	bool isAtEnd() const {
		return current >= sourceNChars;
	}

	char advance() {
		if (isAtEnd()) return '\0';

		return source[current++];
	}

	bool match(char expected) {
		if (isAtEnd() || source[current] != expected) return false;

		// is match

		current++;
		return true;
	}

	char peek() {
		if (isAtEnd()) return '\0';

		return source[current];
	}

	char peekNext() {
		if (current + 1 >= sourceNChars) return '\0';

		return source[current + 1];
	}

	// substring [start, current)

	std::string_view currentSubstr() const;

	void scanTokens(); // reads all
	void scanToken(); // reads one

	// append to vector

	void addToken(TokenType type);
	void addToken(TokenType type, LoxObject& literal);

	// handling variable length things in the code e.g. string literal

	void string();
	void number();
	void identifier();

public:
	Scanner(const std::string_view source) :
		source(source), sourceNChars(source.size()) {}

	Token* begin() {
		if (!hasScanned) scanTokens();

		return tokens.data();
	}
	Token* end() {
		return tokens.data() + tokens.size();
	}
};