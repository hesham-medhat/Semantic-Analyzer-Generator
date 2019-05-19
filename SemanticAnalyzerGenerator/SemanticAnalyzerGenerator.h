#ifndef SEMANTIC_ANALYZER_GENERATOR_SEMANTICANALYZERGENERATOR_H
#define SEMANTIC_ANALYZER_GENERATOR_SEMANTICANALYZERGENERATOR_H

#include "../ParserGenerator/GrammarSymbol.h"
#include "../ParserGenerator/NonTerminalSymbol.h"
#include <fstream>
#include <string>
#include <unordered_map>

class SemanticAnalyzerGenerator {
public:
  explicit SemanticAnalyzerGenerator(std::string header);

  void generateSemanticAnalyzer(int productionId,
                                NonTerminalSymbol producingSymbol,
                                std::shared_ptr<GrammarSymbol::Production>,
                                std::vector<std::string>);
  void makeFactory();

  ~SemanticAnalyzerGenerator();

private:
  std::ofstream outStream;
  int productionCounter = 0;
};

#endif // SEMANTIC_ANALYZER_GENERATOR_SEMANTICANALYZERGENERATOR_H
