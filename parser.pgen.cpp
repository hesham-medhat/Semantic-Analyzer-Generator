#include <fstream>
#include <iostream>
#include <libgen.h>
#include <memory>
#include "ParserGenerator/Parser.h"
#include "build/GeneratedSemanticAnalyzer.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "USAGE: " << basename(argv[0]) << " <source>" << std::endl;
    return 1;
  }

  std::ifstream sourceFile(argv[1]);
  if (!sourceFile) {
    std::cerr << "Cannot open source code file" << std::endl;
    return 1;
  }

  std::ifstream lexerFile("pgen.lexer");
  if (!lexerFile) {
    std::cerr << "Cannot open lexer file: pgen.lexer" << std::endl;
    return 1;
  }

  std::ifstream parserFile("pgen.parser");
  if (!parserFile) {
    std::cerr << "Cannot open parser file: pgen.parser" << std::endl;
    return 1;
  }

  LexicalAnalyzer lex(lexerFile);
  Parser parser(lex, parserFile);
  std::ofstream offf("FINALSTAGE");
  parser.save(offf);
  std::unique_ptr<SemanticAnalyzerFactory> fact =
    std::make_unique<SemanticAnalyzerFactory>();
  parser.setSemanticAnalyzerFactory(std::move(fact));
  parser.initProgramParse(argv[1]);
  parser.parseFullProgram(sourceFile);

  return 0;
}
