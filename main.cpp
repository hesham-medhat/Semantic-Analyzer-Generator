#include <iostream>
#include <string>
#include <fstream>
#include "ParserGenerator/GrammarSymbol.h"
#include "ParserGenerator/TerminalSymbol.h"
#include "ParserGenerator/NonTerminalSymbol.h"
#include "LexicalAnalysis/LexicalAnalyzerGenerator.h"
#include "LexicalAnalysis/LexicalAnalyzer.h"




int main(int argc, char* argv[]) {

    /*
    E -> TE’
    E’ -> +E | 
    T -> FT’
    T’ -> T | 
    F -> PF'
    F' -> *F | 
    P -> (E) | a | b |Em
    */

/*

    GrammarSymbol::ptr E = std::make_shared<NonTerminalSymbol> ("E");
    GrammarSymbol::ptr E_ = std::make_shared<NonTerminalSymbol> ("E`");
    GrammarSymbol::ptr T = std::make_shared<NonTerminalSymbol> ("T");
    GrammarSymbol::ptr T_ = std::make_shared<NonTerminalSymbol> ("T`");
    GrammarSymbol::ptr F = std::make_shared<NonTerminalSymbol> ("F");
    GrammarSymbol::ptr F_ = std::make_shared<NonTerminalSymbol> ("F`");
    GrammarSymbol::ptr P = std::make_shared<NonTerminalSymbol> ("P");

    GrammarSymbol::ptr plus = std::make_shared<TerminalSymbol> ("+");
    GrammarSymbol::ptr openPractice = std::make_shared<TerminalSymbol> ("(");
    GrammarSymbol::ptr closePractice = std::make_shared<TerminalSymbol> (")");
    GrammarSymbol::ptr x = std::make_shared<TerminalSymbol> ("*");
    GrammarSymbol::ptr a = std::make_shared<TerminalSymbol> ("a");
    GrammarSymbol::ptr b = std::make_shared<TerminalSymbol> ("b");
    GrammarSymbol::ptr Em = std::make_shared<TerminalSymbol> ("Em");
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
    std::deque<std::shared_ptr<GrammarSymbol>> production9;
    std::deque<std::shared_ptr<GrammarSymbol>> production10;
    std::deque<std::shared_ptr<GrammarSymbol>> production11;
    std::deque<std::shared_ptr<GrammarSymbol>> production12;
    std::deque<std::shared_ptr<GrammarSymbol>> production13;

    production1.push_back(T);
    production1.push_back(E_);

    production2.push_back(plus);
    production2.push_back(E);

    production3.push_back(epsilon);


    production4.push_back(F);
    production4.push_back(T_);

    production5.push_back(T);


    production6.push_back(epsilon2);

    production7.push_back(P);
    production7.push_back(F_);

    production8.push_back(x);
    production8.push_back(F);

    production9.push_back(epsilon);


    production10.push_back(openPractice);
    production10.push_back(E);
    production10.push_back(closePractice);

    production11.push_back(a);

    production12.push_back(b);

    production13.push_back(Em);

    NonTerminalSymbol::ptr En = std::dynamic_pointer_cast<NonTerminalSymbol>(E);
    NonTerminalSymbol::ptr E_n = std::dynamic_pointer_cast<NonTerminalSymbol>(E_);
    NonTerminalSymbol::ptr Tn = std::dynamic_pointer_cast<NonTerminalSymbol>(T);
    NonTerminalSymbol::ptr T_n = std::dynamic_pointer_cast<NonTerminalSymbol>(T_);
    NonTerminalSymbol::ptr Fn = std::dynamic_pointer_cast<NonTerminalSymbol>(F);
    NonTerminalSymbol::ptr F_n = std::dynamic_pointer_cast<NonTerminalSymbol>(F_);
    NonTerminalSymbol::ptr Pn = std::dynamic_pointer_cast<NonTerminalSymbol>(P);


    En->addProduction(production1);
    E_n->addProduction(production2);
    E_n->addProduction(production3);
    Tn->addProduction(production4);
    T_n->addProduction(production5);
    T_n->addProduction(production6);
    Fn->addProduction(production7);
    F_n->addProduction(production8);
    F_n->addProduction(production9);
    Pn->addProduction(production10);
    Pn->addProduction(production11);
    Pn->addProduction(production12);
    Pn->addProduction(production13);

    En->hasEpsilonProduction = false;
    E_n->hasEpsilonProduction = true;
    Tn->hasEpsilonProduction = false;
    T_n->hasEpsilonProduction = true;
    Fn->hasEpsilonProduction = false;
    F_n->hasEpsilonProduction = true;
    Pn->hasEpsilonProduction = false;



    En->addUsingProduction(E_n, production2);
    En->addUsingProduction(Pn, production10);

    E_n->addUsingProduction(En, production1);

    Tn->addUsingProduction(En, production1);
    Tn->addUsingProduction(T_n, production5);

    T_n->addUsingProduction(Tn, production4);

    Fn->addUsingProduction(Tn, production4);
    Fn->addUsingProduction(F_n, production8);

    F_n->addUsingProduction(Fn, production7);

    Pn->addUsingProduction(Fn, production7);




    unordered_set<std::string> emptySet;

    unordered_set<TerminalSymbol::ptr> follow;
    unordered_set<TerminalSymbol::ptr>::iterator iter;
    follow = En->getFollow(emptySet);
    for (iter = follow.begin(); iter != follow.end(); iter++) {
        cout<<(*iter)->getName()<<endl;
    }
    En->followCalculated = true;
    cout<<"============================="<<endl;

    follow = E_n->getFollow(emptySet);
    for (iter = follow.begin(); iter != follow.end(); iter++) {
        cout<<(*iter)->getName()<<endl;
    }
    E_n->followCalculated = true;
    cout<<"============================="<<endl;



    follow = Tn->getFollow(emptySet);
    for (iter = follow.begin(); iter != follow.end(); iter++) {
        cout<<(*iter)->getName()<<endl;
    }
    Tn->followCalculated = true;
    cout<<"============================="<<endl;

    follow = T_n->getFollow(emptySet);
    for (iter = follow.begin(); iter != follow.end(); iter++) {
        cout<<(*iter)->getName()<<endl;
    }
    T_n->followCalculated = true;
    cout<<"============================="<<endl;

    follow = Fn->getFollow(emptySet);
    for (iter = follow.begin(); iter != follow.end(); iter++) {
        cout<<(*iter)->getName()<<endl;
    }
    Fn->followCalculated = true;
    cout<<"============================="<<endl;

    follow = F_n->getFollow(emptySet);
    for (iter = follow.begin(); iter != follow.end(); iter++) {
        cout<<(*iter)->getName()<<endl;
    }
    F_n->followCalculated = true;
    cout<<"============================="<<endl;

    follow = Pn->getFollow(emptySet);
    for (iter = follow.begin(); iter != follow.end(); iter++) {
        cout<<(*iter)->getName()<<endl;
    }
    Pn->followCalculated = true;




    cout<<"#####################################"<<endl;
    cout<<"first"<<endl;

    unordered_set<TerminalSymbol::ptr> first;
    unordered_set<TerminalSymbol::ptr>::iterator iter2;
    first = En->getFirst(emptySet);
    for (iter2 = first.begin(); iter2 != first.end(); iter2++) {
        cout<<(*iter2)->getName()<<endl;
    }
    En->firstCalculated = true;
    cout<<"============================="<<endl;

    first = E_n->getFirst(emptySet);
    for (iter2 = first.begin(); iter2 != first.end(); iter2++) {
        cout<<(*iter2)->getName()<<endl;
    }
    E_n->firstCalculated = true;
    cout<<"============================="<<endl;

    first = Tn->getFirst(emptySet);
    for (iter2 = first.begin(); iter2 != first.end(); iter2++) {
        cout<<(*iter2)->getName()<<endl;
    }
    Tn->firstCalculated = true;
    cout<<"============================="<<endl;

    first = T_n->getFirst(emptySet);
    for (iter2 = first.begin(); iter2 != first.end(); iter2++) {
        cout<<(*iter2)->getName()<<endl;
    }
    T_n->firstCalculated = true;
    cout<<"============================="<<endl;

    first = Fn->getFirst(emptySet);
    for (iter2 = first.begin(); iter2 != first.end(); iter2++) {
        cout<<(*iter2)->getName()<<endl;
    }
    Fn->firstCalculated = true;
    cout<<"============================="<<endl;

    first = F_n->getFirst(emptySet);
    for (iter2 = first.begin(); iter2 != first.end(); iter2++) {
        cout<<(*iter2)->getName()<<endl;
    }
    F_n->firstCalculated = true;
    cout<<"============================="<<endl;

    first = Pn->getFirst(emptySet);
    for (iter2 = first.begin(); iter2 != first.end(); iter2++) {
        cout<<(*iter2)->getName()<<endl;
    }
    Pn->firstCalculated = true;
*/







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

    //production5.push_back(x);
    //production5.push_back(F);
    production5.push_back(T_);
    production5.insert(production5.begin(),F);
    production5.insert(production5.begin(),x);


    //production5.pop_front();
    GrammarSymbol::Production::iterator iter3 = production5.begin();
    GrammarSymbol::Production::iterator iter5 = iter3;
    iter5++;

    //cout<<(*iter3)->getName()<<endl;
    //cout<<(*iter5)->getName()<<endl;


    //cout<<(*(iter + production5.size() - 1))->getName();

    //cout<<(*(iter))->getName();
    //cout<<(*(iter+1))->getName();

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



    En->addUsingProduction(Fn, production7);

    E_n->addUsingProduction(En, production1);
    E_n->addUsingProduction(E_n, production2);

    Tn->addUsingProduction(En, production2);
    Tn->addUsingProduction(E_n, production1);

    T_n->addUsingProduction(Tn, production4);
    T_n->addUsingProduction(T_n, production5);

    Fn->addUsingProduction(Tn, production4);
    Fn->addUsingProduction(T_n, production5);




    unordered_set<std::string> emptySet;

    unordered_set<TerminalSymbol::ptr> follow;
    unordered_set<TerminalSymbol::ptr>::iterator iter;
    follow = En->getFollow(emptySet);
    for (iter = follow.begin(); iter != follow.end(); iter++) {
        cout<<(*iter)->getName()<<endl;
    }
    En->followCalculated = true;
    cout<<"============================="<<endl;

    follow = E_n->getFollow(emptySet);
    for (iter = follow.begin(); iter != follow.end(); iter++) {
        cout<<(*iter)->getName()<<endl;
    }
    E_n->followCalculated = true;
    cout<<"============================="<<endl;



    follow = Tn->getFollow(emptySet);
    for (iter = follow.begin(); iter != follow.end(); iter++) {
        cout<<(*iter)->getName()<<endl;
    }
    Tn->followCalculated = true;
    cout<<"============================="<<endl;

    follow = T_n->getFollow(emptySet);
    for (iter = follow.begin(); iter != follow.end(); iter++) {
        cout<<(*iter)->getName()<<endl;
    }
    T_n->followCalculated = true;
    cout<<"============================="<<endl;

    follow = Fn->getFollow(emptySet);
    for (iter = follow.begin(); iter != follow.end(); iter++) {
        cout<<(*iter)->getName()<<endl;
    }
    Fn->followCalculated = true;
    cout<<"============================="<<endl;


    cout<<"#####################################"<<endl;
    cout<<"first"<<endl;

    unordered_set<TerminalSymbol::ptr> first;
    unordered_set<TerminalSymbol::ptr>::iterator iter2;
    first = En->getFirst(emptySet);
    for (iter2 = first.begin(); iter2 != first.end(); iter2++) {
        cout<<(*iter2)->getName()<<endl;
    }
    En->firstCalculated = true;
    cout<<"============================="<<endl;

    first = E_n->getFirst(emptySet);
    for (iter2 = first.begin(); iter2 != first.end(); iter2++) {
        cout<<(*iter2)->getName()<<endl;
    }
    E_n->firstCalculated = true;
    cout<<"============================="<<endl;

    first = Tn->getFirst(emptySet);
    for (iter2 = first.begin(); iter2 != first.end(); iter2++) {
        cout<<(*iter2)->getName()<<endl;
    }
    Tn->firstCalculated = true;
    cout<<"============================="<<endl;

    first = T_n->getFirst(emptySet);
    for (iter2 = first.begin(); iter2 != first.end(); iter2++) {
        cout<<(*iter2)->getName()<<endl;
    }
    T_n->firstCalculated = true;
    cout<<"============================="<<endl;

    first = Fn->getFirst(emptySet);
    for (iter2 = first.begin(); iter2 != first.end(); iter2++) {
        cout<<(*iter2)->getName()<<endl;
    }
    Fn->firstCalculated = true;




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
