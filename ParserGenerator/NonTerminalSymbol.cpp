#include "NonTerminalSymbol.h"

void NonTerminalSymbol::addProduction(std::shared_ptr<TerminalSymbol> input,
                                      GrammarSymbol::Production newProduction) {
    /* Update hasEpsilonProduction if epsilon */
    if (newProduction.size() == 1) {
        std::shared_ptr<GrammarSymbol> singleTerminal = *newProduction.begin();
        std::string name = singleTerminal->getName();
        if (name == "") {// Epsilon transition
            hasEpsilonProduction = true;
        }
    }

    productions[input] = newProduction;// Add production to map
}
