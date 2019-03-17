#ifndef OUR_JAVA_COMPILER_DFASTATE_H
#define OUR_JAVA_COMPILER_DFASTATE_H


#include "State.h"

class DFAState : public State{

private:
    std::unordered_map<char, State&> transitions;
};


#endif
