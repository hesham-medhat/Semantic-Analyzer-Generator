#include "NFAState.h"

NFAState::NFAState() { }

NFAState::NFAState(Token& acceptedToken) : State(acceptedToken) { }


std::unordered_set<std::shared_ptr<State>> NFAState::getNextState(char inputChar) {
    std::unordered_set<std::shared_ptr<State>> resultSet(transitions[inputChar]);
    return resultSet;
}

void NFAState::addTransition(char inputChar, std::shared_ptr<State> nextState) {
    transitions[inputChar].insert(nextState);
}

std::vector<std::pair<char, std::unordered_set<std::shared_ptr<State>>>> NFAState::viewTransitions() {
    std::vector<std::pair<char, std::unordered_set<std::shared_ptr<State>>>> view(transitions.size());
    for (auto transition : transitions) {
        std::pair<char, std::unordered_set<std::shared_ptr<State>>> pair;
        pair.first = transition.first;
        for (auto destination : transition.second) pair.second.insert(destination);
        view.push_back(pair);
    }
    return view;
}


void NFAState::explore(std::unordered_map<std::shared_ptr<State>, int> &collection, int* counter) {
    for (auto transition : transitions) {
        for (auto destination : transition.second) {
            if (collection.find(destination) == collection.end()) {
                collection[destination] = (*counter)++;
                destination->explore(collection, counter);
            }
        }
    }
}
