#include <iostream>
#include <string>

#include "LexicalAnalysis/LexicalAnalyzerGenerator.h"
#include "LexicalAnalysis/LexicalAnalyzer.h"

int main() {
    std::cout << "Rules input file relative path: " << std::endl;

    std::string inputFilePath;
    std::cin >> inputFilePath; // Read input file path

    std::cout << "Program file relative path: " << std::endl;

    std::string programFilePath;
    std::cin >> programFilePath; // Read input file path

    // Build lexical analyzer
    LexicalAnalyzerGenerator lexicalAnalyzerGenerator(&inputFilePath);
    LexicalAnalyzer* lexicalAnalyzer = lexicalAnalyzerGenerator.buildLexicalAnalyzer();

    lexicalAnalyzer->analyzeCompleteProgram(&programFilePath);

    return 0;
}
