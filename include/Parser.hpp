#pragma once

#include <initializer_list>
#include <memory>
#include <vector>

#include "Expr.hpp"
#include "NTStringView.hpp"
#include "TokenType.hpp"

class Parser;

using ParseExpr = std::unique_ptr<Expr>(Parser::*)();

class Parser {
	size_t current = 0;

	// receive token vector from scanner
	// parser will not add to / remove from it
	const std::vector<Token>& tokens;
	size_t nTokens;

	Parser(const std::vector<Token>& tokens) :
		tokens(tokens), nTokens(tokens.size()) {}
	
	// move but don't allow copy
	Parser(Parser& other) = delete;
	Parser(Parser&& other) = default;

	// check if upcoming tokens match ones provided in initializer list
	bool match(std::initializer_list<TokenType> types);

	// does current match expected type
	bool check(TokenType type);

	// consume current and return (or, advance and return previous)
	const Token& advance();

	// basic utilities
	bool isAtEnd();
	const Token& peek();
	const Token& previous();

	// parsing different kinds of expressions with these functions


	// expressions using binary operators
	std::unique_ptr<Expr> expression();
	std::unique_ptr<Expr> equality();
	std::unique_ptr<Expr> comparison();
	std::unique_ptr<Expr> term();
	std::unique_ptr<Expr> factor();

	// unary operators
	std::unique_ptr<Expr> unary();

	// primary (e.g. string literal or parenthesized expr)
	std::unique_ptr<Expr> primary();

	template<ParseExpr subExpr, typename... Types>
	std::unique_ptr<Expr> parseBinary(Types... types) {
		auto expr = subExpr();

		while (match({ types... })) {
			auto& bOperator = previous();
			auto right = subExpr();

			expr = std::make_unique<Binary>(
				std::move(expr),
				bOperator,
				std::move(right)
			);
		}

		return expr;
	}

	const Token& consume(TokenType type, NTStringView msg);
};