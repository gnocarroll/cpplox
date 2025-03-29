#pragma once

#include <string>
#include <string_view>

#include "TokenType.hpp"

class Token {
public:
    const TokenType type;
    const std::string_view lexeme;
    const void *literal;
    const int line;

    Token(const TokenType type, const std::string_view lexeme,
        const void* literal, const int line) :
        type(type), lexeme(lexeme), literal(literal), line(line) {}

    std::string toString();
};