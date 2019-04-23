#include <sstream>
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

Parser::Parser(LexicalAnalyzer &lexicalAnalyzer, std::istream &inputStream)
        : lexicalAnalyzer(lexicalAnalyzer), programStream(nullptr) {
    int terminalsCount, nonterminalsCount;
    std::string buffer;

    /* Read non-terminals */
    inputStream >> nonterminalsCount;
    getline(inputStream, buffer);// End line
    std::shared_ptr<NonTerminalSymbol> nonterminalsArray[nonterminalsCount];
    for (int i = 0; i < nonterminalsCount; i++) {
        getline(inputStream, buffer);
        nonTerminals[buffer] = nonterminalsArray[i] =
                std::make_shared<NonTerminalSymbol> (buffer);
    }

    /* Read starting symbol */
    getline(inputStream, buffer);
    startingSymbol = nonTerminals[buffer];

    /* Read terminals */
    inputStream >> terminalsCount;
    getline(inputStream, buffer);// End line
    std::shared_ptr<TerminalSymbol> terminalsArray[terminalsCount];
    for (int i = 0; i < terminalsCount; i++) {
        getline(inputStream, buffer);
        terminals[buffer] = terminalsArray[i] =
                std::make_shared<TerminalSymbol> (buffer);
    }

    /* Prepare epsilon production */
    std::shared_ptr<TerminalSymbol> epsilonTerminal =
            terminals[""] = std::make_shared<TerminalSymbol> ("");
    GrammarSymbol::Production epsilonProduction;
    epsilonProduction.push_back(epsilonTerminal);

    /* Prepare synchronization production */
    std::shared_ptr<TerminalSymbol> synchTerminal =
            terminals["$"] = std::make_shared<TerminalSymbol> ("$");
    GrammarSymbol::Production synchProduction;
    synchProduction.push_back(synchTerminal);

    /* Read non-terminals productions */
    std::string productionTok, grammarSymbolTok;
    for (int i = 0; i < nonterminalsCount; i++) {
        getline(inputStream, buffer);

        /* Tokenize line on spaces */
        std::stringstream productionsSS (buffer);
        for (int terminalIndex = 0; getline(productionsSS, productionTok,
                                            ' '); terminalIndex++) {
            if (productionTok == "$") {
                nonterminalsArray[i]->addTransition
                        (terminalsArray[terminalIndex], synchProduction);
            } else if (productionTok == "$$") {
                nonterminalsArray[i]->addTransition
                        (terminalsArray[terminalIndex], epsilonProduction);
            } else if (productionTok == "$$$") {
                continue;
            } else {
                /* Tokenize productionToken on $ */
                NonTerminalSymbol::Production newProduction;
                std::stringstream grammarSymbolsSS(productionTok);
                while (getline(grammarSymbolsSS, grammarSymbolTok, '$')) {
                    std::shared_ptr<GrammarSymbol> symbol;

                    /* Find given symbol */
                    if (terminals.find(grammarSymbolTok) == terminals.end()) {
                        if (nonTerminals.find(grammarSymbolTok) ==
                            nonTerminals.end()) {
                            throw std::runtime_error("A grammar symbol is "
                                                     "neither "
                                                     "a given terminal nor "
                                                     "non-terminal");
                        } else {
                            symbol = nonTerminals[grammarSymbolTok];
                        }
                    } else {
                        symbol = terminals[grammarSymbolTok];
                    }

                    newProduction.push_back(symbol);
                }
                nonterminalsArray[i]->addTransition
                        (terminalsArray[terminalIndex], newProduction);
            }
        }

    }


}


// the stream should be go to lexical analyzer
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
                output += terminal->getName() + " ";
                if (terminal->getName().compare(currentToken.getType()) == 0){
                    note = "matched";
                    currentToken = lexicalAnalyzer.nextToken();
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
            //sentence.push_back(startingSymbol);
            note = "error extra token";

            std::cout<<"================="<<std::endl;
            std::cout<<output<<std::endl;
            std::cout<<note<<std::endl;
            currentToken = lexicalAnalyzer.nextToken();
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

//not implemented
Parser::Sentence Parser::getNextDerivation() {
    Parser::Sentence sentence;
    return sentence;
}

void Parser::save(std::ostream& out) {

    /* Write non-terminals names */
    out << nonTerminals.size() << std::endl;
    for (const auto& nonTerminalMapEntry : nonTerminals) {
        out << nonTerminalMapEntry.first << std::endl;
    }

    /* Write the name of starting symbol */
    out << startingSymbol->getName() << std::endl;

    /* Write terminals */
    out << terminals.size() << std::endl;
    for (const auto& terminalMapEntry : terminals) {
        out << terminalMapEntry.first << std::endl;
    }

    /* Write non-terminals productions */
    for (const auto& nonTerminalMapEntry : nonTerminals) {
        std::shared_ptr<NonTerminalSymbol> nonTerminal =
                nonTerminalMapEntry.second;
        nonTerminal->saveProductions(out, terminals);
    }
}
