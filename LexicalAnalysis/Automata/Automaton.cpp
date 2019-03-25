#include <sstream>
#include "Automaton.h"
#include "NFAState.h"
#include "StateBuilder.h"

Automaton::Automaton(std::istream& savedRepresentationStream) {
    if (savedRepresentationStream.bad()) throw std::runtime_error("Automaton constructor passed bad stream");
    loadFromFile(savedRepresentationStream);
}

Automaton::Automaton(char character) {
    startState = std::make_shared<NFAState>();
    finalState = std::make_shared<NFAState>();

    startState->addTransition(character, finalState);
}

Automaton::Automaton(char first, char last) {
    startState = std::make_shared<NFAState>();
    finalState = std::make_shared<NFAState>();

    if (last < first) throw new std::invalid_argument("Automaton: Bad character class input");

    char i = first;

    do {
        startState->addTransition(i, finalState);
    } while (++i < last);
}
Automaton::Automaton() {}

void Automaton::unionOp(Automaton other, Token& acceptedToken) {
    std::shared_ptr<NFAState> newStart = std::make_shared<NFAState>();
    std::shared_ptr<NFAState> newFinal = std::make_shared<NFAState>(acceptedToken);

    newStart->addTransition(0, startState);
    newStart->addTransition(0, other.startState);

    finalState->addTransition(0, newFinal);
    other.finalState->addTransition(0, newFinal);

    startState = newStart;
    finalState = newFinal;
}

void Automaton::concatenateOp(Automaton other, Token& acceptedToken) {
    std::shared_ptr<NFAState> newFinal = std::make_shared<NFAState>(acceptedToken);

    finalState->addTransition(0, other.startState);
    other.finalState->addTransition(0, newFinal);

    //other.finalState = newFinal;
    finalState = newFinal;
}

void Automaton::kleeneClosureOp(Token& acceptedToken) {
    std::shared_ptr<NFAState> newStart = std::make_shared<NFAState>();
    std::shared_ptr<NFAState> newFinal = std::make_shared<NFAState>(acceptedToken);

    newStart->addTransition(0, newFinal);
    newStart->addTransition(0, startState);
    newFinal->addTransition(0, newStart);

    startState = newStart;
    finalState = newFinal;
}

void Automaton::positiveClosureOp(Token& acceptedToken) {
    std::shared_ptr<NFAState> newStart = std::make_shared<NFAState>();
    std::shared_ptr<NFAState> newFinal = std::make_shared<NFAState>(acceptedToken);

    newStart->addTransition(0, startState);
    newFinal->addTransition(0, newStart);

    startState = newStart;
    finalState = newFinal;
}


void Automaton::saveIntoFile(std::ostream& stream) {
    if (stream.bad()) throw std::runtime_error("Automaton::saveIntoFile passed bad stream");

    if (dynamic_cast<NFAState*>(startState.get()) == nullptr) {
        stream << "DFA" << std::endl;
    } else {
        stream << "NFA" << std::endl;
    }

    std::unordered_map<std::shared_ptr<State>, int> states = getAllStates();
    stream << states.size() << std::endl;

    std::shared_ptr<State> statesArray[states.size()];
    // Populate the states array in order
    for (auto i = states.begin(); i != states.end(); i++) {
        statesArray[i->second] = i->first;
    }

    for (int i = 0; i < states.size(); i++) {
        stream << statesArray[i]->getAcceptedToken().getType() << std::endl;
    }

    for (auto i = states.begin(); i != states.end(); i++) {
      if (i->first)
        for (auto transInput : i->first->viewTransitions()) {
            for (auto destination : transInput.second)
                stream << states[i->first] << "  " << states[destination] << "  " << transInput.first << std::endl;
        }
    }
}

void Automaton::loadFromFile(std::istream& stream) {
    std::string stateType;
    size_t totalStates;
    stream >> stateType;
    stream >> totalStates;

    std::shared_ptr<State>* states = (std::shared_ptr<State>*) malloc(sizeof(std::shared_ptr<State>) * totalStates);

    std::string buffer;
    for(int i = 0; i < totalStates; i++) {
        stream >> buffer;
        states[i] = StateBuilder::buildState(stateType, buffer);
    }

    int source, destination;
    char transitionCharacter;
    while(!stream.eof()) {
        stream >> buffer;
        std::stringstream lineParser(buffer);
        lineParser >> source >> destination >> transitionCharacter;

        states[source]->addTransition(transitionCharacter, states[destination]);
    }
    startState = states[0];
    finalState = states[1];
}

std::unordered_map<std::shared_ptr<State>, int> Automaton::getAllStates() {
    std::unordered_map<std::shared_ptr<State>, int> collection;
    int counter = 0;
    collection[startState] = counter++;

    startState->explore(collection, &counter);

    return collection;
}
