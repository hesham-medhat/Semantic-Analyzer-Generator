#include "State.h"

State::State()
: acceptedToken(*(new std::string)) { }

State::State(Token &acceptedToken)
: acceptedToken(acceptedToken) { }


Token State::getAcceptedToken() { return acceptedToken; }
