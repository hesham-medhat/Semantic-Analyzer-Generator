#include "SemanticAction.h"

SemanticAction::SemanticAction(std::string name)
: GrammarSymbol(std::move(name)){ }


GrammarSymbol::Type SemanticAction::getType() {
    return GrammarSymbol::Type::SemanticAction;
}

void SemanticAction::setSemanticAnalyzer(std::shared_ptr<SemanticAnalyzer>
        semanticAnalyzerIn) {
    semanticAnalyzer = semanticAnalyzerIn;
}

void SemanticAction::execute() {
    semanticAnalyzer->execute();
}
