#include "SemanticAction.h"

SemanticAction::SemanticAction(std::string name)
: GrammarSymbol(std::move(name)){ }


GrammarSymbol::Type SemanticAction::getType() {
    return GrammarSymbol::Type::SemanticAction;
}

void SemanticAction::setSemanticAnalyzer(std::shared_ptr<SemanticAnalyzer>
        semanticAnalyzerIn) {
    semanticAnalyzer = std::move(semanticAnalyzerIn);
}

void SemanticAction::execute(std::string input) {
    semanticAnalyzer->execute(std::move(input));
}
