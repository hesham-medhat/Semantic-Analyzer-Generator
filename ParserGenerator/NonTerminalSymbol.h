#ifndef PARSER_GENERATOR_NONTERMINAL_H
#define PARSER_GENERATOR_NONTERMINAL_H


#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <deque>
#include "TerminalSymbol.h"
#include "GrammarSymbol.h"

class NonTerminalSymbol : public GrammarSymbol {
public:
    explicit NonTerminalSymbol(std::string name);

    std::unordered_set<std::shared_ptr<TerminalSymbol>> getFirst();
    std::unordered_set<std::shared_ptr<TerminalSymbol>> getFollow();

    void addProduction(std::shared_ptr<TerminalSymbol>,
            GrammarSymbol::Production);

    bool hasEpsilonTransition;
private:
    // Flag used during building process of the parser to indicate DSs are built
    bool built;
    std::unordered_map<std::shared_ptr<TerminalSymbol>,
    GrammarSymbol::Production> productions;
};


#endif //PARSER_GENERATOR_NONTERMINAL_H
