#include "TerminalSymbol.h"

TerminalSymbol::TerminalSymbol(std::string name) : GrammarSymbol(name) {}


GrammarSymbol::Type TerminalSymbol::getType() {
    return GrammarSymbol::Type::Terminal;
}
