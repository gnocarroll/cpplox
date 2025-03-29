#include "Scanner.hpp"

#include <cstdlib>

#include "Lox.hpp"

void Scanner::scanTokens() {
	while (!isAtEnd()) {
		start = current;
		scanToken();
	}

	tokens.push_back(Token(TokenType::TOK_EOF, "", nullptr, line));
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

	auto number = Scanner::parseNumber(currentSubstr());

	LoxObject numberObj;

	if (!number) {
		Lox::error(line, "could not parse number (shouldn't happen)");
		numberObj = std::move(LoxObject(LoxObject::NUMBER));
	}
	else numberObj = std::move(LoxObject(*number));

	addToken(TokenType::NUMBER, numberObj);
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

std::optional<double> Scanner::parseNumber(const std::string_view sv) {
	if (sv.size() == 0) return {};

	const char* cStr = sv.data();
	const char* realEnd = cStr + sv.size();
	char* foundEnd = nullptr;

	std::optional<std::string> str;

	if (sv[sv.size()] != '\0') {
		str = std::string(sv);
		
		cStr = str->c_str();
		realEnd = cStr + str->size();
	}

	double ret = std::strtod(cStr, &foundEnd);

	if (foundEnd != realEnd) return {};

	return ret;
}

const std::string_view Scanner::currentSubstr() const {
	return std::string_view(
		source.data() + start,
		current - start
	);
}