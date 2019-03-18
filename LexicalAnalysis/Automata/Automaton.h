#ifndef OUR_JAVA_COMPILER_AUTOMATON_H
#define OUR_JAVA_COMPILER_AUTOMATON_H


#include <istream>
#include <memory>
#include "State.h"

class Automaton {
public:
    Automaton(std::istream savedRepresentationStream);
    Automaton(char character);
    // Constructor for character class
    Automaton(char first, char last);

    // Automata operations passed other automata and acceptance token for final state
    void unionOp(Automaton, Token&);
    void concatenateOp(Automaton, Token&);
    void kleeneClosureOp(Token&);
    void positiveClosureOp(Token&);

    void saveIntoFile(std::ostream stream);

    std::shared_ptr<State> startState;
    std::shared_ptr<State> finalState;
private:
    // Gets all the states in the automaton in DFS manner
    std::unordered_map<std::shared_ptr<State>, int> getAllStates();
};


#endif
