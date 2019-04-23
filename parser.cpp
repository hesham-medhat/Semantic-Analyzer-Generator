#include <fstream>
#include <iostream>
#include <libgen.h>
#include "LexicalAnalysis/LexicalAnalyzerGenerator.h"
#include "ParserGenerator/ParserGenerator.h"

int main(int argc, char* argv[]) {

  std::string x = "C:\\Users\\first\\Desktop\\program.txt";
  char xc[x.size()];
  strcpy(xc,x.c_str());
  std::string y = "C:\\Users\\first\\Desktop\\lexer-rules.txt";
  char yc[y.size()];
  strcpy(yc,y.c_str());
  std::string z = "C:\\Users\\first\\Desktop\\parser-rules.txt";
  char zc[z.size()];
  strcpy(zc,z.c_str());


 std::ifstream lexRulesFile(yc);
  if (!lexRulesFile) {
    std::cerr << "Cannot open file: " << yc << std::endl;
    return 1;
  }

  std::ifstream parseRulesFile(zc);
  if (!parseRulesFile) {
    std::cerr << "Cannot open file: " << zc << std::endl;
    return 1;
  }

  std::ifstream programFile(xc);
  if (!programFile) {
    std::cerr << "Cannot open file: " << xc << std::endl;
    return 1;
  }

  std::cout << "Generating lexical analyzer..." << std::endl;
  LexicalAnalyzerGenerator lexGen(lexRulesFile);
  LexicalAnalyzer lex = lexGen.buildLexicalAnalyzer();

  std::cout << "Generating parser..." << std::endl;
  Parser parser = ParserGenerator::generateParser(parseRulesFile, lex);

  std::cout << "Parsing program..." << std::endl;
  parser.initProgramParse(xc);
  parser.parseFullProgram(programFile);

  return 0;
}
