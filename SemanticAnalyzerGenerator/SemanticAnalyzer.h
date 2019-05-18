#ifndef SEMANTIC_ANALYZER_GENERATOR_SEMANTICANALYZER_H
#define SEMANTIC_ANALYZER_GENERATOR_SEMANTICANALYZER_H


#include <string>
#include <unordered_map>
#include <deque>
#include <memory>
#include <vector>

/* Considered as base class for semantic analyzers for single productions
 * executing their semantic actions and holding their data */
class SemanticAnalyzer {
public:
    // Takes the producing non-terminal's struct object. Not implemented as this
    // is an abstract class that should not be initialized
    /* explicit SemanticAnalyzer(void* producingObject); */

    // Execute the next semantic action; which should be nothing in this base
    // class case, and increment the iterator curAction to point
    virtual void execute(std::string);
    void* getNextNonTerminal();
    std::string _input;
protected:

    // Structure map mapping name to its struct object pointer
    std::vector<void *> structs;
private:
    int nonTerminalCounter=0;
};


#endif //SEMANTIC_ANALYZER_GENERATOR_SEMANTICANALYZER_H
