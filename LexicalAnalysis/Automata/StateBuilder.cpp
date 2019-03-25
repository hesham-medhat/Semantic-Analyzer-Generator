#include "StateBuilder.h"
#include "DFAState.h"
#include "NFAState.h"

std::shared_ptr<State> StateBuilder::buildState(std::string stateType, std::string tokenType) {
    Token token(tokenType, 0);
    std::shared_ptr<State> returner;

    // Instantiate state according to type
    if (stateType == "DFA") {
        returner = std::make_shared<DFAState>(token);
    } else if (stateType == "NFA") {
        returner = std::make_shared<NFAState>(token);
    } else {
        throw std::runtime_error("StateBuilder got bad stateType specification: " + stateType);
    }

    return returner;
}
