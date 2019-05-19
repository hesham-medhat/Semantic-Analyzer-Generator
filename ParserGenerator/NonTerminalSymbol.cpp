#include <ostream>
#include "NonTerminalSymbol.h"

NonTerminalSymbol::NonTerminalSymbol(std::string name) : GrammarSymbol
(std::move(name)) {
    firstCalculated = false;
    followCalculated = false;
    hasEpsilonProduction = false;
}


GrammarSymbol::Type NonTerminalSymbol::getType() {
    return GrammarSymbol::Type::NonTerminal;
}


//return empty set if not LL1
std::unordered_set<TerminalSymbol::ptr> NonTerminalSymbol::getFirst(std::unordered_set<std::string> caller) {
    caller.insert(this->getName());
    if(!firstCalculated) {
        std::vector<GrammarSymbol::Production>::iterator prodIte;
        for (prodIte = productions.begin(); prodIte != productions.end(); prodIte++) {
            GrammarSymbol::Production production = *prodIte;
            GrammarSymbol::Production::iterator symIte;
            for (symIte = production.begin(); symIte != production.end(); symIte++) {
                GrammarSymbol::ptr symbol = *symIte;
                if (symbol->getType() == GrammarSymbol::Type::Terminal) {
                    TerminalSymbol::ptr terminal = std::dynamic_pointer_cast<TerminalSymbol>(
                            symbol);
                    first.insert(terminal);
                    if (terminal->getName().compare("") != 0) {
                        if (transitions.find(terminal) == transitions.end()) {
                            addTransition(terminal,
                                          std::make_shared<Production>(production));
                        } else if (*transitions[terminal] != production) {
                            //first.clear();
                            //return first;
                            //throw error not LL1
                            throw NULL;
                        }
                    } else {
                        this->hasEpsilonProduction = true;
                        this->addTransition(
                          epsilon,
                          std::make_shared<Production>(production));
                    }
                    break;
                } else if (symbol->getType() ==
                GrammarSymbol::Type::SemanticAction) {
                    continue;
                } else {
                    NonTerminalSymbol::ptr nonTerminal = std::dynamic_pointer_cast<NonTerminalSymbol>(symbol);
                    bool noRecursion = true;
                    std::unordered_set<std::string>::iterator callerIter;
                    for(callerIter = caller.begin(); callerIter != caller.end(); callerIter++){
                        noRecursion = noRecursion && nonTerminal->getName().compare(*callerIter) != 0;
                    }
                    if (noRecursion) {
                        std::unordered_set<TerminalSymbol::ptr> secFirst = nonTerminal->getFirst(caller);
                        std::unordered_set<TerminalSymbol::ptr>::iterator secFirstIte;
                        for (secFirstIte = secFirst.begin(); secFirstIte != secFirst.end(); secFirstIte++) {
                            TerminalSymbol::ptr terminal = std::dynamic_pointer_cast<TerminalSymbol>(*secFirstIte);
                            if (terminal->getName().compare("") != 0) {
                                first.insert(terminal);
                                if (transitions.find(terminal) == transitions.end()) {
                                  addTransition(terminal,
                                                std::make_shared<Production>(production));
                                } else if(*transitions[terminal] != production){
                                    //first.clear();
                                    //return first;
                                    //throw error not LL1
                                    throw NULL;
                                }
                            } else if (symIte + 1 == production.end()) {
                                first.insert(terminal);
                            }
                        }
                    }
                    if (!nonTerminal->hasEpsilonProduction) {
                        break;
                    }
                }
            }
        }
       /* std::cout<<this->getName()<<std::endl;
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
    }
    return first;
}

std::unordered_set<TerminalSymbol::ptr> NonTerminalSymbol::getFollow(std::unordered_set<std::string> caller) {
    caller.insert(this->getName());
    if(!followCalculated){

        std::vector<NonTerminalSymbol::usingPair>::iterator pairIter;
        for (pairIter = usingProductions.begin();
        pairIter != usingProductions.end(); pairIter++) {
            NonTerminalSymbol::ptr parent = (*pairIter).first;
            GrammarSymbol::Production production = (*pairIter).second;
            GrammarSymbol::Production::iterator prodIter;
            for(prodIter = production.begin(); prodIter != production.end(); prodIter++) {
                if((*prodIter)->getName().compare(this->getName()) == 0) {
                    GrammarSymbol::Production::iterator prodIter2 = prodIter;
                    prodIter2++;
                    while (prodIter2 != production.end()) {
                        GrammarSymbol::ptr nextSymbol = *prodIter2;
                        if (nextSymbol->getType() ==
                        GrammarSymbol::Type::Terminal) {
                            TerminalSymbol::ptr terminal = std::dynamic_pointer_cast<TerminalSymbol>(
                                    nextSymbol);
                            follow.insert(terminal);
                            break;
                        } else if (nextSymbol->getType() ==
                                   GrammarSymbol::Type::SemanticAction) {
                            prodIter2++;
                            continue;
                        } else {
                            NonTerminalSymbol::ptr nonTerminal = std::dynamic_pointer_cast<NonTerminalSymbol>(
                                    nextSymbol);
                            std::unordered_set<std::string> emptySet;
                            std::unordered_set<TerminalSymbol::ptr> secFollow = nonTerminal->getFirst(emptySet);

                            std::unordered_set<TerminalSymbol::ptr>::iterator secFollowIter;
                            for (secFollowIter = secFollow.begin(); secFollowIter != secFollow.end(); secFollowIter++) {
                                if ((*secFollowIter)->getName().compare("") != 0) {
                                    follow.insert(*secFollowIter);
                                } else {
                                    nonTerminal->hasEpsilonProduction = true;
                                    for (const auto& p : nonTerminal->productions) {
                                      GrammarSymbol::ptr firstSymbol = *p.begin();
                                      if (firstSymbol == *secFollowIter) {
                                        nonTerminal->addTransition(
                                          epsilon,
                                          std::make_shared<Production>(p));
                                        break;
                                      } else if (firstSymbol->getType() == NonTerminal) {
                                        NonTerminalSymbol::ptr fSymNT =
                                          std::dynamic_pointer_cast<NonTerminalSymbol>(firstSymbol);
                                        auto fSymFirstSet = nonTerminal->getFirst(emptySet);
                                        if (fSymFirstSet.find(*secFollowIter) != fSymFirstSet.end()) {
                                        nonTerminal->addTransition(
                                          epsilon,
                                          std::make_shared<Production>(p));
                                          break;
                                        }
                                      }
                                    }
                                }
                            }
                            if (!nonTerminal->hasEpsilonProduction) {
                                break;
                            } else {
                                prodIter2++;
                            }
                        }
                    }
                    bool noRecursion = true;
                    std::unordered_set<std::string>::iterator callerIter;
                    for(callerIter = caller.begin(); callerIter != caller.end(); callerIter++){
                        noRecursion = noRecursion && parent->getName().compare(*callerIter) != 0;
                    }
                    if ((prodIter2) == production.end() && noRecursion) {
                        std::unordered_set<TerminalSymbol::ptr> secFollow = parent->getFollow(caller);
                        std::unordered_set<TerminalSymbol::ptr>::iterator secFollowIter;
                        for (secFollowIter = secFollow.begin(); secFollowIter != secFollow.end(); secFollowIter++) {
                            follow.insert(*secFollowIter);
                        }
                    }
                }
            }
        }

        TerminalSymbol::ptr epsTerminal = epsilon;
        TerminalSymbol::ptr synTerminal = std::make_shared<TerminalSymbol>("$");
        GrammarSymbol::Production epsProduction;
        epsProduction.push_back(epsTerminal);
        GrammarSymbol::Production synProduction;
        synProduction.push_back(synTerminal);
        std::unordered_set<TerminalSymbol::ptr>::iterator followIter;
        for (followIter = follow.begin(); followIter != follow.end(); followIter++) {
            if(transitions.find(*followIter) == transitions.end()){
                if(hasEpsilonProduction){
                  addTransition(epsilon,
                                std::make_shared<Production>(epsProduction));
                } else {
                  addTransition(*followIter,
                                std::make_shared<Production>(synProduction));
                }
            } else if(hasEpsilonProduction){
                    GrammarSymbol::Production production =
                            *transitions[*followIter];
                    if((*production.begin())->getName().compare("") != 0) {
                        throw NULL;
                        //follow.clear();
                        //return follow;
                        //throw error not LL1
                    }
            }
        }
        /*if(hasEpsilonProduction){
            transitions[synTerminal] = epsProduction;
        } else {
            transitions[synTerminal] = synProduction;
        }*/

        /*std::cout<<this->getName()<<std::endl;
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
        std::cout <<(synTerminal)->getName()<<" -> ";
        if(hasEpsilonProduction){
            std::cout<< " ";
        } else {
            std::cout<< "$";
        }
        std::cout<<std::endl;
        std::cout<<"+++++++++++++++++++++++++++"<<std::endl;*/
    }

    return follow;
}

