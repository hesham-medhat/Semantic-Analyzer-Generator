#include "Token.h"

Token::Token(const std::string& type)
: type(type) { }


std::string Token::getType() { return type; }

std::string Token::getLexeme() { return lexeme; }

std::string Token::setLexeme(std::string &value) { lexeme = value; }
