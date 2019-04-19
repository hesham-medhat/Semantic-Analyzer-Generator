#include "GrammarSymbol.h"

GrammarSymbol::GrammarSymbol(std::string name)
: name(std::move(name)) {}


std::string GrammarSymbol::getName() {
    return name;
}
