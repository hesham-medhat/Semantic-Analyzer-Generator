#include "Token.h"

Token::Token(const std::string &type, const int priority)
    : type(type), priority(priority) {}

std::string Token::getType() { return type; }

std::string Token::getLexeme() { return lexeme; }

int Token::getPriority() { return priority; };

void Token::setLexeme(std::string &value) { lexeme = value; }
