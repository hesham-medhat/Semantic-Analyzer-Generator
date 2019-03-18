#ifndef OUR_JAVA_COMPILER_STATEBUILDER_H
#define OUR_JAVA_COMPILER_STATEBUILDER_H


#include "State.h"

class StateBuilder {
public:
    static std::shared_ptr<State> buildState(std::string& stateType, std::string tokenType);
private:
    // Prevent instantiating as it is unnecessary
    StateBuilder();
};


#endif
