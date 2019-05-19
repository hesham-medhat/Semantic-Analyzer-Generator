#include <sstream>
#include "Parser.h"
#include "../SemanticAnalyzerGenerator/SemanticAction.h"


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
    std::shared_ptr<TerminalSymbol> epsilonTerminal = terminals[""];
    std::shared_ptr<GrammarSymbol::Production> epsilonProduction =
            std::make_shared<GrammarSymbol::Production>();

    epsilonProduction->push_back(epsilonTerminal);

    /* Prepare synchronization production */
    std::shared_ptr<TerminalSymbol> synchTerminal =
            std::make_shared<TerminalSymbol> ("$");
    std::shared_ptr<GrammarSymbol::Production> synchProduction =
            std::make_shared<GrammarSymbol::Production>();
    synchProduction->push_back(synchTerminal);

    /* Read non-terminals productions */
    std::string productionTok, grammarSymbolTok, productionIdTok;
    int productionId;
    std::unordered_map<int, std::shared_ptr<Sentence>> prodIdMapper;
    for (int i = 0; i < nonterminalsCount; i++) {
        getline(inputStream, buffer);

        /* Tokenize line on spaces */
        std::stringstream productionsSS (buffer);
        for (int terminalIndex = 0; getline(productionsSS, productionTok,
                                            ' '); terminalIndex++) {
            /* Read associated production id*/
            getline(productionsSS, productionIdTok, ' ');
            std::stringstream productionIdSS(productionIdTok);
            productionIdSS >> productionId;

            if (productionTok == "$") {
                nonterminalsArray[i]->addTransition
                        (terminalsArray[terminalIndex], synchProduction);
            } else if (productionTok == "$$") {
                nonterminalsArray[i]->addTransition
                        (terminalsArray[terminalIndex], epsilonProduction);
                nonterminalsArray[i]->hasEpsilonProduction = true;
            } else if (productionTok == "$$$") {
                continue;
            } else {
                /* Check if same sentence already read */
                if (prodIdMapper.find(productionId) != prodIdMapper.end()) {
                    nonterminalsArray[i]->addTransition
                            (terminalsArray[terminalIndex],
                                    prodIdMapper[productionId]);
                    continue;
                }

                std::shared_ptr<Sentence> newProduction =
                        std::make_shared<Sentence>();
                if (productionTok == "$$$$$") {// Epsilon production with
                    // semantic action
                    newProduction->push_back(epsilonTerminal);
                    std::shared_ptr<SemanticAction> action =
                            std::make_shared<SemanticAction>();
                    nonterminalsArray[i]->addTransition
                            (terminalsArray[terminalIndex], newProduction);
                    productionIds[newProduction] = productionId;
                    nonterminalsArray[i]->hasEpsilonProduction = true;
                }

                /* Tokenize productionToken on $ */
                std::stringstream grammarSymbolsSS(productionTok);
                while (getline(grammarSymbolsSS, grammarSymbolTok, '$')) {
                    std::shared_ptr<GrammarSymbol> symbol;

                    /* Find given symbol */
                    if (grammarSymbolTok == SemanticAction::RESERVED_NAME) {
                        symbol = std::make_shared<SemanticAction>();
                    } else if (terminals.find(grammarSymbolTok) == terminals
                    .end()) {
                        if (nonTerminals.find(grammarSymbolTok) ==
                            nonTerminals.end()) {
                            throw std::runtime_error("A grammar symbol is "
                                                     "neither "
                                                     "a given terminal nor "
                                                     "non-terminal nor the "
                                                     "reserved semantic "
                                                     "action name");
                        } else {
                            symbol = nonTerminals[grammarSymbolTok];
                        }
                    } else {
                        symbol = terminals[grammarSymbolTok];
                    }

                    newProduction->push_back(symbol);
                }
                nonterminalsArray[i]->addTransition
                        (terminalsArray[terminalIndex], newProduction);
                productionIds[newProduction] = productionId;
            }
        }

    }


}


