#include "Token.hpp"

#include <cstdlib>

#include "Util.hpp"

std::string Token::toString() {
    return Util::SVsToString({ type.toStringView(), lexeme });
}
