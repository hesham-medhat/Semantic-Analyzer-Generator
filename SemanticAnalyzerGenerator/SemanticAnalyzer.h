#ifndef SEMANTIC_ANALYZER_GENERATOR_SEMANTICANALYZER_H
#define SEMANTIC_ANALYZER_GENERATOR_SEMANTICANALYZER_H


#include <string>
#include <unordered_map>

class SemanticAnalyzer {
public:
    // Execute the next semantic action which should be nothing in this base
    // class case
    virtual void execute();
protected:
    // Next semantic action
    int semanticActionIndex = 0;

    // Structure map mapping name to its struct object pointer
    std::unordered_map<std::string, void*> map;
};


#endif //SEMANTIC_ANALYZER_GENERATOR_SEMANTICANALYZER_H
