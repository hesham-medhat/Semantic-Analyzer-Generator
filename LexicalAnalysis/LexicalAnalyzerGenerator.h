#ifndef OUR_JAVA_COMPILER_LEXICALANALYZERGENERATOR_H
#define OUR_JAVA_COMPILER_LEXICALANALYZERGENERATOR_H


#include "LexicalAnalyzer.h"

class LexicalAnalyzerGenerator {
public:
    // Constructor that takes the path of input containing rules
    LexicalAnalyzerGenerator(std::istream& inputStream);

    // Generates LexicalAnalyzer according to the given rules
    LexicalAnalyzer buildLexicalAnalyzer();

private:
    const std::istream& inputStream;

    Automaton buildNFA();
    Automaton convertNFAToDFA(Automaton nfa);
    Automaton reduceDFA(Automaton dfa);

    int getFormat(std::string token);

    void updateKeywords(std::string token);

    void updatePunctuations(std::string token);

    std::shared_ptr<Automaton> createExpAutomaton(std::string tokenName, std::string token);

    void performOp(Token& t);

    bool isOperation(char op);

    int precedence(char op);

    std::shared_ptr<Automaton> createDefAutomaton(std::string name, std::string token, int priority);

    bool isLetter(char c);

    std::shared_ptr<Automaton> getAutomatonForWord(std::string name, std::string token,int priority);
};


#endif
