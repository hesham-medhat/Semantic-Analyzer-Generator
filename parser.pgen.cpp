#include "ParserGenerator/Parser.h"
#include "build/GeneratedSemanticAnalyzer.h"
#include <fstream>
#include <iostream>
#include <libgen.h>
#include <memory>

int main(int argc, char *argv[]) {
  constexpr auto SAVED_LEXER_FNAME = "pgen.lexer";
  constexpr auto SAVED_PARSER_FNAME = "pgen.parser";

  if (argc != 2) {
    std::cout << "USAGE: " << basename(argv[0]) << " <source>" << std::endl;
    return 1;
  }

  std::ifstream sourceFile(argv[1]);
  if (!sourceFile) {
    std::cerr << "Cannot open source code file" << std::endl;
    return 1;
  }

  std::ifstream lexerFile(SAVED_LEXER_FNAME);
  if (!lexerFile) {
    std::cerr << "Cannot open lexer file: " << SAVED_LEXER_FNAME << std::endl;
    return 1;
  }

  std::ifstream parserFile(SAVED_PARSER_FNAME);
  if (!parserFile) {
    std::cerr << "Cannot open parser file: " << SAVED_PARSER_FNAME << std::endl;
    return 1;
  }

  LexicalAnalyzer lex(lexerFile);
  Parser parser(lex, parserFile);
  std::unique_ptr<SemanticAnalyzerFactory> fact =
      std::make_unique<SemanticAnalyzerFactory>();
  parser.setSemanticAnalyzerFactory(std::move(fact));
  parser.initProgramParse(argv[1]);
  parser.parseFullProgram(sourceFile);

  return 0;
}
