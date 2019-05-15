#ifndef SEMANTIC_ANALYZER_GENERATOR_SEMANTICANALYZERGENERATOR_H
#define SEMANTIC_ANALYZER_GENERATOR_SEMANTICANALYZERGENERATOR_H


#include <string>
#include <unordered_map>
#include <fstream>
#include "../ParserGenerator/GrammarSymbol.h"
#include "../ParserGenerator/NonTerminalSymbol.h"

class SemanticAnalyzerGenerator {
public:
    explicit SemanticAnalyzerGenerator(std::string header);

    void generateSemanticAnalyzer(int productionId, NonTerminalSymbol
    producingSymbol, std::shared_ptr<GrammarSymbol::Production>);
private:
    std::ofstream outStream;
    // Maps names of non-terminals to their structure definitions
    std::unordered_map<std::string, std::string> nonTerminalsStructs;

    void writeHeader(std::string header);
};


#endif //SEMANTIC_ANALYZER_GENERATOR_SEMANTICANALYZERGENERATOR_H
