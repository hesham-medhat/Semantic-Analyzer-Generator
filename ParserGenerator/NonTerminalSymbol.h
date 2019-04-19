#ifndef PARSER_GENERATOR_NONTERMINAL_H
#define PARSER_GENERATOR_NONTERMINAL_H


#include <unordered_map>
#include <unordered_set>
#include <memory>
#include "TerminalSymbol.h"
#include "GrammarSymbol.h"

class NonTerminalSymbol : public GrammarSymbol {
public:
    explicit NonTerminalSymbol(std::string name);

    std::unordered_set<std::shared_ptr<TerminalSymbol>> getFirst();
    std::unordered_set<std::shared_ptr<TerminalSymbol>> getFollow();

    Type getType() override;
    
    // Assumes epsilon production to be of single Symbol whose name is empty
    void addProduction(std::shared_ptr<TerminalSymbol>,
            GrammarSymbol::Production);

    // Saves productions according to the Parsers Saved Representation Format
    void saveProductions(std::ostream&, std::unordered_map<std::string,
            std::shared_ptr<TerminalSymbol>>& terminals);

    bool hasEpsilonProduction;
private:
    // Flag used during building process of the parser to indicate DSs are built
    bool built;
    std::unordered_map<std::shared_ptr<TerminalSymbol>,
    GrammarSymbol::Production> productions;
};


#endif //PARSER_GENERATOR_NONTERMINAL_H
