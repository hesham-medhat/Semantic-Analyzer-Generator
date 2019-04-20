#include "NonTerminalSymbol.h"

NonTerminalSymbol::NonTerminalSymbol(std::string name) : GrammarSymbol
(std::move(name)) {
    firstCalculated = false;
    followCalculated = false;
}


GrammarSymbol::Type NonTerminalSymbol::getType() {
    return GrammarSymbol::Type::NonTerminal;
}


//return empty set if not LL1
std::unordered_set<TerminalSymbol::ptr> NonTerminalSymbol::getFirst() {
    if(!firstCalculated) {
        std::vector<GrammarSymbol::Production>::iterator prodIte;
        for (prodIte = productions.begin(); prodIte != productions.end(); prodIte++) {
            GrammarSymbol::Production production = *prodIte;
            GrammarSymbol::Production::iterator symIte;
            for (symIte = production.begin(); symIte != production.end(); symIte++) {
                GrammarSymbol::ptr symbol = *symIte;
                if (symbol->getType() == GrammarSymbol::Type::Terminal) {
                    TerminalSymbol::ptr terminal = std::dynamic_pointer_cast<TerminalSymbol>(symbol);
                    first.insert(terminal);
                    if (terminal->getName().compare("") != 0) {
                        if (transitions.find(terminal) == transitions.end()) {
                            transitions[terminal] = production;
                        } else {
                            first.clear();
                            return first;
                            //throw error not LL1
                        }
                    }
                    break;
                } else {
                    NonTerminalSymbol::ptr nonTerminal = std::dynamic_pointer_cast<NonTerminalSymbol>(symbol);
//                NonTerminalSymbol::ptr nonTerminal = make_shared<NonTerminalSymbol>("o");
                    std::unordered_set<TerminalSymbol::ptr> secFirst = nonTerminal->getFirst();
                    std::unordered_set<TerminalSymbol::ptr>::iterator secFirstIte;
                    for (secFirstIte = secFirst.begin(); secFirstIte != secFirst.end(); secFirstIte++) {
                        TerminalSymbol::ptr terminal = std::dynamic_pointer_cast<TerminalSymbol>(*secFirstIte);
                        if (terminal->getName().compare("") != 0) {
                            first.insert(terminal);
                            if (transitions.find(terminal) == transitions.end()) {
                                transitions[terminal] = production;
                            } else {
                                first.clear();
                                return first;
                                //throw error not LL1
                            }
                        } else if (symIte + 1 == production.end()) {
                            first.insert(terminal);
                        }
                    }
                    if (!nonTerminal->hasEpsilonProduction) {
                        break;
                    }
                }
            }
        }
        firstCalculated = true;
    }
    /*std::cout<<this->getName()<<std::endl;
    std::unordered_set<TerminalSymbol::ptr>::iterator iterator;
    for (iterator = first.begin(); iterator != first.end(); iterator++) {
        GrammarSymbol::Production production = transitions[*iterator];
        GrammarSymbol::Production::iterator symIte;
        std::cout <<(*iterator)->getName()<<" -> ";
        for(symIte = production.begin(); symIte != production.end(); symIte++){
            std::cout<<(*symIte)->getName()<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<"+++++++++++++++++++++++++++"<<std::endl;*/
    return first;
}

/*
 * A -> aC
 * C -> bcA
 * problem
 */
