#include "DFAState.h"

DFAState::DFAState() { }

DFAState::DFAState(Token& acceptedToken) : State(acceptedToken) { }

void DFAState::addTransition(char inputChar, std::shared_ptr<State> nextState) {
    transitions[inputChar] = nextState;
}

std::vector<std::pair<char, std::unordered_set<std::shared_ptr<State>>>> DFAState::viewTransitions() {
    std::vector<std::pair<char, std::unordered_set<std::shared_ptr<State>>>> view(transitions.size());
    for(auto transition : transitions) {
        std::pair<char, std::unordered_set<std::shared_ptr<State>>> pair;
        pair.first = transition.first;
        pair.second.insert(transition.second);
        view.push_back(pair);
    }
    return view;
}