// the stream should be go to lexical analyzer
void Parser::parseFullProgram(std::istream &) {

    Token currentToken = lexicalAnalyzer.nextToken();
    std::string output;
    std::string note;

    typedef std::pair<std::shared_ptr<GrammarSymbol>,
            std::shared_ptr<SemanticAnalyzer>> SymbolAnalyzerPair;

    std::deque<SymbolAnalyzerPair> sentence;

    SymbolAnalyzerPair startingPair(startingSymbol, nullptr);
    sentence.push_back(startingPair);
    while (!currentToken.getType().empty()) {
        if(!sentence.empty()) {
            SymbolAnalyzerPair sap = *sentence.begin();
            GrammarSymbol::ptr symbol = sap.first;
            if (symbol->getType() == GrammarSymbol::Type::Terminal) {
                TerminalSymbol::ptr terminal = std::dynamic_pointer_cast<TerminalSymbol>(
                        symbol);
                sentence.pop_front();
                output += terminal->getName() + " ";
                if (terminal->getName() == currentToken.getType()) {
                    note = "matched";
                    currentToken = lexicalAnalyzer.nextToken();
                } else {
                    note = "error insert unmatched symbol " +
                           terminal->getName();
                }
            } else if (symbol->getType() ==
            GrammarSymbol::Type::SemanticAction) {
                std::shared_ptr<SemanticAction> action =
                        std::dynamic_pointer_cast<SemanticAction>(symbol);
                action->execute(currentToken.getLexeme());
                sentence.pop_front();
            } else {
                NonTerminalSymbol::ptr nonTerminal =
                        std::dynamic_pointer_cast<NonTerminalSymbol>(symbol);
                std::shared_ptr<GrammarSymbol::Production> production =
                        nonTerminal->getProduction(terminals[currentToken.getType()]);
                if(!production->empty()){
                    sentence.pop_front();
                    if((*(production->begin()))->getName().compare("$") == 0){
                        output += "$ ";
                        note = "error in " + nonTerminal->getName();
                    } else if((*(production->begin()))->getName().empty()) {
                        note = nonTerminal->getName() + " -> ";

                    } else {// Non-terminal
                        std::shared_ptr<SemanticAnalyzer> derivationAnalyzer =
                                semanticAnalyzerFactory->getSemanticAnalyzer
                                (productionIds[production], (sap.second? sap
                                .second->getNextNonTerminal() : nullptr));
                        std::deque<SymbolAnalyzerPair> derivation;
                        for(const auto& symbolRef : *production) {
                            std::shared_ptr<GrammarSymbol> newSymbol;
                            if (symbolRef->getType() ==
                            GrammarSymbol::Type::SemanticAction) {
                                newSymbol = std::make_shared<SemanticAction>();
                                std::shared_ptr<SemanticAction> action =
                                        std::dynamic_pointer_cast
                                                <SemanticAction>(newSymbol);
                                action->setSemanticAnalyzer(derivationAnalyzer);
                            } else {
                                newSymbol = symbolRef;
                            }
                            SymbolAnalyzerPair derivedSAP(newSymbol,
                                    derivationAnalyzer);
                            derivation.push_back(derivedSAP);
                        }
                        // INSERT DERIVATION
                        sentence.insert(sentence.begin(),derivation.begin(),
                                derivation.end());
                        note = nonTerminal->getName() + " ->";
                        GrammarSymbol::Production::iterator proIte;
                        for (proIte = production->begin(); proIte !=
                        production->end(); proIte++){
                            note += " " + (*proIte)->getName();
                        }
                    }
                } else {
                    note = "discard the token";
                    currentToken = lexicalAnalyzer.nextToken();
                }
            }
            std::cout<<"================="<<std::endl;
            std::cout<<output;
            for (auto& stackIte : sentence) {
                std::cout<<stackIte.first->getName()+ " ";
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
        GrammarSymbol::ptr symbol = (*sentence.begin()).first;
        sentence.pop_front();
        if (symbol->getType() == GrammarSymbol::Type::NonTerminal) {
            NonTerminalSymbol::ptr nonTerminal = std::dynamic_pointer_cast<NonTerminalSymbol>(
                    symbol);
            if (nonTerminal->hasEpsilonProduction) {
                note = nonTerminal->getName() + " -> ";
            } else {
                output += "$ ";
                note = "error in " + nonTerminal->getName();
            }
        } else if (symbol->getType() == GrammarSymbol::Type::SemanticAction) {
            std::shared_ptr<SemanticAction> action =
                    std::dynamic_pointer_cast<SemanticAction>(symbol);
            action->execute(currentToken.getLexeme());
        } else {// Non-terminal
            output += symbol->getName() + " ";
            note = "error insert unmatched symbol " + symbol->getName();
        }
        std::cout<<"================="<<std::endl;
        std::cout<<output;
        for (auto& stackIte : sentence) {
            std::cout<<stackIte.first->getName()+ " ";
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
    throw std::runtime_error("Parser::getNextDerivation is not implemented");
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
        nonTerminal->saveProductions(out, terminals, productionIds);
    }
}

void Parser::initProgramParse(const std::string& path) {
    lexicalAnalyzer.initProgramParse(path);
}

void Parser::setSemanticAnalyzerFactory(
        std::unique_ptr<AbstractSemanticAnalyzerFactory> value) {
    semanticAnalyzerFactory = std::move(value);
}