std::unordered_set<TerminalSymbol::ptr> NonTerminalSymbol::getFollow() {
    if(!followCalculated){
        std::vector<NonTerminalSymbol::usingPair>::iterator pairIte;
        for (pairIte = usingProductions.begin();
        pairIte != usingProductions.end(); pairIte++) {
            NonTerminalSymbol::ptr parent = (*pairIte).first;
            GrammarSymbol::ptr nextSymbol = (*pairIte).second;
            if(nextSymbol->getType() == GrammarSymbol::Type::Terminal) {
                TerminalSymbol::ptr nextTerminal = std::dynamic_pointer_cast<TerminalSymbol>(nextSymbol);
                if(nextTerminal->getName() == "" && parent->getName().compare(this->getName()) != 0){
                    std::unordered_set<TerminalSymbol::ptr> secFollow = parent->getFollow();
                    std::unordered_set<TerminalSymbol::ptr>::iterator secFollowIter;
                    for (secFollowIter = secFollow.begin(); secFollowIter != secFollow.end(); secFollowIter++) {
                        follow.insert(*secFollowIter);
                    }
                } else if(nextTerminal->getName() != "" ){
                    follow.insert(nextTerminal);
                }
            } else {
                NonTerminalSymbol::ptr nextNonTerminal = std::dynamic_pointer_cast<NonTerminalSymbol>(nextSymbol);
                std::unordered_set<TerminalSymbol::ptr> secFollow = nextNonTerminal->getFirst();
                std::unordered_set<TerminalSymbol::ptr>::iterator secFollowIter;
                for (secFollowIter = secFollow.begin(); secFollowIter != secFollow.end(); secFollowIter++) {
                    if ((*secFollowIter)->getName().compare("") != 0) {
                        follow.insert(*secFollowIter);
                    }
                }
                if(nextNonTerminal->hasEpsilonProduction){
                    std::unordered_set<TerminalSymbol::ptr> secFollow2 = nextNonTerminal->getFollow();
                    std::unordered_set<TerminalSymbol::ptr>::iterator secFollowIter2;
                    for (secFollowIter2 = secFollow2.begin(); secFollowIter2 != secFollow2.end(); secFollowIter2++) {
                        follow.insert(*secFollowIter2);
                    }
                }
            }
        }

        TerminalSymbol::ptr epsTerminal = std::make_shared<TerminalSymbol>("");
        TerminalSymbol::ptr synTerminal = std::make_shared<TerminalSymbol>("$");
        GrammarSymbol::Production epsProduction;
        epsProduction.push_back(epsTerminal);
        GrammarSymbol::Production synProduction;
        synProduction.push_back(synTerminal);
        std::unordered_set<TerminalSymbol::ptr>::iterator followIter;
        for (followIter = follow.begin(); followIter != follow.end(); followIter++) {
            if(transitions.find(*followIter) == transitions.end()){
                if(hasEpsilonProduction){
                    transitions[*followIter] = epsProduction;
                } else {
                    transitions[*followIter] = synProduction;
                }
            } else {
                follow.clear();
                return follow;
                //throw error not LL1
            }
        }
        if(hasEpsilonProduction){
            transitions[synTerminal] = epsProduction;
        } else {
            transitions[synTerminal] = synProduction;
        }

        std::cout<<this->getName()<<std::endl;
        std::unordered_set<TerminalSymbol::ptr>::iterator iterator;
        for (iterator = follow.begin(); iterator != follow.end(); iterator++) {
            GrammarSymbol::Production production = transitions[*iterator];
            GrammarSymbol::Production::iterator symIte;
            std::cout <<(*iterator)->getName()<<" -> ";
            for(symIte = production.begin(); symIte != production.end(); symIte++){
                std::cout<<(*symIte)->getName()<<" ";
            }
            std::cout<<std::endl;
        }
        GrammarSymbol::Production production = transitions[synTerminal];
        GrammarSymbol::Production::iterator symIte;
        std::cout <<(synTerminal)->getName()<<" -> ";
        for(symIte = production.begin(); symIte != production.end(); symIte++){
            std::cout<<(*symIte)->getName()<<" ";
        }
        std::cout<<std::endl;
        std::cout<<"+++++++++++++++++++++++++++"<<std::endl;

        followCalculated = true;
    }
    return follow;
}

void NonTerminalSymbol::addTransition(TerminalSymbol::ptr input,
                                      GrammarSymbol::Production newProduction) {

/* Update hasEpsilonProduction if epsilon */

    if (newProduction.size() == 1) {
        std::shared_ptr<GrammarSymbol> singleTerminal = *newProduction.begin();
        std::string name = singleTerminal->getName();
        if (name.empty()) {// Epsilon transition
            hasEpsilonProduction = true;
        }
    }

    transitions[input] = newProduction;// Add production to map
}

void NonTerminalSymbol::addProduction(GrammarSymbol::Production production) {
    productions.push_back(production);
}

void NonTerminalSymbol::addUsingProduction(NonTerminalSymbol::ptr nonTerm, GrammarSymbol::ptr symbol) {
    NonTerminalSymbol::usingPair pair(nonTerm,symbol);
    usingProductions.push_back(pair);
}