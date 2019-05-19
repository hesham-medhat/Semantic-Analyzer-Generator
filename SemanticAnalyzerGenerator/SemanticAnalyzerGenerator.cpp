#include "SemanticAnalyzerGenerator.h"
#include "SemanticAction.h"

SemanticAnalyzerGenerator::SemanticAnalyzerGenerator(std::string header) {
  outStream.open("GeneratedSemanticAnalyzer.h");
  outStream << "#include \"SemanticAnalyzerGenerator/SemanticAnalyzer.h\"\n";
  outStream
      << "#include "
         "\"SemanticAnalyzerGenerator/AbstractSemanticAnalyzerFactory.h\"\n";
  outStream << header << std::endl;
}

void SemanticAnalyzerGenerator::generateSemanticAnalyzer(
    int productionId, NonTerminalSymbol producingSymbol,
    std::shared_ptr<GrammarSymbol::Production> production,
    std::vector<std::string> functions) {
  std::string className = "production" + std::to_string(productionCounter);
  productionCounter++;
  outStream << "class " << className << ":public SemanticAnalyzer {\n";
  outStream << "\tint functionCounter = 0;\n";
  outStream << "\t struct " << producingSymbol.getName() << " "
            << producingSymbol.getName() << ";\n";
  std::unordered_map<std::string, int> NonTerminalCount;
  std::unordered_map<std::string, int> TerminalCount;
  std::vector<std::string> nonTerminals;
  for (auto &i : *production) {
    if (i->getType() == GrammarSymbol::Type::NonTerminal) {
      outStream << "\tstruct " << i->getName() << " " << i->getName();
      if ((NonTerminalCount.find(i->getName()) == NonTerminalCount.end())) {
        outStream << "1;\n";
        NonTerminalCount.insert(std::pair<std::string, int>(i->getName(), 2));
        nonTerminals.push_back(i->getName() + "1");
        // outStream << "\tstructs.push_back(&" << i->getName() << "1);\n";

      } else {
        outStream << NonTerminalCount[i->getName()] << ";\n";
        nonTerminals.push_back(i->getName() +
                               std::to_string(NonTerminalCount[i->getName()]));
        //    outStream << "\tstructs.push_back(&" << i->getName() <<
        //    NonTerminalCount[i->getName()]
        //              << ");\n";

        NonTerminalCount[i->getName()]++;
      }
    } else if (i->getType() == GrammarSymbol::Terminal) {
      if (i->getName().empty())
        continue;
      outStream << "\t std::string " << i->getName();
      if ((TerminalCount.find(i->getName()) == TerminalCount.end())) {
        outStream << "1;\n";
        TerminalCount.insert(std::pair<std::string, int>(i->getName(), 2));
      } else {
        outStream << TerminalCount[i->getName()] << ";\n";
        TerminalCount[i->getName()]++;
      }
    }
  }
  outStream << "public:\t" << className << " (void* parent){\n\t\t";
  outStream << "if(parent!= nullptr)\n\t\t\t";
  outStream << producingSymbol.getName() << "=*(struct "
            << producingSymbol.getName() << "*) parent;\n";
  for (std::string x : nonTerminals) {
    outStream << "\t\tstructs.push_back(&" << x << ");\n";
  }

  outStream << "\t}\n";
  for (int i = 0; i < functions.size(); i++) {
    outStream << "\tvoid f" << i << "() {\n";
    outStream << functions[i] << "\n}\n";
  }
  outStream << "\tvoid execute ( std::string _input ) {\n";
  outStream << "\t\tthis->_input=_input;\n";
  outStream << "\t\tswitch (functionCounter) {\n";
  for (int i = 0; i < functions.size(); i++) {
    outStream << "\t\t\tcase " << i << ": {f" << i << "();break;}\n";
  }
  outStream << "\t\t\tdefault: {break;}\n\t\t}";
  outStream << "\n\t\tfunctionCounter++;\n\t\t}";
  outStream << "\n};\n";
}

void SemanticAnalyzerGenerator::makeFactory() {
  outStream << "class SemanticAnalyzerFactory: public "
               "AbstractSemanticAnalyzerFactory {\n";
  outStream << "\t std::shared_ptr<SemanticAnalyzer> getSemanticAnalyzer(int "
               "productionId, void * parent) {\n";
  outStream << "\tswitch (productionId){\n";
  for (int i = 0; i < productionCounter; i++) {
    outStream << "\t\tcase " << i << ":return std::make_shared<production" << i
              << ">(parent);\n";
  }
  outStream << "\t}\n}\n};";
}

SemanticAnalyzerGenerator::~SemanticAnalyzerGenerator() { outStream.close(); }
