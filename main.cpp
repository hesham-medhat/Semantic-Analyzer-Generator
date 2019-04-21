#include <iostream>
#include <string>
#include <fstream>
#include "ParserGenerator/GrammarSymbol.h"
#include "ParserGenerator/TerminalSymbol.h"
#include "ParserGenerator/NonTerminalSymbol.h"
#include "LexicalAnalysis/LexicalAnalyzerGenerator.h"
#include "LexicalAnalysis/LexicalAnalyzer.h"


/*
std::unordered_map<TerminalSymbol::ptr,
        GrammarSymbol::Production> transitions;
std::vector<GrammarSymbol::Production> productions;
*/


/*
//return empty set if not LL1
std::unordered_set<TerminalSymbol::ptr> getFirst() {
    std::unordered_set<TerminalSymbol::ptr> first;
    std::vector<GrammarSymbol::Production>::iterator prodIte;
    for(prodIte = productions.begin(); prodIte != productions.end(); prodIte++){
        GrammarSymbol::Production production = *prodIte;
        GrammarSymbol::Production::iterator symIte;
        for(symIte = production.begin(); symIte != production.end(); symIte++){
            GrammarSymbol::ptr symbol = *symIte;
            if(symbol->getType() == GrammarSymbol::Type::Terminal){
                TerminalSymbol::ptr terminal = dynamic_pointer_cast<TerminalSymbol>(symbol);
                first.insert(terminal);
                if(terminal->getName().compare("") != 0){
                    if(transitions.find(terminal) == transitions.end()){
                        transitions[terminal] = production;
                    }else {
                        first.clear();
                        return first;
                    }
                }
                break;
            } else {
                NonTerminalSymbol::ptr nonTerminal = dynamic_pointer_cast<NonTerminalSymbol>(symbol);
//                NonTerminalSymbol::ptr nonTerminal = make_shared<NonTerminalSymbol>("o");
                std::unordered_set<TerminalSymbol::ptr> secFirst = nonTerminal->getFirst();
                std::unordered_set<TerminalSymbol::ptr>::iterator secFirstIte;
                for (secFirstIte = secFirst.begin(); secFirstIte != secFirst.end(); secFirstIte++) {
                    TerminalSymbol::ptr terminal = dynamic_pointer_cast<TerminalSymbol>(*secFirstIte);
                    if(terminal->getName().compare("") != 0){
                        first.insert(terminal);
                        if(transitions.find(terminal) == transitions.end()){
                            transitions[terminal] = production;
                        }else {
                            first.clear();
                            return first;
                        }
                    }else if(symIte + 1 == production.end()){
                        first.insert(terminal);
                    }
                }
                if(!nonTerminal->hasEpsilonProduction) {
                    break;
                }
            }
        }
    }
    return first;
}
*/


std::unordered_set<TerminalSymbol::ptr> getFollow(){

}
/*

void addProduction(GrammarSymbol::Production production){
  productions.push_back(production);
}
*/



