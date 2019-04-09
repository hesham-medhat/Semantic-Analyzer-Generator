#include <iostream>
#include <string>
#include <fstream>

#include "LexicalAnalysis/LexicalAnalyzerGenerator.h"
#include "LexicalAnalysis/LexicalAnalyzer.h"

int main(int argc, char* argv[]) {
  if (argc < 3) return 1;

  std::ifstream rulesFile(argv[1]);
  if (!rulesFile) {
    std::cout << "Could not open " << argv[1] << std::endl;
    return 1;
  }

  std::cout << "Initializing Lexical Analyzer Generator..." << std::endl;
  LexicalAnalyzerGenerator lexicalAnalyzerGenerator(rulesFile);
  std::cout << "Generating Lexical Analyzer..." << std::endl;
  LexicalAnalyzer lexicalAnalyzer
    = lexicalAnalyzerGenerator.buildLexicalAnalyzer();

  std::cout << "Saving Automaton..." << std::endl;
  if (std::ofstream tmpout = std::ofstream("tmp"))
    lexicalAnalyzer.saveLexicalAnalyzerAutomaton(tmpout);

  std::cout << "Loading Automaton..." << std::endl;
  if (std::ifstream tmpin = std::ifstream("tmp"))
    lexicalAnalyzer = LexicalAnalyzer(tmpin);

  std::cout << "Analyzing Program..." << std::endl;
  lexicalAnalyzer.analyzeCompleteProgram(argv[2]);

  return 0;
}
