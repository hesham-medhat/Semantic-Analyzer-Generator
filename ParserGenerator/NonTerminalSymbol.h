#ifndef PARSER_GENERATOR_NONTERMINAL_H
#define PARSER_GENERATOR_NONTERMINAL_H


#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <vector>
#include <iostream>
#include "TerminalSymbol.h"
#include "GrammarSymbol.h"

class NonTerminalSymbol : public GrammarSymbol {
public:
    typedef std::shared_ptr<NonTerminalSymbol> ptr;
    typedef std::pair<NonTerminalSymbol::ptr,GrammarSymbol::Production> usingPair;
    explicit NonTerminalSymbol(std::string name);

    std::unordered_set<TerminalSymbol::ptr> getFirst(std::unordered_set<std::string>);

    std::unordered_set<TerminalSymbol::ptr> getFollow(std::unordered_set<std::string>);

    Type getType() override;

    // Assumes epsilon production to be of single Symbol whose name is an
    // empty string
    void addProduction(GrammarSymbol::Production);
    void addTransition(TerminalSymbol::ptr,
            std::shared_ptr<GrammarSymbol::Production>);
    std::shared_ptr<Production> getProduction(TerminalSymbol::ptr);
    void addUsingProduction(NonTerminalSymbol::ptr, GrammarSymbol::Production);

    // Saves productions according to the Parsers Saved Representation Format
    void saveProductions(std::ostream&, std::unordered_map<std::string,
            std::shared_ptr<TerminalSymbol>>& terminals,
            std::unordered_map<std::shared_ptr<Production>, int>&
                    productionIds);


    bool hasEpsilonProduction;
    bool followCalculated;
    bool firstCalculated;
    std::vector<GrammarSymbol::Production> productions;
private:

    std::unordered_map<TerminalSymbol::ptr,
            std::shared_ptr<GrammarSymbol::Production>> transitions;
    std::vector<NonTerminalSymbol::usingPair> usingProductions;
    std::unordered_set<TerminalSymbol::ptr> first;
    std::unordered_set<TerminalSymbol::ptr> follow;

};


#endif //PARSER_GENERATOR_NONTERMINAL_H
