#include "TerminalSymbol.h"

TerminalSymbol::TerminalSymbol(std::string name) : GrammarSymbol
(std::move(name)) {}


GrammarSymbol::Type TerminalSymbol::getType() {
    return GrammarSymbol::Type::Terminal;
}
