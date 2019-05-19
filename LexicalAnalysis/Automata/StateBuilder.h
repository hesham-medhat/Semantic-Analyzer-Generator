#ifndef OUR_JAVA_COMPILER_STATEBUILDER_H
#define OUR_JAVA_COMPILER_STATEBUILDER_H

#include "State.h"

// Utility builder class for states
class StateBuilder {
public:
  static std::shared_ptr<State> buildState(std::string stateType,
                                           std::string tokenType, int priority);

private:
  // Prevent instantiating as it is unnecessary. Intentionally not implemented
  StateBuilder();
};

#endif
