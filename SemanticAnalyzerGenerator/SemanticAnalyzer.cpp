#include "SemanticAnalyzer.h"


void *SemanticAnalyzer::getNextNonTerminal() {
    return structs[nonTerminalCounter++];
}
