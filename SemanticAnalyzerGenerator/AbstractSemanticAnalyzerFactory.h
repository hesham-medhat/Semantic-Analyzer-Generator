#ifndef SEMANTIC_ANALYZER_GENERATOR_ABSTRACTSEMANTICANALYZERFACTORY_H
#define SEMANTIC_ANALYZER_GENERATOR_ABSTRACTSEMANTICANALYZERFACTORY_H

#include "SemanticAnalyzer.h"
#include "lex.h"
class AbstractSemanticAnalyzerFactory {
public:
    virtual std::shared_ptr<SemanticAnalyzer> getSemanticAnalyzer(int
    productionId,void *);
};


#endif //SEMANTIC_ANALYZER_GENERATOR_ABSTRACTSEMANTICANALYZERFACTORY_H
