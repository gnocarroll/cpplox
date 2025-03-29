#pragma once

#include <string_view>
#include <vector>

#include "Token.hpp"

class Scanner {
	const std::string_view source;
	std::vector<Token> tokens;

	bool hasScanned = false;

	size_t start = 0;
	size_t current = 0;
	size_t line = 1;
	size_t sourceNChars;

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

	void scanTokens(); // reads all
	void scanToken(); // reads one

	// append to vector

	void addToken(TokenType type);
	void addToken(TokenType type, void* literal);

	// string literal

	void string();

public:
	Scanner(const std::string_view source) :
		source(source), sourceNChars(source.size()) {}
};