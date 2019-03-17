#ifndef OUR_JAVA_COMPILER_NFASTATE_H
#define OUR_JAVA_COMPILER_NFASTATE_H


#include <unordered_map>
#include <memory>
#include "State.h"

class NFAState : public State {
public:
    NFAState();
    NFAState(Token& acceptedToken);

    void addTransition(char inputChar, std::shared_ptr<State> nextState);
private:
    std::unordered_map<char, std::unordered_set<std::shared_ptr<State>>> transitions;
};


#endif
