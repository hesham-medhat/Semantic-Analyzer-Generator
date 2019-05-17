#ifndef SEMANTIC_ANALYZER_GENERATOR_ABSTRACTSEMANTICANALYZERFACTORY_H
#define SEMANTIC_ANALYZER_GENERATOR_ABSTRACTSEMANTICANALYZERFACTORY_H


#include "SemanticAnalyzer.h"

class AbstractSemanticAnalyzerFactory {
public:
    virtual std::shared_ptr<SemanticAnalyzer> getSemanticAnalyzer(int
    productionId);
};


#endif //SEMANTIC_ANALYZER_GENERATOR_ABSTRACTSEMANTICANALYZERFACTORY_H
