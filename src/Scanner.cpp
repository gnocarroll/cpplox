#include "Scanner.hpp"

#include <cstdlib>

#include "Lox.hpp"

const std::unordered_map<
	std::string_view,
	TokenType
> Scanner::keywords = std::unordered_map<
	std::string_view,
	TokenType
>{

#define DEF_KEYWORD(lower, upper) {#lower, TokenType::upper},

#include "Keywords.info"

#undef DEF_KEYWORD

	// adding this to make macro work -- kind of jank but there will
	// never be an identifier found that is just "" unless something
	// is broken

	{"", TokenType::VAR}
};

void Scanner::scanTokens() {
	while (!isAtEnd()) {
		start = current;
		scanToken();
	}

	LoxObject obj;

	tokens.push_back(Token(
		TokenType::TOK_EOF,
		std::string_view(""),
		obj,
		line
	));

	hasScanned = true;
}

void Scanner::scanToken() {
	char c = advance();

	switch (c) {
	// single char tokens

	case '(': addToken(TokenType::LEFT_PAREN); break;
	case ')': addToken(TokenType::RIGHT_PAREN); break;
	case '{': addToken(TokenType::LEFT_BRACE); break;
	case '}': addToken(TokenType::RIGHT_BRACE); break;
	case ',': addToken(TokenType::COMMA); break;
	case '.': addToken(TokenType::DOT); break;
	case '-': addToken(TokenType::MINUS); break;
	case '+': addToken(TokenType::PLUS); break;
	case ';': addToken(TokenType::SEMICOLON); break;
	case '*': addToken(TokenType::STAR); break;

	// maybe double char

	case '!':
		addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
		break;
	case '=':
		addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
		break;
	case '<':
		addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
		break;
	case '>':
		addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
		break;

	// '/' - comment or division

	case '/':
		if (match('/')) { // => comment
			while (peek() != '\n' && !isAtEnd()) advance();
		}
		else if (match('*')) { // => block comment (challenge)
			blockComment();
		}
		else addToken(TokenType::SLASH);
		break;

	case ' ':
	case '\r':
	case '\t':
		break;

	case '\n':
		line++;
		break;

	case '"': string(); break; // string literal

	default:
		if (Scanner::isDigit(c)) {
			number();
		}
		else if (Scanner::isAlpha(c)) {
			identifier();
		}
		else Lox::error(line, "Unexpected char.");
	}
}

void Scanner::addToken(TokenType type) {
	LoxObject empty;

	addToken(type, empty);
}

void Scanner::addToken(TokenType type, LoxObject& literal) {
	// get relevant substr for token

	tokens.push_back(Token(type, currentSubstr(), literal, line));
}

void Scanner::string() {
	while (peek() != '"' && !isAtEnd()) {
		if (peek() == '\n') line++;
		advance();
	}

	if (isAtEnd()) {
		Lox::error(line, "unterminated string");
		return;
	}

	advance(); // closing ""

	// we exclude surrounding quotes

	const std::string_view value(
		source.data() + start - 1,
		current - start - 2
	);

	LoxObject stringObj(value);

	addToken(TokenType::STRING, stringObj);
}

void Scanner::number() {
	while (Scanner::isDigit(peek())) advance();

	if (peek() == '.' && isDigit(peekNext())) {
		advance();

		while (isDigit(peek())) advance();
	}

	// needs to be null-terminated for c provided str to double conversion,
	// so will create std::string
	std::string numStr(currentSubstr());

	auto number = Scanner::parseNumber(numStr);

	LoxObject numberObj;

	if (!number) {
		Lox::error(line, "could not parse number (shouldn't happen)");
		numberObj = std::move(LoxObject(LoxObject::NUMBER));
	}
	else numberObj = std::move(LoxObject(*number));

	addToken(TokenType::NUMBER, numberObj);
}

void Scanner::identifier() {
	while (isAlphanumeric(peek())) advance();

	TokenType type = TokenType::IDENTIFIER;

	std::string_view substr = currentSubstr();

	if (keywords.count(substr)) type = keywords.at(substr);

	addToken(type);
}

void Scanner::blockComment() {
	size_t nestCount = 1;

	while (nestCount > 0) {
		char c = advance();

		if (c == '\n') {
			line++;
		}
		else if (c == '\0') {
			Lox::error(line, "unterminated block comment");
			return;
		}

		if ((c == '/') && (peek() == '*')) {
			nestCount++;
			advance();
		}
		else if ((c == '*') && (peek() == '/')) {
			nestCount--;
			advance();
		}
	}
}

// utilities for testing what char is

bool Scanner::isDigit(char c) {
	return c >= '0' && c <= '9';
}
bool Scanner::isLower(char c) {
	return c >= 'a' && c <= 'z';
}
bool Scanner::isUpper(char c) {
	return c >= 'A' && c <= 'Z';
}
bool Scanner::isLetter(char c) {
	return isLower(c) || isUpper(c);
}
bool Scanner::isAlpha(char c) {
	return isLetter(c) || c == '_';
}
bool Scanner::isAlphanumeric(char c) {
	return isAlpha(c) || isDigit(c);
}

std::optional<double> Scanner::parseNumber(NTStringView sv) {
	if (sv.size() == 0) return {};

	char* foundEnd = nullptr;

	double ret = std::strtod(sv.data(), &foundEnd);

	if (foundEnd != (sv.data() + sv.size())) return {};

	return ret;
}

std::string_view Scanner::currentSubstr() const {
	return std::string_view(
		source.data() + start,
		current - start
	);
}