#ifndef SEMANTIC_ANALYZER_GENERATOR_SEMANTICANALYZER_H
#define SEMANTIC_ANALYZER_GENERATOR_SEMANTICANALYZER_H


#include <string>
#include <unordered_map>
#include <deque>
#include <memory>

/* Considered as base class for semantic analyzers for single productions
 * executing their semantic actions and holding their data */
class SemanticAnalyzer {
public:
    // Execute the next semantic action; which should be nothing in this base
    // class case, and increment the iterator curAction to point
    void execute(std::string);
protected:
    std::deque<std::shared_ptr<void (std::string)>> actions;
    // Iterator pointing to current action
    std::deque<std::shared_ptr<void (std::string)>>::iterator curAction;

    // Structure map mapping name to its struct object pointer
    std::unordered_map<std::string, void*> map;
};


#endif //SEMANTIC_ANALYZER_GENERATOR_SEMANTICANALYZER_H
