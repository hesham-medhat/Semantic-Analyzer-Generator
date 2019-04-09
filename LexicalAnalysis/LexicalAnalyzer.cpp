#include <iostream>
#include <fstream>
#include "LexicalAnalyzer.h"

LexicalAnalyzer::LexicalAnalyzer(const Automaton& languageAutomaton)
: languageAutomaton(languageAutomaton) { }

LexicalAnalyzer::LexicalAnalyzer(std::istream& inputDFAStream)
: languageAutomaton(inputDFAStream) { }

void LexicalAnalyzer::analyzeCompleteProgram(
    const std::string& programFilePath) {
  std::ifstream input(programFilePath,
                      std::ios_base::in | std::ios_base::binary);
    if (!input) return;
  std::ofstream output(programFilePath + ".tokens"); if (!output) return;

  char currentChar;
  std::shared_ptr<State> currentState = languageAutomaton.startState;
  Token lastMatchedToken("", 0);
  std::string currentToken;
  std::ifstream::pos_type anchor = 0;
  std::ifstream::pos_type lastMatchedPos = -1;

  while (input.get(currentChar)) {
    currentToken += currentChar;
    std::unordered_set<std::shared_ptr<State>> nextStateSet
      = currentState->getNextState(currentChar);
    currentState = nextStateSet.empty() ? nullptr : *nextStateSet.begin();

    if (currentState) {
      Token token = currentState->getAcceptedToken();
      if (!token.getType().empty()) {
        token.setLexeme(currentToken);
        lastMatchedToken = token;
        lastMatchedPos = input.tellg();
      }
    } else if (!lastMatchedToken.getType().empty()) {
      if (lastMatchedToken.getType() != "=ws=") {
        std::cerr << "Matched '" << lastMatchedToken.getLexeme()
                  << "' for '" << lastMatchedToken.getType() << "'"
                  << std::endl;
        output << lastMatchedToken.getType() << std::endl;
      }
      anchor = lastMatchedPos;
      input.seekg(anchor);
      currentToken.clear();
      lastMatchedToken = Token("", 0);
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
  if (!lastMatchedToken.getType().empty()
      && lastMatchedToken.getType() != "=ws=") {
    std::cerr << "Matched '" << lastMatchedToken.getLexeme()
              << "' for '" << lastMatchedToken.getType() << "'"
              << std::endl;
    output << lastMatchedToken.getType() << std::endl;
  }
}

void LexicalAnalyzer::saveLexicalAnalyzerAutomaton(std::ostream& outputStream) {
    languageAutomaton.saveIntoFile(outputStream);
};
