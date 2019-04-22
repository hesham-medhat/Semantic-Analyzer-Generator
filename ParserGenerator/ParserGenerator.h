#ifndef PARSER_GENERATOR_PARSERGENERATOR_H
#define PARSER_GENERATOR_PARSERGENERATOR_H


#include "../LexicalAnalysis/LexicalAnalyzer.h"
#include "Parser.h"

/* Utility class that is used to generate the parser given input
 * stream of grammar rules and their lexical analyzer */
class ParserGenerator {
public:
    static Parser generateParser(std::istream&, LexicalAnalyzer&);
private:
    ParserGenerator(); // Not implemented on purpose
};


#endif //PARSER_GENERATOR_PARSERGENERATOR_H
