#include <ostream>
#include "NonTerminalSymbol.h"

NonTerminalSymbol::NonTerminalSymbol(std::string name) : GrammarSymbol
(std::move(name)) {}


GrammarSymbol::Type NonTerminalSymbol::getType() {
    return GrammarSymbol::Type::NonTerminal;
}

void NonTerminalSymbol::addProduction(std::shared_ptr<TerminalSymbol> input,
                                      GrammarSymbol::Production newProduction) {
    /* Update hasEpsilonProduction if epsilon */
    if (newProduction.size() == 1) {
        std::shared_ptr<GrammarSymbol> singleTerminal = *newProduction.begin();
        std::string name = singleTerminal->getName();
        if (name.empty()) {// Epsilon transition
            hasEpsilonProduction = true;
        }
    }

    productions[input] = newProduction;// Add production to map
}

void NonTerminalSymbol::saveProductions(std::ostream &out,
        std::unordered_map<std::string, std::shared_ptr<TerminalSymbol>>&
        terminals) {
    for (const auto& terminalEntry : terminals) {
        const std::shared_ptr<TerminalSymbol> terminal = terminalEntry.second;
        if (productions.find(terminal) != productions.end()) {
            Production& production = productions[terminal];
            if (production.size() == 1 &&
            (*production.begin())->getName().empty()) {// Epsilon production
                out << static_cast<std::string>("$$");
            } else {
                bool firstSymbol = true;
                for (const auto& grammarSymbol : production) {
                    if (!firstSymbol) out.put('$');
                    out << grammarSymbol->getName();
                    firstSymbol = false;
                }
            }
        } else {// Error production. Terminal symbol not mapped
            out << static_cast<std::string>("$$$");
        }
    }
    out << std::endl;
}
