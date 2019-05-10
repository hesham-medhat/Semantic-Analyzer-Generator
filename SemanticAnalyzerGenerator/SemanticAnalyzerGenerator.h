#ifndef SEMANTIC_ANALYZER_GENERATOR_SEMANTICANALYZERGENERATOR_H
#define SEMANTIC_ANALYZER_GENERATOR_SEMANTICANALYZERGENERATOR_H


#include <string>
#include <unordered_map>
#include <fstream>

class SemanticAnalyzerGenerator {
public:
    explicit SemanticAnalyzerGenerator(std::unordered_map<std::string,
            std::string>&);

    void generateSemanticAnalyzer(int productionId, std::string production);
private:
    std::ofstream outStream;
    // Maps names of non-terminals to their structure definitions
    std::unordered_map<std::string, std::string> nonTerminalsStructs;

    void writeNTStructureDefinitions();
};


#endif //SEMANTIC_ANALYZER_GENERATOR_SEMANTICANALYZERGENERATOR_H
