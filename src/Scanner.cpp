#include "Scanner.hpp"

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
		Lox::error(line, "Unexpected char.");
	}
}

void Scanner::addToken(TokenType type) {
	addToken(type, nullptr);
}

void Scanner::addToken(TokenType type, void* literal) {
	// get relevant substr for token
	
	const std::string_view lexeme(
		source.data() + start,
		current - start
	);

	tokens.push_back(Token(type, lexeme, literal, line));
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
}