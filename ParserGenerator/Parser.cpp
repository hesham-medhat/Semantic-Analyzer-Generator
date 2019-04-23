#include "Parser.h"

Parser::Parser(LexicalAnalyzer & lexicalAnalyzer,
               std::shared_ptr<NonTerminalSymbol> startingSymbol,
               std::unordered_map<std::string,
                       std::shared_ptr<TerminalSymbol>> &terminals,
               std::unordered_map<std::string,
               std::shared_ptr<NonTerminalSymbol>> &nonTerminals)
               : lexicalAnalyzer(lexicalAnalyzer), startingSymbol
               (std::move(startingSymbol)), terminals(terminals),
               nonTerminals(nonTerminals) {}


void Parser::parseFullProgram(std::istream &) {
    Token currentToken = lexicalAnalyzer.nextToken();
    Parser::Sentence sentence;
    std::string output = "";
    std::string note = "";
    sentence.push_back(startingSymbol);
    while (currentToken.getType().compare("") != 0){
        if(!sentence.empty()) {
            GrammarSymbol::ptr symbol = *sentence.begin();
            if (symbol->getType() == GrammarSymbol::Type::Terminal) {
                TerminalSymbol::ptr terminal = std::dynamic_pointer_cast<TerminalSymbol>(symbol);
                sentence.pop_front();
                currentToken = lexicalAnalyzer.nextToken();
                output += terminal->getName() + " ";
                if (terminal->getName().compare(currentToken.getType()) == 0){
                    note = "matched";
                } else {
                    note = "error insert unmatched symbol " + terminal->getName();
                }
            } else {
                NonTerminalSymbol::ptr nonTerminal = std::dynamic_pointer_cast<NonTerminalSymbol>(symbol);
                GrammarSymbol::Production production = nonTerminal->getProduction(terminals[currentToken.getType()]);
                if(!production.empty()){
                    sentence.pop_front();
                    if((*(production.begin()))->getName().compare("$") == 0){
                        output += "$ ";
                        note = "error in " + nonTerminal->getName();
                    } else if((*(production.begin()))->getName().compare("") == 0) {
                        note = nonTerminal->getName() + " -> ";
                    } else {
                        sentence.insert(sentence.begin(),production.begin(), production.end());
                        note = nonTerminal->getName() + " ->";
                        GrammarSymbol::Production::iterator proIte;
                        for (proIte = production.begin(); proIte != production.end(); proIte++){
                            note = " " + (*proIte)->getName();
                        }
                    }
                } else {
                    note = "discard the token";
                    currentToken = lexicalAnalyzer.nextToken();
                }
            }
            std::cout<<"================="<<std::endl;
            std::cout<<output;
            Parser::Sentence::iterator stackIte;
            for (stackIte = sentence.begin(); stackIte != sentence.end(); stackIte++){
                std::cout<<(*stackIte)->getName()+ " ";
            }
            std::cout<<std::endl;
            std::cout<<note<<std::endl;
        } else {
            sentence.push_back(startingSymbol);
            note = "end of sentence";
            std::cout<<"================="<<std::endl;
            std::cout<<output<<std::endl;
            std::cout<<note<<std::endl;
            output = "";
            std::cout<<"+++++++++++++++++++"<<std::endl;
        }
    }
    while (!sentence.empty()){
        GrammarSymbol::ptr symbol = *sentence.begin();
        if (symbol->getType() == GrammarSymbol::Type::NonTerminal) {
            NonTerminalSymbol::ptr nonTerminal = std::dynamic_pointer_cast<NonTerminalSymbol>(symbol);
            if(nonTerminal->hasEpsilonProduction){
                sentence.pop_front();
                note = nonTerminal->getName() + " -> ";
            } else {
                output += "$ ";
                note = "error in " + nonTerminal->getName();
            }
        } else {
            output += symbol->getName() + " ";
            note = "error insert unmatched symbol " + symbol->getName();
        }
        std::cout<<"================="<<std::endl;
        std::cout<<output;
        Parser::Sentence::iterator stackIte;
        for (stackIte = sentence.begin(); stackIte != sentence.end(); stackIte++){
            std::cout<<(*stackIte)->getName()+ " ";
        }
        std::cout<<std::endl;
        std::cout<<note<<std::endl;
    }
    note = "end of sentence";
    std::cout<<"================="<<std::endl;
    std::cout<<output<<std::endl;
    std::cout<<note<<std::endl;
}

//I should make currentToken global and make another function to loop in this and
// make the loop in the new function and the loop here will be in the last else condition below
Parser::Sentence Parser::getNextDerivation() {
    /*Token currentToken = lexicalAnalyzer.nextToken();
    Parser::Sentence sentence;
    std::string output = "";
    std::string note = "";
    sentence.push_back(startingSymbol);
    while (currentToken.getType().compare("") != 0){
        if(!sentence.empty()) {
            GrammarSymbol::ptr symbol = *sentence.begin();
            if (symbol->getType() == GrammarSymbol::Type::Terminal) {
                TerminalSymbol::ptr terminal = std::dynamic_pointer_cast<TerminalSymbol>(symbol);
                sentence.pop_front();
                currentToken = lexicalAnalyzer.nextToken();
                output += terminal->getName() + " ";
                if (terminal->getName().compare(currentToken.getType()) == 0){
                    note = "matched";
                } else {
                    note = "error insert unmatched symbol";
                }
            } else {
                NonTerminalSymbol::ptr nonTerminal = std::dynamic_pointer_cast<NonTerminalSymbol>(symbol);
                GrammarSymbol::Production production = nonTerminal->getProduction(terminals[currentToken.getType()]);
                if(!production.empty()){
                    sentence.pop_front();
                    if((*(production.begin()))->getName().compare("$") == 0){
                        output += "$ ";
                        note = "error in " + nonTerminal->getName();
                    } else {
                        sentence.insert(sentence.begin(),production.begin(), production.end());
                        note = nonTerminal->getName() + " ->";
                        GrammarSymbol::Production::iterator proIte;
                        for (proIte = production.begin(); proIte != production.end(); proIte++){
                            note = " " + (*proIte)->getName();
                        }
                    }
                } else {
                    note = "discard the token";
                    currentToken = lexicalAnalyzer.nextToken();
                }
            }
            std::cout<<"================="<<std::endl;
            std::cout<<output;
            Parser::Sentence::iterator stackIte;
            for (stackIte = sentence.begin(); stackIte != sentence.end(); stackIte++){
                std::cout<<(*stackIte)->getName()+ " ";
            }
            std::cout<<std::endl;
            std::cout<<note<<std::endl;
        } else {
            sentence.push_back(startingSymbol);
            note = "accept";
            std::cout<<"================="<<std::endl;
            std::cout<<output<<std::endl;
            std::cout<<note<<std::endl;
            output = "";

        }
    }
    if(!sentence.empty()){
        std::cout<<"================="<<std::endl;
        std::cout<<"the sentence not complete"<<std::endl;
    }*/
    Parser::Sentence sentence;
    return sentence;
}