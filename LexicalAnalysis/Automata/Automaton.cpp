#include "Automaton.h"
#include "NFAState.h"


Automaton::Automaton(char character) {
    startState = std::make_shared<NFAState>();
    finalState = std::make_shared<NFAState>();

    startState->addTransition(character, finalState);
}

Automaton::Automaton(char first, char last) {
    startState = std::make_shared<NFAState>();
    finalState = std::make_shared<NFAState>();

    if (last < first) throw new std::invalid_argument("AUTOMATON: Bad character class input");

    char i = first;

    do {
        startState->addTransition(i, finalState);
    } while (++i < last);
}


void Automaton::unionOp(Automaton other, Token& acceptedToken) {
    std::shared_ptr<NFAState> newStart = std::make_shared<NFAState>();
    std::shared_ptr<NFAState> newFinal = std::make_shared<NFAState>(acceptedToken);

    newStart->addTransition(0, startState);
    newStart->addTransition(0, other.startState);

    finalState->addTransition(0, newFinal);
    other.finalState->addTransition(0, newFinal);

    finalState = newFinal;
}

void Automaton::concatenateOp(Automaton other, Token& acceptedToken) {
    std::shared_ptr<NFAState> newFinal = std::make_shared<NFAState>(acceptedToken);

    finalState->addTransition(0, other.startState);
    other.finalState->addTransition(0, newFinal);

    other.finalState = newFinal;
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


void Automaton::saveIntoFile(std::ostream stream) {
    if (stream.bad()) throw std::runtime_error("Automaton::saveIntoFile passed bad stream");

    if (dynamic_cast<NFAState*>(startState.get()) == nullptr) {
        stream << "DFA" << std::endl;
    } else {
        stream << "NFA" << std::endl;
    }

    std::unordered_map<std::shared_ptr<State>, int> states = getAllStates();
    stream << states.size() << std::endl;

    for (auto i = states.begin(); i != states.end(); i++) {
        stream << i->first->getAcceptedToken().getType() << std::endl;
    }

    for (auto i = states.begin(); i != states.end(); i++) {
        for (auto transInput : i->first->viewTransitions()) {
            for (auto destination : transInput.second)
                stream << states[i->first] << states[destination] << transInput.first << std::endl;
        }
    }
}

std::unordered_map<std::shared_ptr<State>, int> Automaton::getAllStates() {
    throw "Not Implemented";
}
