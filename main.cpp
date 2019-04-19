#include <iostream>
#include <string>
#include <fstream>
#include "ParserGenerator/GrammarSymbol.h"
#include "ParserGenerator/TerminalSymbol.h"
#include "ParserGenerator/NonTerminalSymbol.h"
#include "LexicalAnalysis/LexicalAnalyzerGenerator.h"
#include "LexicalAnalysis/LexicalAnalyzer.h"


std::unordered_map<TerminalSymbol::ptr,
        GrammarSymbol::Production> transitions;
std::vector<GrammarSymbol::Production> productions;


/*
std::unordered_set<TerminalSymbol::ptr> getFirst() {
  std::unordered_set<TerminalSymbol::ptr> first;
  std::vector<GrammarSymbol::Production>::iterator prodIte;
  for(prodIte = productions.begin(); prodIte != productions.end(); prodIte++){
    GrammarSymbol::Production production = *prodIte;
    GrammarSymbol::Production::iterator symIte;
    for(symIte = production.begin(); symIte != production.end(); symIte++){
      GrammarSymbol symbol = **symIte;
//            cout<< typeid(symbol).name();
      if(symbol.getType() == GrammarSymbol::Terminal){
        shared_ptr<GrammarSymbol> sp= std::make_shared<GrammarSymbol>(symbol);
        //shared_ptr<TerminalSymbol> terminal = dynamic_cast<TerminalSymbol::ptr>(sp);
        //first.insert(make_shared<TerminalSymbol>(symbol));
//                transitions[std::make_shared(symbol)] = production;
        break;
      } else {
//                std::unordered_set<TerminalSymbol::ptr> secFirst =
//                (dynamic_cast<NonTerminalSymbol::ptr>(std::make_shared(symbol)))->getFirst();
        std::unordered_set<TerminalSymbol::ptr>::iterator firstIte;
*/
/*
                for (firstIte = secFirst.begin();firstIte != secFirst.end() ; firstIte++) {
                    TerminalSymbol terminal = **firstIte;
                    if(terminal.getName().compare(0) != 0){

                    }
                }
*//*

      }
    }
  }

  return first;

  //for (auto &production: productions) {

  //}
}
*/


std::unordered_set<TerminalSymbol::ptr> getFollow(){

}

void addProduction(GrammarSymbol::Production production){
  productions.push_back(production);
}



int main(int argc, char* argv[]) {


  std::deque<std::shared_ptr<GrammarSymbol>> production;
  GrammarSymbol x = TerminalSymbol("1");
  NonTerminalSymbol x1 = NonTerminalSymbol("2");
  TerminalSymbol x2 = TerminalSymbol("3");
  TerminalSymbol x3 = TerminalSymbol("4");

  production.push_back(make_shared<GrammarSymbol>(x));
/*

  production.push_back(make_shared<GrammarSymbol>(x));
  production.push_back(make_shared<GrammarSymbol>(x1));
  production.push_back(make_shared<GrammarSymbol>(x2));
  production.push_back(make_shared<GrammarSymbol>(x3));
*/

//  addProduction(production);
//  getFirst();
  //std::deque<std::shared_ptr<GrammarSymbol>>::iterator it = production.begin();
  std::cout<<x2.getName().compare("");




  return 0;
}