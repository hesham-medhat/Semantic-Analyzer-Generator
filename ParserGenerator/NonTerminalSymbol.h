#ifndef PARSER_GENERATOR_NONTERMINAL_H
#define PARSER_GENERATOR_NONTERMINAL_H

#include "GrammarSymbol.h"
#include "TerminalSymbol.h"
#include <iostream>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class NonTerminalSymbol : public GrammarSymbol {
public:
  typedef std::shared_ptr<NonTerminalSymbol> ptr;
  typedef std::pair<NonTerminalSymbol::ptr, ProductionPtr> usingPair;
  explicit NonTerminalSymbol(std::string name);

  std::unordered_set<TerminalSymbol::ptr>
      getFirst(std::unordered_set<std::string>);

  std::unordered_set<TerminalSymbol::ptr>
      getFollow(std::unordered_set<std::string>);

  Type getType() override;

  // Assumes epsilon production to be of single Symbol whose name is an
  // empty string
  void addProduction(ProductionPtr);
  void addTransition(TerminalSymbol::ptr, ProductionPtr);
  ProductionPtr getProduction(TerminalSymbol::ptr);
  void addUsingProduction(NonTerminalSymbol::ptr, ProductionPtr);

  // Saves productions according to the Parsers Saved Representation Format
  void saveProductions(
      std::ostream &,
      std::unordered_map<std::string, std::shared_ptr<TerminalSymbol>>
          &terminals,
      std::unordered_map<ProductionPtr, int> &productionIds);
  TerminalSymbol::ptr epsilon;

  bool hasEpsilonProduction;
  bool followCalculated;
  bool firstCalculated;
  std::vector<ProductionPtr> productions;

private:
  std::unordered_map<TerminalSymbol::ptr, ProductionPtr> transitions;
  std::vector<NonTerminalSymbol::usingPair> usingProductions;
  std::unordered_set<TerminalSymbol::ptr> first;
  std::unordered_set<TerminalSymbol::ptr> follow;
};

#endif // PARSER_GENERATOR_NONTERMINAL_H
