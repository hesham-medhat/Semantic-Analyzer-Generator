#include <iostream>
#include <string>
#include <fstream>
#include <stack>
#include <set>
#include "LexicalAnalysis/Automata/NFAState.h"
#include "LexicalAnalysis/Automata/DFAState.h"
#include "LexicalAnalysis/Automata/Automaton.h"
#include "LexicalAnalysis/Automata/StateBuilder.h"
using namespace std;

unordered_set<shared_ptr<State>> getLambdaClosure(shared_ptr<State> state){

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

Automaton minimizeDFA(unordered_set<shared_ptr<State>> AllDFAStates) {
    vector<unordered_set<shared_ptr<State>>> groupSet;

    for (auto &originalState: AllDFAStates) {
        if (!groupSet.empty()) {
            bool noMatch = true;
            for (auto &group: groupSet) {
                for (auto &state: group) {
                    if (state->getAcceptedToken().getType() == originalState->getAcceptedToken().getType()) {
                        group.insert(originalState);
                        noMatch = false;
                        break;
                    }
                }
                if(!noMatch){
                    break;
                }
            }
            if(noMatch) {
                unordered_set<shared_ptr<State>> newGroup;
                newGroup.insert(originalState);
                groupSet.push_back(newGroup);
            }
        } else {
            unordered_set<shared_ptr<State>> newGroup;
            newGroup.insert(originalState);
            groupSet.push_back(newGroup);
        }
    }



    bool noSplit = false;

    while (!noSplit) {
        vector<unordered_set<shared_ptr<State>>> newGroupSet;
        for (auto &group: groupSet) {
            unordered_set<shared_ptr<State>> newGroup;
            for (auto &state: group) {
                newGroup.insert(state);
                for (auto &otherState: group) {
                    if (state != otherState) {
                        bool isMatch = true;
                        vector<char> vector1;
                        vector1.push_back('i');
                        vector1.push_back('f');
                        vector1.push_back('e');
                        vector1.push_back('l');
                        vector1.push_back('s');
                        for(const auto& i: vector1 ){
                        //for (int i = 1; i < 256 && isMatch; i++) {
                            unordered_set<shared_ptr<State>> transition1 = state->getNextState(i);
                            unordered_set<shared_ptr<State>> transition2 = otherState->getNextState(i);
                            if (transition1.size() != transition2.size()) {
                                isMatch = false;
                                break;
                            }
                            if (transition1.size() != 0) {
                                for (auto &nextSate: transition1) {
                                    for (auto &otherNextState: transition2) {
                                        if (nextSate != otherNextState) {
                                            isMatch = false;
                                        }
                                    }
                                }
                            }
                            if(!isMatch){
                                break;
                            }
                        }
                        if (isMatch) {
                            newGroup.insert(otherState);
                            group.erase(otherState);
                        }
                    }
                }
                newGroupSet.push_back(newGroup);
            }
            cout<<newGroupSet.size()<<endl;
            //newGroupSet.push_back(newGroup);
        }
        if(newGroupSet.size() == groupSet.size()){
            noSplit = true;
        } else {
            groupSet = newGroupSet;
        }
    }
    cout<<groupSet.size()<<endl;
    Automaton a;
    return a;
}

Automaton convertNFAToDFA(Automaton NFA){
    //unordered_set<pair<shared_ptr<State>, unordered_set<shared_ptr<State>>>> DFAPairSet;
    vector<pair<shared_ptr<State>, unordered_set<shared_ptr<State>>>> DFAPairSet;
    unordered_set<shared_ptr<State>> AllDFAStates;


    unordered_set<shared_ptr<State>> NFAStartEquivalents = getLambdaClosure(NFA.startState);
    shared_ptr<State> DFAStart = StateBuilder::buildState("DFA","");
    for(const auto&  newState: NFAStartEquivalents){
        //still need to find most priority token
        if(newState->getAcceptedToken().getType() != ""){
            DFAStart = StateBuilder::buildState("DFA", newState->getAcceptedToken().getType());
            break;
        }
    }
    Automaton DFA;
    DFA.startState = DFAStart;
    AllDFAStates.insert(DFAStart);

    pair<shared_ptr<State>, unordered_set<shared_ptr<State>>> pair1(DFAStart,NFAStartEquivalents);
    DFAPairSet.push_back(pair1);
    //DFAPairSet.insert(pair1);

    stack<pair<shared_ptr<State>, unordered_set<shared_ptr<State>>>> DFAStack;
    DFAStack.push(pair1);
    int counter = 1;
    while (!DFAStack.empty()){
        cout<< counter++<<endl;

        pair<shared_ptr<State>, unordered_set<shared_ptr<State>>> CurrentDFAPair =DFAStack.top();
        unordered_set<shared_ptr<State>> currentSet = CurrentDFAPair.second;
        DFAStack.pop();

        //int i =105;
        //vector<char> vector1;
        //vector1.push_back('i');
        //vector1.push_back('f');
        for(int i = 1; i < 256; i++){
            //for(const auto& i: vector1 ){
            unordered_set<shared_ptr<State>> newSet;
            for(const auto& NFAState : currentSet){
                unordered_set<shared_ptr<State>> nextSet = NFAState->getNextState(i);
                for(const auto& nextState : nextSet){
                    newSet.insert(nextState);
                    unordered_set<shared_ptr<State>> newSetEquivalents = getLambdaClosure(nextState);
                    for(const auto& nextStateEq : newSetEquivalents){
                        newSet.insert(nextStateEq);
                    }
                }
            }
            bool allFonded = false;
            for(const auto&  DFAPair: DFAPairSet){
                if(newSet.size() == DFAPair.second.size()){
                    for(const auto&  newState: newSet){
                        bool founded = false;
                        for(const auto&  currentState: DFAPair.second){
                            if(newState == currentState){
                                founded = true;
                                break;
                            }
                        }
                        allFonded &= founded;
                    }
                }
                if(allFonded ){
                    CurrentDFAPair.first->addTransition(i,DFAPair.first);
                    break;
                }
            }
            if(!allFonded && newSet.size() != 0){
                //still need to find most priority token
                shared_ptr<State> newDFAState = StateBuilder::buildState("DFA","");
                for(const auto&  newState: newSet){
                    if(newState->getAcceptedToken().getType() != ""){
                        newDFAState = StateBuilder::buildState("DFA", newState->getAcceptedToken().getType());
                        break;
                    }
                }
                AllDFAStates.insert(newDFAState);
                pair<shared_ptr<State>, unordered_set<shared_ptr<State>>> newPair(newDFAState,newSet);
                DFAPairSet.push_back(newPair);
                DFAStack.push(newPair);
                CurrentDFAPair.first->addTransition(i,newPair.first);
            }
        }
    }
    minimizeDFA(AllDFAStates);
    return DFA;
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

    Automaton result = convertNFAToDFA(a1);
    //ofstream outfile;
    //outfile.open("file.txt", ios::out );
    //result.saveIntoFile(outfile);

/*
    unordered_map<char, unordered_set<shared_ptr<State>>> unordered_map1;
    shared_ptr<State> s1 = StateBuilder::buildState("NFA","");
    shared_ptr<State> s2 = StateBuilder::buildState("NFA","");
    shared_ptr<State> s3 = StateBuilder::buildState("NFA","");
    shared_ptr<State> s4 = StateBuilder::buildState("NFA","");
    shared_ptr<State> s5 = StateBuilder::buildState("NFA","");
    shared_ptr<State> s6 = StateBuilder::buildState("NFA","");
    unordered_set<shared_ptr<State>> set1;
    set1.insert(s1);
    set1.insert(s2);

    unordered_map1['3'] = set1;

    set1 = unordered_map1['2'];
    if(set1.size() == 0){
        cout<< "null";
    }

*/

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
    unordered_set<shared_ptr<State>> x = getLambdaClosure(s1);
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
/*
    set<string> set1;
    set1.insert("a");
    set1.insert("a");
    set1.insert("a");
    cout<< set1.size();*/



    set<string> set1;
    set1.insert("a");
    set1.insert("c");
    set1.insert("b");
    int counter = 0;
    for(auto&  newState: set1){
        cout<<counter++<<endl;
        for(auto&  newState2: set1){
            if(newState2 == "c"){
                set1.erase(newState2);
            }
        }
    }

    cout<< set1.size();
    return 0;



}


//khalid priority