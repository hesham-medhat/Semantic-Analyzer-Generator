#ifndef OUR_JAVA_COMPILER_NFASTATE_H
#define OUR_JAVA_COMPILER_NFASTATE_H


#include "State.h"

class NFAState : public State {
public:
    NFAState();
    explicit NFAState(Token& acceptedToken);

    std::unordered_set<std::shared_ptr<State>> getNextState(char inputChar) override;
    void addTransition(char inputChar, std::shared_ptr<State> nextState) override;
    std::vector<std::pair<char, std::unordered_set<std::shared_ptr<State>>>> viewTransitions() override;

    void explore(std::unordered_map<std::shared_ptr<State>, int>& collection, int* counter) override;
private:
    std::unordered_map<char, std::unordered_set<std::shared_ptr<State>>> transitions;
};


#endif
