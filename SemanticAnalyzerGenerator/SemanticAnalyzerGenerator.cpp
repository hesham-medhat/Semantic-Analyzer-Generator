#include "SemanticAnalyzerGenerator.h"
#include "SemanticAction.h"

SemanticAnalyzerGenerator::SemanticAnalyzerGenerator(
        std::unordered_map<std::string, std::string> &nonTerminalsStructs)
        : nonTerminalsStructs(nonTerminalsStructs) {






    // TODO: Open file stream and call writeNTStructureDefinitions()
}

SemanticAnalyzerGenerator::SemanticAnalyzerGenerator(std::string header) {
    outStream.open("lex.cpp", std::ofstream::out);
    outStream << "#include \"SemanticAnalyzer.h\"\n";
    outStream << header << std::endl;
    outStream.close();

}

void SemanticAnalyzerGenerator::generateSemanticAnalyzer(int productionId, NonTerminalSymbol producingSymbol,
                                                         std::shared_ptr<GrammarSymbol::Production> production,
                                                         std::vector<std::string> functions) {
    outStream.open("lex.cpp", std::ofstream::app);
    std::string className = "production" + std::to_string(productionCounter);
    productionCounter++;
    outStream << "class " << className << ":SemanticAnalyzer {\n";
    outStream << "\tprivate int functionCounter = 0\n";
    outStream << "\t struct " << producingSymbol.getName() << " " << producingSymbol.getName() << ";\n";
    std::unordered_map<std::string, int> NonTerminalCount;
    std::unordered_map<std::string, int> TerminalCount;
    std::vector<std::shared_ptr<GrammarSymbol>> actions;
    for (auto &i : *production) {
        if (i->getType() == GrammarSymbol::Type::NonTerminal) {
            outStream << "\t " << producingSymbol.getName() << " " << producingSymbol.getName();
            if ((NonTerminalCount.find(i->getName()) == NonTerminalCount.end())) {
                outStream << "1;\n";
                NonTerminalCount.insert(std::pair<std::string, int>(i->getName(), 2));
                outStream << "\tstructs.push_back(&" << producingSymbol.getName() << "1);";

            } else {
                outStream << NonTerminalCount[i->getName()] << ";\n";
                outStream << "\tstructs.push_back(&" << producingSymbol.getName() << NonTerminalCount[i->getName()]
                          << ");\n";

                NonTerminalCount[i->getName()]++;
            }
        } else if (i->getType() == GrammarSymbol::Terminal) {
            outStream << "\t std::string " << producingSymbol.getName();
            if ((TerminalCount.find(i->getName()) == TerminalCount.end())) {
                outStream << "1;\n";
                TerminalCount.insert(std::pair<std::string, int>(i->getName(), 2));
            } else {
                outStream << TerminalCount[i->getName()] << ";\n";
                TerminalCount[i->getName()]++;
            }
        }
    }
    for (int i = 0; i < functions.size(); i++) {
        outStream << "\tvoid f" << i << "() {\n";
        outStream << functions[i] << "\n}\n";
    }
    outStream << "\tvoid execute ( std::string _input ) {\n";
    outStream << "\t\tthis->_input=_input";
    outStream << "\t\tswitch (functionCounter) {\n";
    for (int i = 0; i < functions.size(); i++) {
        outStream << "\t\t\tcase " << i << ": {f" << i << "();break;\n";
    }
    outStream << "\t\t\tdefault: {break;}\n\t\t}";
    outStream << "\t\tfunctionCounter++;\n\t\t}";
    outStream << "\n\t}\n}";
    outStream.close();
}
