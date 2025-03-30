#pragma once

#include "Expr.hpp"

#include <initializer_list>
#include <string>

class AstPrinter : ExprVisitor<std::string> {
    std::string parenthesize(
        std::string_view s,
        std::initializer_list<Expr*> exprs
    ) {

    }

public:
    std::string visitBinaryExpr(Binary& expr) {
        return parenthesize(expr.bOperator.lexeme,
            { expr.left, expr.right });
    }

    std::string visitGroupingExpr(Grouping& expr) {
        return parenthesize("group", { expr.expression });
    }

    std::string visitLiteralExpr(Literal& expr) {
        return expr.value.toString();
    }

    std::string visitUnaryExpr(Unary& expr) {
        return parenthesize(expr.uOperator.lexeme, { expr.right });
    }
};