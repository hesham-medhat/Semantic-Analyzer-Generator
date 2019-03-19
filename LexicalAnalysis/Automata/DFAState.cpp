#include "DFAState.h"

DFAState::DFAState() { }

DFAState::DFAState(Token& acceptedToken) : State(acceptedToken) { }


std::unordered_set<std::shared_ptr<State>> DFAState::getNextState(char inputChar) {
    std::unordered_set<std::shared_ptr<State>> resultSet;
    resultSet.insert(transitions[inputChar]);
    return resultSet;
}

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


void DFAState::explore(std::unordered_map<std::shared_ptr<State>, int> &collection, int* counter) {
    for (auto transition : transitions) {
        if (collection.find(transition.second) == collection.end()) {
            collection[transition.second] = (*counter)++;
            transition.second->explore(collection, counter);
        }
    }
}
