#include "State.h"

State::State()
: acceptedToken("", INT_MAX) { }

State::State(Token &acceptedToken)
: acceptedToken(acceptedToken) { }


Token State::getAcceptedToken() { return acceptedToken; }
