//
// Created by Ahmed on 5/17/2019.
//
struct x{
    int i;
    int j;
};

#include <GrammarSymbol.h>
#include "SemanticAnalyzer.h"
class temps: SemanticAnalyzer{
    std::string _input;
    void tempfun(){

        std::shared_ptr<GrammarSymbol::Production> production;
            x y;
            structs.push_back(&y);
    }

    void execute(std::string _input){
      x y;
      y.i=0;
      y.j=1;
    }


};
