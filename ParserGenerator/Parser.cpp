#include "Parser.h"

Parser::Parser(LexicalAnalyzer & lexicalAnalyzer,
               std::shared_ptr<NonTerminalSymbol> startingSymbol,
               std::unordered_map<std::string,
                       std::shared_ptr<TerminalSymbol>> &terminals,
               std::unordered_map<std::string,
               std::shared_ptr<NonTerminalSymbol>> &nonTerminals)
               : lexicalAnalyzer(lexicalAnalyzer), startingSymbol
               (std::move(startingSymbol)), terminals(terminals),
               nonTerminals(nonTerminals) {}