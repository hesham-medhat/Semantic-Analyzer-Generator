#include <sstream>
#include "Parser.h"

Parser::Parser(LexicalAnalyzer &lexicalAnalyzer, std::istream &inputStream)
: lexicalAnalyzer(lexicalAnalyzer), programStream(nullptr) {
    int terminalsCount, nonterminalsCount;
    std::string buffer;

    /* Read non-terminals */
    inputStream >> nonterminalsCount;
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
                nonterminalsArray[i]->addProduction(synchProduction);
            } else if (productionTok == "$$") {
                nonterminalsArray[i]->addProduction(epsilonProduction);
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
                        nonTerminals.end() {
                            throw runtime_error("A grammar symbol is neither "
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
                nonterminalsArray[i]->addProduction(terminals[terminalIndex],
                        newProduction);
            }
        }



    }



}
