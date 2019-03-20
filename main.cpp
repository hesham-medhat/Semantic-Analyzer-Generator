/*
#include <iostream>
#include <string>
#include <fstream>

#include "LexicalAnalysis/LexicalAnalyzerGenerator.h"
#include "LexicalAnalysis/LexicalAnalyzer.h"

int main() {
    std::cout << "Rules input file relative path: " << std::endl;

    std::string inputRulesFilePath;
    std::cin >> inputRulesFilePath; // Read input file path
    std::ifstream inputRulesStream(inputRulesFilePath);

    std::cout << "Program file relative path: " << std::endl;

    std::string programFilePath;
    std::cin >> programFilePath; // Read input file path

    // Build lexical analyzer
    LexicalAnalyzerGenerator lexicalAnalyzerGenerator(inputRulesStream);
    LexicalAnalyzer lexicalAnalyzer = lexicalAnalyzerGenerator.buildLexicalAnalyzer();

    lexicalAnalyzer.analyzeCompleteProgram(programFilePath);

    return 0;
}
*/
