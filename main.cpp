#include <iostream>
#include <string>
#include <fstream>

#include "LexicalAnalysis/LexicalAnalyzerGenerator.h"
#include "LexicalAnalysis/LexicalAnalyzer.h"

int main(int argc, char* argv[]) {
    string rules="C:\\Users\\Ahmed\\Desktop\\text.txt";
    string code="C:\\Users\\Ahmed\\Desktop\\text2.txt";
  std::ifstream rulesFile(rules);
  if (!rulesFile) {
    std::cout << "Could not open " << argv[1] << std::endl;
    return 1;
  }

  std::cout << "Initializing Lexical Analyzer Generator..." << std::endl;
  LexicalAnalyzerGenerator lexicalAnalyzerGenerator(rulesFile);
  std::cout << "Generating Lexical Analyzer..." << std::endl;
  LexicalAnalyzer lexicalAnalyzer
    = lexicalAnalyzerGenerator.buildLexicalAnalyzer();

  std::ofstream outputStream("automaton.txt");
  lexicalAnalyzer.saveLexicalAnalyzerAutomaton(outputStream);
  outputStream.close();

  std::ifstream iStream("automaton.txt");
  LexicalAnalyzer loadedLexicalAnalyzer(iStream);

  std::cout << "Analyzing Program..." << std::endl;
  loadedLexicalAnalyzer.analyzeCompleteProgram(code);

  return 0;
}
