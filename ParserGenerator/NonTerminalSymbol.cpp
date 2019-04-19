#include "NonTerminalSymbol.h"

NonTerminalSymbol::NonTerminalSymbol(std::string name) : GrammarSymbol
(std::move(name)) {}


GrammarSymbol::Type NonTerminalSymbol::getType() {
    return GrammarSymbol::Type::NonTerminal;
}

/*
void NonTerminalSymbol::addProduction(TerminalSymbol::ptr input,
                                      GrammarSymbol::Production newProduction) {
    */
/* Update hasEpsilonProduction if epsilon *//*

    if (newProduction.size() == 1) {
        std::shared_ptr<GrammarSymbol> singleTerminal = *newProduction.begin();
        std::string name = singleTerminal->getName();
        if (name.empty()) {// Epsilon transition
            hasEpsilonProduction = true;
        }
    }

    productions[input] = newProduction;// Add production to map
}
*/

/*
std::unordered_set<TerminalSymbol::ptr> NonTerminalSymbol::getFirst() {
    std::unordered_set<TerminalSymbol::ptr> first;
    std::vector<GrammarSymbol::Production>::iterator prodIte;
    for(prodIte = productions.begin(); prodIte != productions.end(); prodIte++){
        GrammarSymbol::Production production = *prodIte;
        GrammarSymbol::Production::iterator symIte;
        for(symIte = production.begin(); symIte != production.end(); symIte++){
            GrammarSymbol::ptr symbol = *symIte;
            if(symbol->getType() == Terminal){
                //first.insert(symbol);
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



    //for (auto &production: productions) {

    //}
}
*/





/*
std::deque<int> mydeque;

// set some initial values:
for (int i=1; i<6; i++) mydeque.push_back(i); // 1 2 3 4 5

std::deque<int>::iterator it = mydeque.begin();
++it;

it = mydeque.insert (it,10);                  // 1 10 2 3 4 5
// "it" now points to the newly inserted 10

mydeque.insert (it,2,20);                     // 1 20 20 10 2 3 4 5
// "it" no longer valid!

it = mydeque.begin()+2;

std::vector<int> myvector (2,30);
mydeque.insert (it,myvector.begin(),myvector.end());
// 1 20 30 30 20 10 2 3 4 5

std::cout << "mydeque contains:";
for (it=mydeque.begin(); it!=mydeque.end(); ++it)
std::cout << ' ' << *it;
std::cout << '\n';

return 0;
}


Edit & Run

Output:

mydeque contains: 1 20 30 30 20 10 2 3 4 5
*/
