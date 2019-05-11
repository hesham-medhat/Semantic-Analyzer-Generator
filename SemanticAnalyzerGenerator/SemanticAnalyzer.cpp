#include "SemanticAnalyzer.h"

void SemanticAnalyzer::execute(std::string input) {
    (**curAction++)(std::move(input));
}
