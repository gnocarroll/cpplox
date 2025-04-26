#pragma once

#include "Expr.hpp"

#include <initializer_list>
#include <string>

class AstPrinter : ExprVisitor<std::string> {
public:
    std::string print(Expr& expr) {
        return expr.accept(*this);
    }
    
    template <typename... Types>
    std::string parenthesize(
        std::string_view s,
        Types&&... exprs
    ) {
        std::string ret;
        ret.reserve(64);

        ret += "(";
        ret += s;

        for (const auto& expr : exprs) {
            ret += " ";
            ret += expr->accept(*this);
        }

        ret += ")";

        return ret;
    }

    std::string visitBinaryExpr(Binary& expr) {
        return parenthesize(expr.bOperator.lexeme,
            expr.left, expr.right);
    }

    std::string visitGroupingExpr(Grouping& expr) {
        return parenthesize("group", *expr.expression);
    }

    std::string visitLiteralExpr(Literal& expr) {
        return expr.value.toString();
    }

    std::string visitUnaryExpr(Unary& expr) {
        return parenthesize(expr.uOperator.lexeme, expr.right);
    }
};