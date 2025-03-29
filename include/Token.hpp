#pragma once

#include <ostream>
#include <string>
#include <string_view>

#include "LoxObject.hpp"
#include "TokenType.hpp"
#include "Util.hpp"

class Token {
public:
	const TokenType type;
	const std::string_view lexeme;
	LoxObject literal;
	const int line;

	Token(const TokenType type, const std::string_view lexeme,
		LoxObject& literal, const int line) :
		type(type), lexeme(lexeme), literal(std::move(literal)), line(line) {}

	Token(Token& other) = delete;
	Token(Token&& other) = default;

	std::string toString();

	friend std::ostream& operator<<(std::ostream& os, const Token& token) {
		return Util::outputSVs(os, {
			token.type.toStringView(),
			token.lexeme
			});
	}
};