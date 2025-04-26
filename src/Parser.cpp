#include "Parser.hpp"

#include "Lox.hpp"

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
ExprPtr Parser::expression() {
	return equality();
}

// will template it so that you just have to provide the operators you need

ExprPtr Parser::equality() {
	return parseBinary<comparison>(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL);
}

ExprPtr Parser::comparison() {
	return parseBinary<term>(
		TokenType::GREATER,
		TokenType::GREATER_EQUAL,
		TokenType::LESS,
		TokenType::LESS_EQUAL
	);
}

ExprPtr Parser::term() {
	return parseBinary<factor>(TokenType::PLUS, TokenType::MINUS);
}

ExprPtr Parser::factor() {
	return parseBinary<unary>(TokenType::STAR, TokenType::SLASH);
}

ExprPtr Parser::unary() {
	if (match({ TokenType::BANG, TokenType::MINUS })) {
		auto& uOperator = previous();
		auto right = unary();

		return std::make_unique<Unary>(uOperator, std::move(right));
	}
}

ExprPtr Parser::primary() {
	// literals

	if (match({ TokenType::FALSE })) {
		return std::make_unique<Literal>(LoxObject::boolFalse());
	}
	if (match({ TokenType::TRUE })) {
		return std::make_unique<Literal>(LoxObject::boolTrue());
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

		if (!consume(TokenType::RIGHT_PAREN)) {
			Parser::error(peek(), "Expected ')' after expression.");
			return {};
		}

		return std::make_unique<Grouping>(std::move(expr));
	}

	// did not match

	Parser::error(peek(), "Expected primary expr, no expansion matched");
	return {};
}

std::optional<const Token&> Parser::consume(TokenType type) {
	if (check(type)) return advance();

	return {};
}

void Parser::error(const Token & token, NTStringView msg) {
	Lox::error(token, msg.toStringView());
}

void Parser::synchronize() {
	advance();

	while (!isAtEnd()) {
		if (previous().type == TokenType::SEMICOLON) return;

		switch (peek().type.getVal()) {
#define CASE_TT(name) case TokenType::name:

			CASE_TT(CLASS)
			CASE_TT(FUN)
			CASE_TT(VAR)
			CASE_TT(FOR)
			CASE_TT(IF)
			CASE_TT(WHILE)
			CASE_TT(PRINT)
			CASE_TT(RETURN)

#undef CASE_TT
			return; // if one of the above then done

		default:
			break;
		}

		advance();
	}
}

ExprPtr Parser::parse() {
	return expression();
}