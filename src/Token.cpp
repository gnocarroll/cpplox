#include "Token.hpp"

#include <cstdlib>

#include "Util.hpp"

std::string Token::toString() {
    return SVsToString({ type.toStringView(), lexeme });
}