#include "DFAState.h"

void DFAState::addTransition(char inputChar, std::shared_ptr<State> nextState) {
    throw "Not Implemented";
}

std::vector<std::pair<char, std::unordered_set<std::shared_ptr<State>>>> DFAState::viewTransitions() {
    throw "Not Implemented";
}
