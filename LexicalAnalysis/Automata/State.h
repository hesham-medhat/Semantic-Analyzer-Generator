#ifndef OUR_JAVA_COMPILER_NODE_H
#define OUR_JAVA_COMPILER_NODE_H


#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <vector>

#include "../Token.h"

class State {
public:
    State();
    State(Token& acceptedToken);

    void setAcceptedToken(Token& acceptedToken);
    Token getAcceptedToken();
    std::unordered_set<std::shared_ptr<State>> getNextState(char inputChar);

    virtual void addTransition(char inputChar, std::shared_ptr<State> nextState) = 0;
    virtual std::vector<std::pair<char, std::unordered_set<std::shared_ptr<State>>>> viewTransitions() = 0;
protected:
    const Token acceptedToken;
};


#endif
