#ifndef OUR_JAVA_COMPILER_AUTOMATON_H
#define OUR_JAVA_COMPILER_AUTOMATON_H

#include "State.h"
#include <istream>
#include <memory>

class Automaton {
public:
  // Loads automaton from saved representation through given stream
  explicit Automaton(std::istream &savedRepresentationStream);
  // Constructor for automaton of a transition
  explicit Automaton(char character);
  // Constructor for automaton of a character class
  Automaton(char first, char last);
  Automaton();

  // Automata operations passed other automata and acceptance token for final
  // state
  void unionOp(Automaton, Token &);
  void concatenateOp(Automaton, Token &);
  void kleeneClosureOp(Token &);
  void positiveClosureOp(Token &);

  // Saves automaton according to specification in Automata Saved Representation
  // Foramt.txt
  void saveIntoFile(std::ostream &stream);

  std::shared_ptr<State> startState;
  std::shared_ptr<State> finalState;

private:
  // Loads automaton according to specification in Automata Saved Representation
  // Foramt.txt
  void loadFromFile(std::istream &stream);

  // Gets all the states in the automaton in DFS manner
  std::unordered_map<std::shared_ptr<State>, int> getAllStates();
};

#endif
