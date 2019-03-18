#include "NFAState.h"

NFAState::NFAState() { }

NFAState::NFAState(Token& acceptedToken) { }

void NFAState::addTransition(char inputChar, std::shared_ptr<State> nextState) {
    throw "Not Implemented";
}

std::vector<std::pair<char, std::unordered_set<std::shared_ptr<State>>>> NFAState::viewTransitions() {
    throw "Not Implemented";
}
