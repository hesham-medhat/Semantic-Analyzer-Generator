#ifndef OUR_JAVA_COMPILER_LEXICALANALYZER_H
#define OUR_JAVA_COMPILER_LEXICALANALYZER_H


#include <string>

#include "Token.h"
#include "Automata/Automaton.h"

class LexicalAnalyzer {
public:
    LexicalAnalyzer(const Automaton& languageAutomaton);
    LexicalAnalyzer(std::istream& inputDFAStream);

    // Analyzes given program as a whole generating output at once
    void analyzeCompleteProgram(const std::string& programFilePath);
    // Initialize parse sequence
    void initProgramParse(std::string& programFilePath);
    // Gets next token for the parser. Returns nullptr if uninitialized and empty token when done
    Token nextToken();
private:
    Automaton languageAutomaton;
};


#endif
