#include "Parser.hpp"

// check if upcoming tokens match ones provided in initializer list
bool Parser::match(std::initializer_list<TokenType> types) {
	for (const auto& type : types) {
		if (!check(type)) continue;

		advance();
		return true;
	}

	return false;
}

// does current match expected type
bool Parser::check(TokenType type) {
	if (isAtEnd()) return false;

	return peek().type == type;
}

// consume current and return (or, advance and return previous)
const Token& Parser::advance() {
	if (!isAtEnd()) current++;

	return previous();
}

// basic utilities
bool Parser::isAtEnd() {
	return current >= nTokens || peek().type == TokenType::TOK_EOF;
}

const Token& Parser::peek() {
	return tokens[current];
}

const Token& Parser::previous() {
	return tokens[current - 1];
}

// parsing different kinds of expressions with these functions
std::unique_ptr<Expr> Parser::expression() {
	return equality();
}

// will template it so that you just have to provide the operators you need

std::unique_ptr<Expr> Parser::equality() {
	return parseBinary<comparison>(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL);
}

std::unique_ptr<Expr> Parser::comparison() {
	return parseBinary<term>(
		TokenType::GREATER,
		TokenType::GREATER_EQUAL,
		TokenType::LESS,
		TokenType::LESS_EQUAL
	);
}

std::unique_ptr<Expr> Parser::term() {
	return parseBinary<factor>(TokenType::PLUS, TokenType::MINUS);
}

std::unique_ptr<Expr> Parser::factor() {
	return parseBinary<unary>(TokenType::STAR, TokenType::SLASH);
}

std::unique_ptr<Expr> Parser::unary() {
	if (match({ TokenType::BANG, TokenType::MINUS })) {
		auto& uOperator = previous();
		auto right = unary();

		return std::make_unique<Unary>(uOperator, std::move(right));
	}
}

std::unique_ptr<Expr> Parser::primary() {
	// literals

	if (match({ TokenType::FALSE })) {
		return std::make_unique<Literal>(LoxObject(false));
	}
	if (match({ TokenType::TRUE })) {
		return std::make_unique<Literal>(LoxObject(true));
	}
	if (match({ TokenType::NIL })) {
		return std::make_unique<Literal>(LoxObject::nil());
	}
	if (match({ TokenType::NUMBER, TokenType::STRING })) {
		return std::make_unique<Literal>(previous().literal);
	}

	// parenthesized expr

	if (match({ TokenType::LEFT_PAREN })) {
		auto expr = expression();

		consume(TokenType::RIGHT_PAREN, "Expected ')' after expression.");

		return std::make_unique<Grouping>(std::move(expr));
	}

	// ...
}