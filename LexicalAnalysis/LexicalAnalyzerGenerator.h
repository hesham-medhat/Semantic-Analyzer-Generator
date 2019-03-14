#ifndef OUR_JAVA_COMPILER_LEXICALANALYZERGENERATOR_H
#define OUR_JAVA_COMPILER_LEXICALANALYZERGENERATOR_H


#include <string>

#include "LexicalAnalyzer.h"

class LexicalAnalyzerGenerator {
public:
    // Constructor that takes the path of input file containing rules
    LexicalAnalyzerGenerator(std::string* inputFilePath);

    // Generates LexicalAnalyzer according to the given rules
    LexicalAnalyzer* buildLexicalAnalyzer();

private:
    const std::string* inputFilePath;
};


#endif
