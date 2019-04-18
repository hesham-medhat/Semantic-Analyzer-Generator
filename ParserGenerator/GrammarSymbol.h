#ifndef PARSER_GENERATOR_GRAMMERSYMBOL_H
#define PARSER_GENERATOR_GRAMMERSYMBOL_H


#include <string>
#include <memory>
#include <deque>

class GrammarSymbol {
public:
    typedef std::deque<std::shared_ptr<GrammarSymbol>> Production;

    GrammarSymbol(std::string name);
    enum Type {NonTerminal, Terminal};

    Type getType();
    std::string& getName();
protected:
    const Type type;
private:
    const std::string name;
};


#endif //PARSER_GENERATOR_GRAMMERSYMBOL_H
