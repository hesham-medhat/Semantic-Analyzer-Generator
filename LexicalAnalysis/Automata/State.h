#ifndef OUR_JAVA_COMPILER_NODE_H
#define OUR_JAVA_COMPILER_NODE_H


#include <unordered_map>
#include <set>
#include <unordered_set>

#include "../Token.h"

class State {
    State();
    State(Token& acceptedToken);

    void setAcceptedToken(Token& acceptedToken);
    Token getAcceptedToken();
    std::unordered_set<State&> getNextState(char inputChar);

    virtual void addTransition(char inputChar, State& nextState) = 0;

protected:
    const Token acceptedToken;
};


#endif
