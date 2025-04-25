#pragma once

#include <string_view>

#define INJECT_TOKS \
	TOK(LEFT_PAREN) \
	TOK(RIGHT_PAREN) \
	TOK(LEFT_BRACE) \
	TOK(RIGHT_BRACE) \
	TOK(COMMA) \
	TOK(DOT) \
	TOK(MINUS) \
	TOK(PLUS) \
	TOK(SEMICOLON) \
	TOK(SLASH) \
	TOK(STAR) \
	TOK(BANG) \
	TOK(BANG_EQUAL) \
	TOK(EQUAL) \
	TOK(EQUAL_EQUAL) \
	TOK(GREATER) \
	TOK(GREATER_EQUAL) \
	TOK(LESS) \
	TOK(LESS_EQUAL) \
	TOK(IDENTIFIER) \
	TOK(STRING) \
	TOK(NUMBER) \
	TOK(AND) \
	TOK(CLASS) \
	TOK(ELSE) \
	TOK(FALSE) \
	TOK(FUN) \
	TOK(FOR) \
	TOK(IF) \
	TOK(NIL) \
	TOK(OR) \
	TOK(PRINT) \
	TOK(RETURN) \
	TOK(SUPER) \
	TOK(THIS) \
	TOK(TRUE) \
	TOK(VAR) \
	TOK(WHILE) \
	TOK(TOK_EOF) \
	TOK(LOX_TOKENTYPE_COUNT)

class TokenType {
	static constexpr char *cStrs[] = {
#define TOK(name) (char * const) #name,

		INJECT_TOKS

#undef TOK

		(char * const) ""
	};

public:
	enum Type {
#define TOK(name) name,

		INJECT_TOKS

#undef TOK
	};

private:
	enum Type type;

	template <typename T>
	operator T() const = delete;
public:
	TokenType(Type type) : type(type) {}
	
	bool operator ==(const TokenType& other) const {
		return type == other.type;
	}
	bool operator !=(const TokenType& other) const {
		return type != other.type;
	}

	const char* toCStr() const {
		if (type < 0 || type >= LOX_TOKENTYPE_COUNT) return nullptr;

		return cStrs[type];
	}

	const std::string_view toStringView() const {
		const char* cStr = toCStr();

		if (!cStr) return std::string_view("");

		return cStr;
	}


	friend bool operator ==(const TokenType& left, const TokenType::Type& right) {
		return left.type == right;
	}
	friend bool operator !=(const TokenType& left, const TokenType::Type& right) {
		return left.type != right;
	}

	friend bool operator ==(const TokenType::Type& left, const TokenType& right) {
		return left == right.type;
	}
	friend bool operator !=(const TokenType::Type& left, const TokenType& right) {
		return left != right.type;
	}
};

#undef INJECT_TOKS
