#include <fstream>
#include <iostream>
#include <libgen.h>
#include "LexicalAnalysis/LexicalAnalyzerGenerator.h"
#include "ParserGenerator/ParserGenerator.h"

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cout << "USGAE: "
              << basename(argv[0])
              << " <lexer-rules-file>"
              << " <parser-rules-file>"
              << std::endl;
    return 1;
  }

  std::ifstream lexerRulesFile(argv[1]);
  if (!lexerRulesFile) {
    std::cerr << "Cannot open lexer rules file" << std::endl;
    return 1;
  }

  std::ifstream parserRulesFile(argv[2]);
  if (!parserRulesFile) {
    std::cerr << "Cannot open parser rules file" << std::endl;
    return 1;
  }

  LexicalAnalyzerGenerator lexGen(lexerRulesFile);
  LexicalAnalyzer lex = lexGen.buildLexicalAnalyzer();
  Parser parser = ParserGenerator::generateParser(parserRulesFile, lex);

  // TODO: write parser source code to output file

  return 0;
}
