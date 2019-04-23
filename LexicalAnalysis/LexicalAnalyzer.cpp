#include <iostream>
#include <fstream>
#include "LexicalAnalyzer.h"

LexicalAnalyzer::LexicalAnalyzer(const LexicalAnalyzer& original) :
  languageAutomaton(original.languageAutomaton),
  LACurrentState(original.LACurrentState) { }

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

void LexicalAnalyzer::initProgramParse(const std::string& programFilePath) {
  programIstream = std::make_unique<std::ifstream>(
      programFilePath,
      std::ios_base::in | std::ios_base::binary);
  if (*programIstream) {
    LACurrentState = languageAutomaton.startState;
  } else {
    programIstream = nullptr;
  }
}

Token LexicalAnalyzer::nextToken() {
  if (!*programIstream) { return Token("", INT_MAX); }

  Token lastMatchedToken("", INT_MAX);
  char currentChar;
  std::string currentToken;
  std::ifstream::pos_type anchor = programIstream->tellg();

  while (programIstream->get(currentChar)) {
    currentToken += currentChar;
    std::unordered_set<std::shared_ptr<State>> nextStateSet
      = LACurrentState->getNextState(currentChar);
    LACurrentState = nextStateSet.empty() ? nullptr : *nextStateSet.begin();

    if (LACurrentState) {
      Token token = LACurrentState->getAcceptedToken();
      if (!token.getType().empty()) {
        token.setLexeme(currentToken);
        lastMatchedToken = token;
        anchor = programIstream->tellg();
      }
    } else if (!lastMatchedToken.getType().empty()) {
      programIstream->seekg(anchor);
      LACurrentState = languageAutomaton.startState;
      if (lastMatchedToken.getType() != "=ws=") {
        return lastMatchedToken;
      }
      currentToken.clear();
      lastMatchedToken = Token("", 0);
    } else {
      std::cerr << "[ERROR] Unrecognized token: " << currentToken << std::endl;
      // Discard one prefix character and restart scanning
      anchor += 1;
      programIstream->seekg(anchor);
      currentToken.clear();
      LACurrentState = languageAutomaton.startState;
    }
  }
  if (lastMatchedToken.getType() != "=ws=") {
    return lastMatchedToken;
  }
  return Token("", INT_MAX);
}

void LexicalAnalyzer::saveLexicalAnalyzerAutomaton(std::ostream& outputStream) {
    languageAutomaton.saveIntoFile(outputStream);
};

