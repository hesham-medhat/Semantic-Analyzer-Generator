#ifndef SEMANTIC_ANALYZER_GENERATOR_ABSTRACTSEMANTICANALYZERFACTORY_H
#define SEMANTIC_ANALYZER_GENERATOR_ABSTRACTSEMANTICANALYZERFACTORY_H

#include "SemanticAnalyzer.h"


class AbstractSemanticAnalyzerFactory {
public:
    // Not implemented on purpose as this is an abstract class
    virtual std::shared_ptr<SemanticAnalyzer> getSemanticAnalyzer(int
    productionId, void* producingObject);
};


#endif //SEMANTIC_ANALYZER_GENERATOR_ABSTRACTSEMANTICANALYZERFACTORY_H
