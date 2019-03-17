#include "NFAState.h"

NFAState::NFAState() { }

NFAState::NFAState(Token& acceptedToken) { }

void NFAState::addTransition(char inputChar, std::shared_ptr<State> nextState) {
    throw "Not Implemented";
}