#include <iostream>
#include <fstream>
#include "LexicalAnalyzer.h"

LexicalAnalyzer::LexicalAnalyzer(const Automaton& languageAutomaton)
: languageAutomaton(languageAutomaton) { }

LexicalAnalyzer::LexicalAnalyzer(std::istream& inputDFAStream)
: languageAutomaton(inputDFAStream) { }

void LexicalAnalyzer::analyzeCompleteProgram(std::string& programFilePath) {
  std::ifstream input(programFilePath); if (!input) return;
  std::ofstream output(programFilePath + ".tokens"); if (!output) return;

  char currentChar;
  std::shared_ptr<State> currentState = languageAutomaton.startState;
  std::shared_ptr<State> prevState;
  std::string lastMatchedToken;
  std::string currentToken;
  std::ifstream::pos_type anchor = 0;
  std::ifstream::pos_type lastMatchedPos = -1;

  while (input.get(currentChar)) {
    currentToken += currentChar;
    std::unordered_set<std::shared_ptr<State>> nextStateSet
      = currentState->getNextState(currentChar);
    prevState = currentState;
    currentState = nextStateSet.empty() ? nullptr : *nextStateSet.begin();

    if (currentState) {
      std::string token = currentState->getAcceptedToken().getType();
      if (!token.empty()) {
        lastMatchedToken = token;
        lastMatchedPos = input.tellg();
      }
    } else if (!lastMatchedToken.empty()) {
      output << lastMatchedToken << std::endl;
      anchor = lastMatchedPos;
      input.seekg(anchor);
      lastMatchedToken = "";
      lastMatchedPos = -1;
      currentState = languageAutomaton.startState;
    } else {
      std::cerr << "[ERROR] Unrecognized token: " << currentToken << std::endl;
      // Discard one prefix character and restart scanning
      anchor += 1;
      input.seekg(anchor);
      currentToken.clear();
      currentState = languageAutomaton.startState;
    }
  }
}
