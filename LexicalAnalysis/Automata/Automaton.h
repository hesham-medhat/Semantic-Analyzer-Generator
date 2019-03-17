#ifndef OUR_JAVA_COMPILER_AUTOMATON_H
#define OUR_JAVA_COMPILER_AUTOMATON_H


#include <istream>
#include <memory>
#include "State.h"

class Automaton {
    Automaton(std::istream savedRepresentation);
    Automaton(char character);
    // Constructor for character class
    Automaton(char first, char last);

    // Automata operations passed other automata and acceptance token for final state
    void unionOp(Automaton, Token&);
    void concatenateOp(Automaton, Token&);
    void kleeneClosureOp(Token&);
    void positiveClosureOp(Token&);

    void saveIntoFile();

    std::shared_ptr<State> startState;
    std::shared_ptr<State> finalState;
};


#endif
