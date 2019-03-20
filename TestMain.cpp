#include <iostream>
#include <string>
#include <fstream>
#include <stack>
#include "LexicalAnalysis/Automata/NFAState.h"
#include "LexicalAnalysis/Automata/DFAState.h"
#include "LexicalAnalysis/Automata/Automaton.h"
#include "LexicalAnalysis/Automata/StateBuilder.h"
using namespace std;

unordered_set<shared_ptr<State>> getEpsronClosure(shared_ptr<State> state){

    //push all states of T onto stack;
    unordered_set<shared_ptr<State>>  states;
    states = state->getNextState(0);
    stack<shared_ptr<State>> s;
    unordered_set<shared_ptr<State>>  result;
    result.insert(state);
    for (const auto& elem: states) {
        s.push(elem);
        result.insert(elem);
    }
    while (s.size() != 0) {
        shared_ptr<State> temp = s.top();
        s.pop();
        unordered_set<shared_ptr<State>>  near;
        near = temp->getNextState(0);
        for (const auto& elem: near) {
            if(result.find(elem) == result.end()){
                s.push(elem);
                result.insert(elem);
            }
        }
    }
    return result;
}
Automaton convertNFAToDFA(Automaton NFA){
    unordered_set<pair<shared_ptr<State>, unordered_set<shared_ptr<State>>>> DFAStates;

    shared_ptr<State> DFAStart = StateBuilder::buildState("DFA","");
    Automaton DFA;
    DFA.startState = DFAStart;
    unordered_set<shared_ptr<State>> NFAStartEquivalents = getEpsronClosure(NFA.startState);

    pair<shared_ptr<State>, unordered_set<shared_ptr<State>>> pair1(DFAStart,NFAStartEquivalents);
    DFAStates.insert(pair1);

    stack<pair<shared_ptr<State>, unordered_set<shared_ptr<State>>>> DFAStack;
    DFAStack.push(pair1);
    while (!DFAStack.empty()){
        unordered_set<shared_ptr<State>> currentSet = DFAStack.top().second;
        unordered_set<shared_ptr<State>> newSet;
        for(int i = 1; i < 256; i++){
            for(const auto& NFAState : currentSet){
                unordered_set<shared_ptr<State>> nextSet = NFAState->getNextState(i);
                for(const auto& nextState : nextSet){
                    newSet.insert(nextState);
                }
            }
            for(const auto& newState : newSet){
                newSet.insert(newState);
            }
        }
    }

}

int main() {

    Token t1("if");
    Token nullT("");
    Token t2("iff");
    Token t3("else");


    Automaton a1 = Automaton('i');
    Automaton a2 = Automaton('f');

    a1.concatenateOp(a2,t1);


    Automaton a3 = Automaton('i');
    Automaton a4 = Automaton('f');
    Automaton a5 = Automaton('f');

    a3.concatenateOp(a4,nullT);
    a3.concatenateOp(a5,t2);


    Automaton a6 = Automaton('e');
    Automaton a7 = Automaton('l');
    Automaton a8 = Automaton('s');
    Automaton a9 = Automaton('e');

    a6.concatenateOp(a7,nullT);
    a6.concatenateOp(a8,nullT);
    a6.concatenateOp(a9,t3);

    a1.unionOp(a3,nullT);
    a1.unionOp(a6,nullT);




/*
    shared_ptr<State> s1 = StateBuilder::buildState("NFA","");
    shared_ptr<State> s2 = StateBuilder::buildState("NFA","");
    shared_ptr<State> s3 = StateBuilder::buildState("NFA","");
    shared_ptr<State> s4 = StateBuilder::buildState("NFA","");
    shared_ptr<State> s5 = StateBuilder::buildState("NFA","");
    shared_ptr<State> s6 = StateBuilder::buildState("NFA","");
    s1->addTransition(0,s2);
    s1->addTransition('a',s6);
    s1->addTransition(0,s3);
    s3->addTransition(0,s4);
    s2->addTransition(0,s5);
    s5->addTransition(0,s3);
    unordered_set<shared_ptr<State>> x = getEpsronClosure(s1);
    if(x.find(s1) != x.end()){
        cout<<"1 is true"<<endl;
    }
    if(x.find(s2) != x.end()){
        cout<<"2 is true"<<endl;
    }
    if(x.find(s3) != x.end()){
        cout<<"3 is true"<<endl;
    }
    if(x.find(s4) != x.end()){
        cout<<"4 is true"<<endl;
    }
    if(x.find(s5) != x.end()){
        cout<<"5 is true"<<endl;
    }
    if(x.find(s6) == x.end()){
        cout<<"6 is true"<<endl;
    }


    */

    unordered_set<string> set;
    set.insert("a");
    set.insert("a");
    set.insert("a");
    cout<< set.size();
    return 0;



}


//khalid priority