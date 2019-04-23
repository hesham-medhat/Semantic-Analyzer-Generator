#include <fstream>
#include <iostream>
#include <libgen.h>
#include "LexicalAnalysis/LexicalAnalyzer.h"
#include "ParserGenerator/ParserGenerator.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "USGAE: "
              << basename(argv[0]) << " <rules-file>" << std::endl;
    return 1;
  }

  std::ifstream rulesFile(argv[1]);
  if (!rulesFile) {
    std::cerr << "Cannot open rules file" << std::endl;
    return 1;
  }

  LexicalAnalyzer dummyLex = LexicalAnalyzer(Automaton());
  Parser parser = ParserGenerator::generateParser(rulesFile, dummyLex);

  std::ofstream otmp("tmpfile");
  parser.save(otmp);
  std::ifstream itmp("tmpfile");
  Parser loadedParser(dummyLex, itmp);

  for (const auto& symbol : parser.nonTerminals) {
    for (const auto& prod : symbol.second->productions) {
      std::cout << symbol.first << " ::= ";
      for (const auto& rhsTerm : prod) {
        if (rhsTerm->getType() == GrammarSymbol::Terminal) {
          std::cout << "'" << rhsTerm->getName() << "' ";
        } else {
          std::cout << rhsTerm->getName() << " ";
        }
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }

  std::cout << "LOADED PARSER:\n" << std::endl;
  for (const auto& symbol : parser.nonTerminals) {
    for (const auto& prod : symbol.second->productions) {
      std::cout << symbol.first << " ::= ";
      for (const auto& rhsTerm : prod) {
        if (rhsTerm->getType() == GrammarSymbol::Terminal) {
          std::cout << "'" << rhsTerm->getName() << "' ";
        } else {
          std::cout << rhsTerm->getName() << " ";
        }
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }

  return 0;
}
