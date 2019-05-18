#include "SemanticAction.h"

SemanticAction::SemanticAction()
: GrammarSymbol(SemanticAction::RESERVED_NAME){}

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
