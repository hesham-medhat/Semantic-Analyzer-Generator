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

  return 0;
}
