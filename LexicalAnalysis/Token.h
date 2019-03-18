#ifndef OUR_JAVA_COMPILER_TOKEN_H
#define OUR_JAVA_COMPILER_TOKEN_H


#include <string>

class Token {
public:
    Token(std::string& type);

    std::string getType();
    std::string getLexeme();
    std::string setLexeme(std::string&);
private:
    std::string type;
    std::string lexeme;
};


#endif
