#ifndef PARSER_GENERATOR_NONTERMINAL_H
#define PARSER_GENERATOR_NONTERMINAL_H


#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <vector>
#include "TerminalSymbol.h"
#include "GrammarSymbol.h"
#include <iostream>

class NonTerminalSymbol : public GrammarSymbol {
public:
    typedef std::shared_ptr<NonTerminalSymbol> ptr;
    typedef std::pair<NonTerminalSymbol::ptr,GrammarSymbol::ptr> usingPair;
    explicit NonTerminalSymbol(std::string name);

    std::unordered_set<TerminalSymbol::ptr> getFirst();
    std::unordered_set<TerminalSymbol::ptr> getFollow();

    Type getType() override;
    
    // Assumes epsilon production to be of single Symbol whose name is an
    // empty string
    void addProduction(GrammarSymbol::Production);
    void addTransition(TerminalSymbol::ptr,
            GrammarSymbol::Production);
    GrammarSymbol::Production getProduction(TerminalSymbol::ptr);
    void addUsingProduction(NonTerminalSymbol::ptr, GrammarSymbol::ptr);

    bool hasEpsilonProduction;
private:

    bool firstCalculated;
    bool followCalculated;
    std::vector<GrammarSymbol::Production> productions;
    std::unordered_map<TerminalSymbol::ptr,
            GrammarSymbol::Production> transitions;
    std::vector<NonTerminalSymbol::usingPair> usingProductions;
    std::unordered_set<TerminalSymbol::ptr> first;
    std::unordered_set<TerminalSymbol::ptr> follow;

};


#endif //PARSER_GENERATOR_NONTERMINAL_H
