#ifndef PARSER_GENERATOR_PARSER_H
#define PARSER_GENERATOR_PARSER_H

#include "../LexicalAnalysis/LexicalAnalyzer.h"
#include "../SemanticAnalyzerGenerator/AbstractSemanticAnalyzerFactory.h"
#include "GrammarSymbol.h"
#include "NonTerminalSymbol.h"
#include <memory>

class Parser {
public:
  typedef std::deque<std::shared_ptr<GrammarSymbol>> Sentence;

  std::unordered_map<std::string, std::shared_ptr<TerminalSymbol>> terminals;
  std::unordered_map<std::string, std::shared_ptr<NonTerminalSymbol>>
      nonTerminals;

  // Maps productions to their ids
  std::unordered_map<std::shared_ptr<Sentence>, int> productionIds;

  Parser(LexicalAnalyzer &, std::shared_ptr<NonTerminalSymbol> startingSymbol,
         std::unordered_map<std::string, std::shared_ptr<TerminalSymbol>>
             &terminals,
         std::unordered_map<std::string, std::shared_ptr<NonTerminalSymbol>>
             &nonTerminals);
  // Loads parser from saved representation
  Parser(LexicalAnalyzer &, std::istream &);

  // Saves the parser according to the Parsers Saved Representation Format
  void save(std::ostream &);

  void initProgramParse(const std::string &);
  Sentence getNextDerivation();

  void parseFullProgram(std::istream &);

  void setSemanticAnalyzerFactory(
      std::unique_ptr<AbstractSemanticAnalyzerFactory>);

private:
  // The lexical analyzer for the language that this parser parses
  LexicalAnalyzer lexicalAnalyzer;
  std::shared_ptr<NonTerminalSymbol> startingSymbol;

  // Semantic analysis
  std::unique_ptr<AbstractSemanticAnalyzerFactory> semanticAnalyzerFactory;

  // Current discrete parsing
  std::unique_ptr<std::istream> programStream;
  std::shared_ptr<NonTerminalSymbol> currentSymbol = startingSymbol;
  std::string lastToken;
};

#endif // PARSER_GENERATOR_PARSER_H
