#ifndef OUR_JAVA_COMPILER_TOKEN_H
#define OUR_JAVA_COMPILER_TOKEN_H


#include <string>

class Token {
public:
    Token(const std::string& type, int priority);

    std::string getType();
    int getPriority();
    std::string getLexeme();
    void setLexeme(std::string&);
private:
    std::string type;
    std::string lexeme;
    int priority;

};


#endif
