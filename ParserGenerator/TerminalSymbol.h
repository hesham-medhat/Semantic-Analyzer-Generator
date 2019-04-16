#ifndef PARSER_GENERATOR_TERMINALSYMBOL_H
#define PARSER_GENERATOR_TERMINALSYMBOL_H


#include "GrammarSymbol.h"

class TerminalSymbol : public GrammarSymbol {
public:
    explicit TerminalSymbol(std::string name);
};


#endif //PARSER_GENERATOR_TERMINALSYMBOL_H
