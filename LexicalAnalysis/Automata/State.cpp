#include "State.h"

State::State()
: acceptedToken("") { }

State::State(Token &acceptedToken)
: acceptedToken(acceptedToken) { }


Token State::getAcceptedToken() { return acceptedToken; }
