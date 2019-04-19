#ifndef PARSER_GENERATOR_NONTERMINAL_H
#define PARSER_GENERATOR_NONTERMINAL_H


#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <vector>
#include "TerminalSymbol.h"
#include "GrammarSymbol.h"

class NonTerminalSymbol : public GrammarSymbol {
public:
    typedef std::shared_ptr<NonTerminalSymbol> ptr;
    explicit NonTerminalSymbol(std::string name);

    std::unordered_set<TerminalSymbol::ptr> getFirst();
    std::unordered_set<TerminalSymbol::ptr> getFollow();

    Type getType() override;
    
    // Assumes epsilon production to be of single Symbol whose name is an
    // empty string
    void addProduction(GrammarSymbol::Production);

    bool hasEpsilonProduction;
private:
    // Flag used during building process of the parser to indicate DSs are built
    bool built;
    std::vector<GrammarSymbol::Production> productions;
    std::unordered_map<TerminalSymbol::ptr,
            GrammarSymbol::Production> transitions;
};


#endif //PARSER_GENERATOR_NONTERMINAL_H
