#include <fstream>
#include <iostream>
#include <libgen.h>
#include "LexicalAnalysis/LexicalAnalyzerGenerator.h"
#include "ParserGenerator/ParserGenerator.h"

int main(int argc, char* argv[]) {
  constexpr auto SAVED_LEXER_FNAME = "pgen.lexer";
  constexpr auto SAVED_PARSER_FNAME = "pgen.parser";

  if (argc != 3) {
    std::cout << "USAGE: "
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
  std::cout << "Generating lexical analyzer..." << std::endl;
  LexicalAnalyzer lex = lexGen.buildLexicalAnalyzer();

  std::cout << "Generating parser and semantic analyzer..." << std::endl;
  Parser parser = ParserGenerator::generateParser(parserRulesFile, lex);
  std::cout << "Parser implementation written to pgen.parser.cpp" << std::endl;

  std::ofstream savedLexer(SAVED_LEXER_FNAME);
  if (!savedLexer) {
    std::cerr << "Cannot open file to write generated lexer: "
              << SAVED_LEXER_FNAME << std::endl;
    return 1;
  }

  std::ofstream savedParser(SAVED_PARSER_FNAME);
  if (!savedParser) {
    std::cerr << "Cannot open file to write generated parser: "
              << SAVED_PARSER_FNAME << std::endl;
    return 1;
  }

  std::cout << "Saving generated lexer to " << SAVED_LEXER_FNAME << "..."
            << std::endl;
  lex.saveLexicalAnalyzerAutomaton(savedLexer);

  std::cout << "Saving generated parser to " << SAVED_PARSER_FNAME << "..."
            << std::endl;
  parser.save(savedParser);

  return 0;
}
