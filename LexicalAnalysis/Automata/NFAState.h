#ifndef OUR_JAVA_COMPILER_NFASTATE_H
#define OUR_JAVA_COMPILER_NFASTATE_H


#include "State.h"

class NFAState : public State {
public:
    NFAState();
    NFAState(Token& acceptedToken);

    std::unordered_set<std::shared_ptr<State>> getNextState(char inputChar);
    void addTransition(char inputChar, std::shared_ptr<State> nextState);
    std::vector<std::pair<char, std::unordered_set<std::shared_ptr<State>>>> viewTransitions();

    void explore(std::unordered_map<std::shared_ptr<State>, int>& collection, int* counter);
private:
    std::unordered_map<char, std::unordered_set<std::shared_ptr<State>>> transitions;
};


#endif