int main(int argc, char* argv[]) {

    /*E -> TE’
    E’ -> +TE’ | 
    T -> FT’
    T’ -> *FT’ | 
    F -> (E) | id
    */

    GrammarSymbol::ptr E = std::make_shared<NonTerminalSymbol> ("E");
    GrammarSymbol::ptr E_ = std::make_shared<NonTerminalSymbol> ("E`");
    GrammarSymbol::ptr T = std::make_shared<NonTerminalSymbol> ("T");
    GrammarSymbol::ptr T_ = std::make_shared<NonTerminalSymbol> ("T`");
    GrammarSymbol::ptr F = std::make_shared<NonTerminalSymbol> ("F");

    GrammarSymbol::ptr plus = std::make_shared<TerminalSymbol> ("+");
    GrammarSymbol::ptr openPractice = std::make_shared<TerminalSymbol> ("(");
    GrammarSymbol::ptr closePractice = std::make_shared<TerminalSymbol> (")");
    GrammarSymbol::ptr x = std::make_shared<TerminalSymbol> ("*");
    GrammarSymbol::ptr id = std::make_shared<TerminalSymbol> ("id");
    GrammarSymbol::ptr epsilon = std::make_shared<TerminalSymbol> ("");
    GrammarSymbol::ptr epsilon2 = std::make_shared<TerminalSymbol> ("");


    std::deque<std::shared_ptr<GrammarSymbol>> production1;
    std::deque<std::shared_ptr<GrammarSymbol>> production2;
    std::deque<std::shared_ptr<GrammarSymbol>> production3;
    std::deque<std::shared_ptr<GrammarSymbol>> production4;
    std::deque<std::shared_ptr<GrammarSymbol>> production5;
    std::deque<std::shared_ptr<GrammarSymbol>> production6;
    std::deque<std::shared_ptr<GrammarSymbol>> production7;
    std::deque<std::shared_ptr<GrammarSymbol>> production8;

    production1.push_back(T);
    production1.push_back(E_);

    production2.push_back(plus);
    production2.push_back(T);
    production2.push_back(E_);

    production3.push_back(epsilon);


    production4.push_back(F);
    production4.push_back(T_);

    production5.push_back(x);
    production5.push_back(F);
    production5.push_back(T_);

    production6.push_back(epsilon2);

    production7.push_back(openPractice);
    production7.push_back(E);
    production7.push_back(closePractice);

    production8.push_back(id);

    NonTerminalSymbol::ptr En = std::dynamic_pointer_cast<NonTerminalSymbol>(E);
    NonTerminalSymbol::ptr E_n = std::dynamic_pointer_cast<NonTerminalSymbol>(E_);
    NonTerminalSymbol::ptr Tn = std::dynamic_pointer_cast<NonTerminalSymbol>(T);
    NonTerminalSymbol::ptr T_n = std::dynamic_pointer_cast<NonTerminalSymbol>(T_);
    NonTerminalSymbol::ptr Fn = std::dynamic_pointer_cast<NonTerminalSymbol>(F);

    En->addProduction(production1);
    E_n->addProduction(production2);
    E_n->addProduction(production3);
    Tn->addProduction(production4);
    T_n->addProduction(production5);
    T_n->addProduction(production6);
    Fn->addProduction(production7);
    Fn->addProduction(production8);

    En->hasEpsilonProduction = false;
    E_n->hasEpsilonProduction = true;
    Tn->hasEpsilonProduction = false;
    T_n->hasEpsilonProduction = true;
    Fn->hasEpsilonProduction = false;

    En->addUsingProduction(Fn, closePractice);

    E_n->addUsingProduction(En, epsilon);
    E_n->addUsingProduction(E_n, epsilon);

    Tn->addUsingProduction(En, E_);
    Tn->addUsingProduction(E_n, E_);

    T_n->addUsingProduction(Tn, epsilon);
    T_n->addUsingProduction(T_n, epsilon);

    Fn->addUsingProduction(Tn, T_);
    Fn->addUsingProduction(T_n, T_);


/*

    unordered_set<TerminalSymbol::ptr> follow = (std::dynamic_pointer_cast<NonTerminalSymbol>(E))->getFollow();
    unordered_set<TerminalSymbol::ptr>::iterator iter;
    for (iter = follow.begin(); iter != follow.end(); iter++) {
        cout<<(*iter)->getName()<<endl;
    }

    cout<<"============================="<<endl;

    follow = (std::dynamic_pointer_cast<NonTerminalSymbol>(E_))->getFollow();
    for (iter = follow.begin(); iter != follow.end(); iter++) {
        cout<<(*iter)->getName()<<endl;
    }

    cout<<"============================="<<endl;

    follow = (std::dynamic_pointer_cast<NonTerminalSymbol>(T))->getFollow();
    for (iter = follow.begin(); iter != follow.end(); iter++) {
        cout<<(*iter)->getName()<<endl;
    }
    cout<<"============================="<<endl;

    follow = (std::dynamic_pointer_cast<NonTerminalSymbol>(T_))->getFollow();
    for (iter = follow.begin(); iter != follow.end(); iter++) {
        cout<<(*iter)->getName()<<endl;
    }

    cout<<"============================="<<endl;

    follow = (std::dynamic_pointer_cast<NonTerminalSymbol>(F))->getFollow();
    for (iter = follow.begin(); iter != follow.end(); iter++) {
        cout<<(*iter)->getName()<<endl;
    }
*/

   /* unordered_set<TerminalSymbol::ptr> first = (std::dynamic_pointer_cast<NonTerminalSymbol>(E))->getFirst();
    unordered_set<TerminalSymbol::ptr>::iterator iter;
    for (iter = first.begin(); iter != first.end(); iter++) {
        cout<<(*iter)->getName()<<endl;
    }

    cout<<"============================="<<endl;

    first = (std::dynamic_pointer_cast<NonTerminalSymbol>(E_))->getFirst();
    for (iter = first.begin(); iter != first.end(); iter++) {
        cout<<(*iter)->getName()<<endl;
    }

    cout<<"============================="<<endl;

    first = (std::dynamic_pointer_cast<NonTerminalSymbol>(T))->getFirst();
    for (iter = first.begin(); iter != first.end(); iter++) {
        cout<<(*iter)->getName()<<endl;
    }
    cout<<"============================="<<endl;

    first = (std::dynamic_pointer_cast<NonTerminalSymbol>(T_))->getFirst();
    for (iter = first.begin(); iter != first.end(); iter++) {
        cout<<(*iter)->getName()<<endl;
    }

    cout<<"============================="<<endl;

    first = (std::dynamic_pointer_cast<NonTerminalSymbol>(F))->getFirst();
    for (iter = first.begin(); iter != first.end(); iter++) {
        cout<<(*iter)->getName()<<endl;
    }
*/




  return 0;
}




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
