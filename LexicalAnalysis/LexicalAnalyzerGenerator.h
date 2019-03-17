#ifndef OUR_JAVA_COMPILER_LEXICALANALYZERGENERATOR_H
#define OUR_JAVA_COMPILER_LEXICALANALYZERGENERATOR_H


#include <string>

#include "LexicalAnalyzer.h"

class LexicalAnalyzerGenerator {
public:
    // Constructor that takes the path of input containing rules
    LexicalAnalyzerGenerator(std::istream& inputStream);

    // Generates LexicalAnalyzer according to the given rules
    LexicalAnalyzer& buildLexicalAnalyzer();

private:
    const std::istream& inputStream;

    Automaton buildNFA();
    Automaton convertNFAToDFA(Automaton nfa);
    Automaton reduceDFA(Automaton dfa);
};


#endif
