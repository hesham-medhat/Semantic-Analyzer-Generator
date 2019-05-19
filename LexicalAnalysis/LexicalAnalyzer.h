#ifndef OUR_JAVA_COMPILER_LEXICALANALYZER_H
#define OUR_JAVA_COMPILER_LEXICALANALYZER_H

#include <string>

#include "Automata/Automaton.h"
#include "Token.h"

class LexicalAnalyzer {
public:
  LexicalAnalyzer(const LexicalAnalyzer &);
  LexicalAnalyzer(const Automaton &languageAutomaton);
  LexicalAnalyzer(std::istream &inputDFAStream);

  // Analyzes given program as a whole generating output at once
  void analyzeCompleteProgram(const std::string &programFilePath);
  // Initialize parse sequence
  void initProgramParse(const std::string &programFilePath);
  // Gets next token for the parser. Returns nullptr if uninitialized and empty
  // token when done
  Token nextToken();

  void saveLexicalAnalyzerAutomaton(std::ostream &outputStream);

private:
  Automaton languageAutomaton;
  std::unique_ptr<std::istream> programIstream;
  std::shared_ptr<State> LACurrentState;
};

#endif
