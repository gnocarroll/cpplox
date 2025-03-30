#pragma once

#include "LoxObject.hpp"
#include "Token.hpp"

template <typename T>
class ExprVisitor;

class Expr {
public:
	virtual ~Expr() = default;

	template <typename T>
	T accept(const ExprVisitor<T>& exprVisitor) {
#define CHILD_CASE(childT) \
		if (dynamic_cast<childT>(this) != nullptr) { \
			return dynamic_cast<childT>(this)->accept(exprVisitor); \
		}

		CHILD_CASE(Binary)
		CHILD_CASE(Grouping)
		CHILD_CASE(Literal)
		CHILD_CASE(Binary)

#undef CHILD_CASE

		return T;
	}
};

#define CHILD_ACCEPT \
	template <typename T> \
	T accept(const ExprVisitor<T>& exprVisitor) { \
		return exprVisitor.visit(*this); \
	}

class Binary : public Expr {
public:
	std::unique_ptr<Expr> left;
	Token bOperator;
	std::unique_ptr<Expr> right;

	Binary() = delete;
	Binary(
		std::unique_ptr<Expr> left,
		Token bOperator,
		std::unique_ptr<Expr> right
	) : left(std::move(left)), bOperator(bOperator), right(std::move(right)) {};

	~Binary() = default;

	CHILD_ACCEPT
};

class Grouping : public Expr {
public:
	std::unique_ptr<Expr> expression;

	Grouping() = delete;
	Grouping(std::unique_ptr<Expr> expression) :
		expression(std::move(expression)) {}

	~Grouping() = default;

	CHILD_ACCEPT
};

class Literal : public Expr {
public:
	LoxObject value;

	Literal() = delete;
	Literal(LoxObject value) : value(std::move(value)) {};

	~Literal() = default;

	CHILD_ACCEPT
};

class Unary : public Expr {
public:
	Token uOperator;
	std::unique_ptr<Expr> right;

	Unary() = delete;
	Unary(Token uOperator, std::unique_ptr<Expr> right) :
		uOperator(uOperator), right(std::move(right)) {};

	CHILD_ACCEPT
};

template <typename T>
class ExprVisitor {
public:
	virtual T visitBinaryExpr(Binary& expr);
	virtual T visitGroupingExpr(Binary& expr);
	virtual T visitLiteralExpr(Binary& expr);
	virtual T visitUnaryExpr(Binary& expr);
};