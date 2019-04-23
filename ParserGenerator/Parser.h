#ifndef PARSER_GENERATOR_PARSER_H
#define PARSER_GENERATOR_PARSER_H


#include <memory>
#include "GrammarSymbol.h"
#include "NonTerminalSymbol.h"
#include "../LexicalAnalysis/LexicalAnalyzer.h"

class Parser {
public:
    typedef std::deque<std::shared_ptr<GrammarSymbol>> Sentence;
    std::unordered_map<std::string, std::shared_ptr<NonTerminalSymbol>> nonTerminals;
    Parser(LexicalAnalyzer&, std::shared_ptr<NonTerminalSymbol>
            startingSymbol, std::unordered_map<std::string,
            std::shared_ptr<TerminalSymbol>>& terminals,
            std::unordered_map<std::string, std::shared_ptr<NonTerminalSymbol>>&
            nonTerminals);
    // Loads parser from saved representation
    Parser(LexicalAnalyzer&, std::istream&);

    // Saves the parser according to the Parsers Saved Representation Format
    void save(std::ostream&);

    void initProgramParse(std::istream&);
    Sentence getNextDerivation();

    void parseFullProgram(std::istream&);
private:
    // The lexical analyzer for the language that this parser parses
    LexicalAnalyzer lexicalAnalyzer;
    std::shared_ptr<NonTerminalSymbol> startingSymbol;
    std::unordered_map<std::string, std::shared_ptr<TerminalSymbol>> terminals;


    std::unique_ptr<std::istream> programStream;
    std::shared_ptr<NonTerminalSymbol> currentSymbol = startingSymbol;
};


#endif //PARSER_GENERATOR_PARSER_H
