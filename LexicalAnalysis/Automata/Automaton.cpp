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
