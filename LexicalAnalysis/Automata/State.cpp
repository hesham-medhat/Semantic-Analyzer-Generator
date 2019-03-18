#include "State.h"

State::State()
: acceptedToken(*(new std::string)) { }

Token State::getAcceptedToken() { return acceptedToken; }
