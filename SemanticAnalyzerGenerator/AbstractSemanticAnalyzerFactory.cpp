#include "AbstractSemanticAnalyzerFactory.h"

std::shared_ptr<SemanticAnalyzer> AbstractSemanticAnalyzerFactory::getSemanticAnalyzer(int productionId, void * parent) {

    switch (productionId){
        case 0:return std::make_shared<production0>(parent);
    }
    return std::shared_ptr<SemanticAnalyzer>();
}
