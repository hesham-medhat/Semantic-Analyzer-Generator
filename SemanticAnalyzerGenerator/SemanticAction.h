#ifndef SEMANTIC_ANALYZER_GENERATOR_SEMANTICACTION_H
#define SEMANTIC_ANALYZER_GENERATOR_SEMANTICACTION_H


#include "../ParserGenerator/GrammarSymbol.h"

class SemanticAction : public GrammarSymbol {
public:
    explicit SemanticAction(std::string name);

    Type getType() override;

    void setSemanticAnalyzer(std::shared_ptr<SemanticAnalyzer>);

    void execute();
private:
    std::shared_ptr<SemanticAnalyzer> semanticAnalyzer;
};


#endif //SEMANTIC_ANALYZER_GENERATOR_SEMANTICACTION_H
