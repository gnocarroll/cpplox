#pragma once

#include <initializer_list>
#include <memory>
#include <optional>
#include <vector>

#include "Expr.hpp"
#include "NTStringView.hpp"
#include "TokenType.hpp"

class Parser;

using ExprPtr = std::optional<std::unique_ptr<Expr> >;

using ParseExpr = ExprPtr(Parser::*)();

class Parser {
	size_t current = 0;

	// receive token vector from scanner
	// parser will not add to / remove from it
	const std::vector<Token>& tokens;
	size_t nTokens;

public:
	Parser(const std::vector<Token>& tokens) :
		tokens(tokens), nTokens(tokens.size()) {}
	
	// move but don't allow copy
	Parser(Parser& other) = delete;
	Parser(Parser&& other) = default;

	// begin parsing
	ExprPtr parse();

private:
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
	ExprPtr expression();
	ExprPtr equality();
	ExprPtr comparison();
	ExprPtr term();
	ExprPtr factor();

	// unary operators
	ExprPtr unary();

	// primary (e.g. string literal or parenthesized expr)
	ExprPtr primary();

	template<ParseExpr subExpr, typename... Types>
	ExprPtr parseBinary(Types... types) {
		auto exprPtr = subExpr();

		if (!exprPtr) return {};

		while (match({ types... })) {
			auto& bOperator = previous();
			
			auto rightPtr = subExpr();

			if (!rightPtr) return {};

			exprPtr = std::make_unique<Binary>(
				std::move(*exprPtr),
				bOperator,
				std::move(*rightPtr)
			);
		}

		return exprPtr;
	}

	std::optional<const Token&> consume(TokenType type);
	static void error(const Token& token, NTStringView msg);

	// in the event of any problems...
	void synchronize();
};