#include <fstream>
#include <iostream>
#include <libgen.h>
#include "LexicalAnalysis/LexicalAnalyzerGenerator.h"
#include "ParserGenerator/ParserGenerator.h"

int main(int argc, char* argv[]) {
  if (argc != 4) {
    std::cout << "USGAE: "
              << basename(argv[0]) << " <lex-rules-file> "
                                   << " <parse-rules-file> "
                                   << " <program-file>" << std::endl;
    return 1;
  }

  std::ifstream lexRulesFile(argv[1]);
  if (!lexRulesFile) {
    std::cerr << "Cannot open file: " << argv[1] << std::endl;
    return 1;
  }

  std::ifstream parseRulesFile(argv[2]);
  if (!parseRulesFile) {
    std::cerr << "Cannot open file: " << argv[2] << std::endl;
    return 1;
  }

  std::ifstream programFile(argv[3]);
  if (!programFile) {
    std::cerr << "Cannot open file: " << argv[3] << std::endl;
    return 1;
  }

  std::cout << "Generating lexical analyzer..." << std::endl;
  LexicalAnalyzerGenerator lexGen(lexRulesFile);
  LexicalAnalyzer lex = lexGen.buildLexicalAnalyzer();

  std::cout << "Generating parser..." << std::endl;
  Parser parser = ParserGenerator::generateParser(parseRulesFile, lex);

  /* TODO: Create semantic analyzer factory and set to parser */

  std::cout << "Parsing program..." << std::endl;
  parser.initProgramParse(argv[3]);
  parser.parseFullProgram(programFile);

  return 0;
}