void NonTerminalSymbol::addTransition(TerminalSymbol::ptr input,
        std::shared_ptr<GrammarSymbol::Production> newProduction) {

/* Update hasEpsilonProduction if epsilon */

    if (newProduction->size() <= 2 &&
        (*newProduction->begin())->getName().empty()) {
        std::shared_ptr<GrammarSymbol> singleTerminal = *newProduction->begin();
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

void NonTerminalSymbol::addUsingProduction(NonTerminalSymbol::ptr nonTerm, GrammarSymbol::Production production) {
    NonTerminalSymbol::usingPair pair(nonTerm,production);
    usingProductions.push_back(pair);
}



std::shared_ptr<GrammarSymbol::Production> NonTerminalSymbol::getProduction(
        TerminalSymbol::ptr terminal) {
    std::shared_ptr<GrammarSymbol::Production> production;
    if(transitions.find(terminal) != transitions.end()) {
        production = transitions[terminal];
    }
    return production;
}




void NonTerminalSymbol::saveProductions(std::ostream &out,
        std::unordered_map<std::string, std::shared_ptr<TerminalSymbol>>&
        terminals, std::unordered_map<std::shared_ptr<Production>, int>&
                productionIds) {
    int productionId = -1;// Meaning not a tracked production
    for (const auto& terminalEntry : terminals) {
        productionId = -1;
        const std::shared_ptr<TerminalSymbol> terminal = terminalEntry.second;
        if (transitions.find(terminal) != transitions.end()) {
            std::shared_ptr<Production> production = transitions[terminal];
            if (production->size() == 1 &&
            (*production->begin())->getName().empty()) {// Epsilon production
                out << static_cast<std::string>("$$");
            } else if (production->size() == 2 &&
                       (*production->begin())->getName().empty()) {
                // Epsilon production with a Semantic Action
                productionId = productionIds[production];
                out << static_cast<std::string>("$$$$$");
            } else {
                productionId = productionIds[production];
                bool firstSymbol = true;
                for (const auto& grammarSymbol : *production) {
                    if (!firstSymbol) out.put('$');
                    out << grammarSymbol->getName();
                    firstSymbol = false;
                }
            }
        } else {// Error production. Terminal symbol not mapped
            out << static_cast<std::string>("$$$");
        }
        out << " " << productionId << " ";
    }
    out << std::endl;
}
