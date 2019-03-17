#ifndef OUR_JAVA_COMPILER_DFASTATE_H
#define OUR_JAVA_COMPILER_DFASTATE_H


#include "State.h"

class DFAState : public State {
public:
    DFAState();
    DFAState(Token& acceptedToken);

    void addTransition(char inputChar, State& nextState);
private:
    std::unordered_map<char, State&> transitions;
};


#endif
