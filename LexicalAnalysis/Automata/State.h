#ifndef OUR_JAVA_COMPILER_NODE_H
#define OUR_JAVA_COMPILER_NODE_H


#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <vector>
#include <climits>

#include "../Token.h"

class State {
public:
    State();
    explicit State(Token& acceptedToken);

    Token getAcceptedToken();

    virtual std::unordered_set<std::shared_ptr<State>> getNextState(char inputChar) = 0;
    virtual void addTransition(char inputChar, std::shared_ptr<State> nextState) = 0;
    virtual std::vector<std::pair<char, std::unordered_set<std::shared_ptr<State>>>> viewTransitions() = 0;

    // Traversal recursive DFS that explores states and adds them to collection incrementing counter
    virtual void explore(std::unordered_map<std::shared_ptr<State>, int>& collection, int* counter) = 0;
protected:
    Token acceptedToken;
};


#endif
