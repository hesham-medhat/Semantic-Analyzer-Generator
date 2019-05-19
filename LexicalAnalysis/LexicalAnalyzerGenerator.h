#ifndef OUR_JAVA_COMPILER_LEXICALANALYZERGENERATOR_H
#define OUR_JAVA_COMPILER_LEXICALANALYZERGENERATOR_H

#include "LexicalAnalyzer.h"
#include <climits>
#include <queue>
#include <stack>
#include <string.h>
using namespace std;

class LexicalAnalyzerGenerator {
public:
  // Constructor that takes the path of input containing rules
  LexicalAnalyzerGenerator(std::istream &inputStream);

  // Generates LexicalAnalyzer according to the given rules
  LexicalAnalyzer buildLexicalAnalyzer();

private:
  static Automaton whitespaceAcceptor;
  const std::istream &inputStream;

  Automaton buildNFA();

  Automaton convertNFAToDFA(Automaton NFA);

  Automaton reduceDFA(Automaton dfa);

  int getFormat(std::string token);

  void updateKeywords(std::string token);

  void updatePunctuations(std::string token);

  void performOp(Token &t, stack<shared_ptr<Automaton>> &operands,
                 std::stack<char> &operators);

  bool isOperation(char op);

  int precedence(char op);

  std::shared_ptr<Automaton>
  createDefAutomaton(std::string name, std::string token, int priority);

  bool isLetter(char c);

  std::shared_ptr<Automaton>
  getAutomatonForWord(std::string name, std::string token, int priority);

  unordered_set<shared_ptr<State>> getLambdaClosure(shared_ptr<State> state);

  Automaton minimizeDFA(unordered_set<shared_ptr<State>> AllDFAStates,
                        shared_ptr<State> startState);
};

#endif
