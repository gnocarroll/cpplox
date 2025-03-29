#pragma once

#include <cassert>
#include <string_view>

#define FOR_EACH_TOK(DO) \
	DO(LEFT_PAREN) \
	DO(RIGHT_PAREN) \
	DO(LEFT_BRACE) \
	DO(RIGHT_BRACE) \
	DO(COMMA) \
	DO(DOT) \
	DO(MINUS) \
	DO(PLUS) \
	DO(SEMICOLON) \
	DO(SLASH) \
	DO(STAR) \
	DO(BANG) \
	DO(BANG_EQUAL) \
	DO(EQUAL) \
	DO(EQUAL_EQUAL) \
	DO(GREATER) \
	DO(GREATER_EQUAL) \
	DO(LESS) \
	DO(LESS_EQUAL) \
	DO(IDENTIFIER) \
	DO(STRING) \
	DO(NUMBER) \
	DO(AND) \
	DO(CLASS) \
	DO(ELSE) \
	DO(FALSE) \
	DO(FUN) \
	DO(FOR) \
	DO(IF) \
	DO(NIL) \
	DO(OR) \
	DO(PRINT) \
	DO(RETURN) \
	DO(SUPER) \
	DO(THIS) \
	DO(TRUE) \
	DO(VAR) \
	DO(WHILE) \
	DO(TOK_EOF) \
	DO(LOX_TOKENTYPE_COUNT)

class TokenType {
	static constexpr char *cStrs[] = {
#define NAME_STR(name) (char * const) #name,

		FOR_EACH_TOK(NAME_STR)

#undef NAME_STR

		(char * const) ""
	};

public:
	enum Type {
#define NAME_COMMA(name) name,

		FOR_EACH_TOK(NAME_COMMA)

#undef NAME_COMMA
	};

private:
	enum Type type;

	template <typename T>
	operator T() const = delete;
public:
	TokenType(Type type) : type(type) {}
	
	const char* toCStr() {
		if (type < 0 || type >= LOX_TOKENTYPE_COUNT) return nullptr;

		return cStrs[type];
	}

	const std::string_view toStringView() {
		const char* cStr = toCStr();

		if (!cStr) return std::string_view("");

		return cStr;
	}
};

#undef FOR_EACH_TOK