#ifndef SEMANTIC_ANALYZER_GENERATOR_SEMANTICACTION_H
#define SEMANTIC_ANALYZER_GENERATOR_SEMANTICACTION_H

#include "../ParserGenerator/GrammarSymbol.h"
#include "SemanticAnalyzer.h"

class SemanticAction : public GrammarSymbol {
public:
  static constexpr const char *RESERVED_NAME = "__ACTION__";
  SemanticAction();

  Type getType() override;

  void setSemanticAnalyzer(std::shared_ptr<SemanticAnalyzer>);

  void execute(std::string input);

private:
  std::shared_ptr<SemanticAnalyzer> semanticAnalyzer;
};

#endif // SEMANTIC_ANALYZER_GENERATOR_SEMANTICACTION_H
