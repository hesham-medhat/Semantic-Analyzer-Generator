#ifndef OUR_JAVA_COMPILER_AUTOMATON_H
#define OUR_JAVA_COMPILER_AUTOMATON_H


#include <istream>
#include "State.h"

class Automaton {
    Automaton(std::istream savedRepresentation);
    Automaton(char character);
    Automaton(char first, char last);

    Automaton unionOp(Automaton);
    Automaton concatenateOp(Automaton);
    Automaton kleeneClosureOp();
    Automaton positiveClosureOp();

    void saveIntoFile();
private:
    State& startState;
    State& finalState;
};


#endif
