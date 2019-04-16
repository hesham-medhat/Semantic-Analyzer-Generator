#ifndef PARSER_GENERATOR_PARSER_H
#define PARSER_GENERATOR_PARSER_H


#include <deque>
#include <memory>
#include "GrammarSymbol.h"
#include "NonTerminalSymbol.h"
#include "../LexicalAnalysis/LexicalAnalyzer.h"

class Parser {
public:
    typedef std::deque<std::shared_ptr<GrammarSymbol>> Sentence;

    Parser(LexicalAnalyzer&, std::shared_ptr<NonTerminalSymbol>);
    // Loads parser from saved representation
    Parser(LexicalAnalyzer&, std::istream&);

    void save(std::ostream&);

    void initProgramParse(std::istream&);
    Sentence getNextDerivation();

    void parseFullProgram(std::istream&);
private:
    // The lexical analyzer for the language that this parser parses
    LexicalAnalyzer& lexicalAnalyzer;
    std::shared_ptr<NonTerminalSymbol> startingSymbol;

    std::istream& programStream;
    std::shared_ptr<NonTerminalSymbol> currentSymbol = startingSymbol;
};


#endif //PARSER_GENERATOR_PARSER_H
