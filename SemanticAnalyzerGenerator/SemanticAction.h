#ifndef SEMANTIC_ANALYZER_GENERATOR_SEMANTICACTION_H
#define SEMANTIC_ANALYZER_GENERATOR_SEMANTICACTION_H


#include "../ParserGenerator/GrammarSymbol.h"
#include "SemanticAnalyzer.h"

static const std::string RESERVED_ACTION_NAME = "___ACTION___";

class SemanticAction : public GrammarSymbol {
public:
    SemanticAction();

    Type getType() override;

    void setSemanticAnalyzer(std::shared_ptr<SemanticAnalyzer>);

    void execute(std::string input);
private:
    std::shared_ptr<SemanticAnalyzer> semanticAnalyzer;
};


#endif //SEMANTIC_ANALYZER_GENERATOR_SEMANTICACTION_H
