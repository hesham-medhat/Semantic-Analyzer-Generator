#include "LexicalAnalyzerGenerator.h"
#include "Automata/StateBuilder.h"
#include <stack>
#include <queue>
#include <string.h>

std::unordered_map<std::string, std::shared_ptr<Automaton>> automatons;
std::unordered_set<std::string> keywords;
std::unordered_set<char> punctuation;
std::stack<std::shared_ptr<Automaton>> operands;
std::stack<char> operators;
Automaton automaton = Automaton('\0');

LexicalAnalyzerGenerator::LexicalAnalyzerGenerator(std::istream &inputStream)
        : inputStream(inputStream) {
    std::string token;
    std::getline(inputStream, token);
    while (!token.empty()) {
        int format = getFormat(token);
        switch (format) {
            case 0:     //keywords
            {
                updateKeywords(token.substr(1, token.size() - 2));
                break;
            }
            case 1:     //punctuation
            {
                updatePunctuations(token.substr(1, token.size() - 2));
                break;
            }
            case 2:     //expression
            {
                unsigned int pos = token.find('=');
                std::string LHS = token.substr(0, (pos - 1));
                std::string RHS = token.substr(pos + 1);
                std::shared_ptr<Automaton>a = createExpAutomaton(LHS, RHS);
                automatons.insert(std::make_pair(LHS, a));
                break;
            }
            case 3:     //definition
            {
                unsigned int pos = token.find(':');
                std::string LHS = token.substr(0, pos - 1);
                std::string RHS = token.substr(pos + 1);
                std::string s = "definition";
                Token t = Token(s);
                automaton.unionOp(*createDefAutomaton(LHS, RHS), t);


                break;
            }

            default:
                break;
        }
        std::getline(inputStream, token);

    }


}


using namespace std;

unordered_set<shared_ptr<State>> getLambdaClosure(shared_ptr<State> state){

    //push all states of T onto stack;
    unordered_set<shared_ptr<State>>  states;
    states = state->getNextState(0);
    stack<shared_ptr<State>> s;
    unordered_set<shared_ptr<State>>  result;
    result.insert(state);
    for (const auto& elem: states) {
        s.push(elem);
        result.insert(elem);
    }
    while (s.size() != 0) {
        shared_ptr<State> temp = s.top();
        s.pop();
        unordered_set<shared_ptr<State>>  near;
        near = temp->getNextState(0);
        for (const auto& elem: near) {
            if(result.find(elem) == result.end()){
                s.push(elem);
                result.insert(elem);
            }
        }
    }
    return result;
}

Automaton minimizeDFA(unordered_set<shared_ptr<State>> AllDFAStates) {
    vector<unordered_set<shared_ptr<State>>> groupSet;

    for (auto &originalState: AllDFAStates) {
        if (!groupSet.empty()) {
            bool noMatch = true;
            for (auto &group: groupSet) {
                for (auto &state: group) {
                    if (state->getAcceptedToken().getType() == originalState->getAcceptedToken().getType()) {
                        group.insert(originalState);
                        noMatch = false;
                        break;
                    }
                }
                if(!noMatch){
                    break;
                }
            }
            if(noMatch) {
                unordered_set<shared_ptr<State>> newGroup;
                newGroup.insert(originalState);
                groupSet.push_back(newGroup);
            }
        } else {
            unordered_set<shared_ptr<State>> newGroup;
            newGroup.insert(originalState);
            groupSet.push_back(newGroup);
        }
    }



    bool noSplit = false;

    while (!noSplit) {
        vector<unordered_set<shared_ptr<State>>> newGroupSet;
        for (auto &group: groupSet) {
            unordered_set<shared_ptr<State>> newGroup;
            for (auto &state: group) {
                newGroup.insert(state);
                for (auto &otherState: group) {
                    if (state != otherState) {
                        bool isMatch = true;
                        vector<char> vector1;
                        vector1.push_back('i');
                        vector1.push_back('f');
                        vector1.push_back('e');
                        vector1.push_back('l');
                        vector1.push_back('s');
                        for(const auto& i: vector1 ){
                        //for (int i = 1; i < 256 && isMatch; i++) {
                            unordered_set<shared_ptr<State>> transition1 = state->getNextState(i);
                            unordered_set<shared_ptr<State>> transition2 = otherState->getNextState(i);
                            if (transition1.size() != transition2.size()) {
                                isMatch = false;
                                break;
                            }
                            if (transition1.size() != 0) {
                                for (auto &nextSate: transition1) {
                                    for (auto &otherNextState: transition2) {
                                        if (nextSate != otherNextState) {
                                            isMatch = false;
                                        }
                                    }
                                }
                            }
                            if(!isMatch){
                                break;
                            }
                        }
                        if (isMatch) {
                            newGroup.insert(otherState);
                            group.erase(otherState);
                        }
                    }
                }
                newGroupSet.push_back(newGroup);
            }
            //newGroupSet.push_back(newGroup);
        }
        if(newGroupSet.size() == groupSet.size()){
            noSplit = true;
        } else {
            groupSet = newGroupSet;
        }
    }
    Automaton a;
    return a;
}

Automaton LexicalAnalyzerGenerator::convertNFAToDFA(Automaton NFA){
    //unordered_set<pair<shared_ptr<State>, unordered_set<shared_ptr<State>>>> DFAPairSet;
    vector<pair<shared_ptr<State>, unordered_set<shared_ptr<State>>>> DFAPairSet;
    unordered_set<shared_ptr<State>> AllDFAStates;


    unordered_set<shared_ptr<State>> NFAStartEquivalents = getLambdaClosure(NFA.startState);
    shared_ptr<State> DFAStart = StateBuilder::buildState("DFA","");
    for(const auto&  newState: NFAStartEquivalents){
        //still need to find most priority token
        if(newState->getAcceptedToken().getType() != ""){
            DFAStart = StateBuilder::buildState("DFA", newState->getAcceptedToken().getType());
            break;
        }
    }
    Automaton DFA;
    DFA.startState = DFAStart;
    AllDFAStates.insert(DFAStart);

    pair<shared_ptr<State>, unordered_set<shared_ptr<State>>> pair1(DFAStart,NFAStartEquivalents);
    DFAPairSet.push_back(pair1);
    //DFAPairSet.insert(pair1);

    stack<pair<shared_ptr<State>, unordered_set<shared_ptr<State>>>> DFAStack;
    DFAStack.push(pair1);
    int counter = 1;
    while (!DFAStack.empty()){

        pair<shared_ptr<State>, unordered_set<shared_ptr<State>>> CurrentDFAPair =DFAStack.top();
        unordered_set<shared_ptr<State>> currentSet = CurrentDFAPair.second;
        DFAStack.pop();

        //int i =105;
        //vector<char> vector1;
        //vector1.push_back('i');
        //vector1.push_back('f');
        for(int i = 1; i < 256; i++){
            //for(const auto& i: vector1 ){
            unordered_set<shared_ptr<State>> newSet;
            for(const auto& NFAState : currentSet){
                unordered_set<shared_ptr<State>> nextSet = NFAState->getNextState(i);
                for(const auto& nextState : nextSet){
                    newSet.insert(nextState);
                    unordered_set<shared_ptr<State>> newSetEquivalents = getLambdaClosure(nextState);
                    for(const auto& nextStateEq : newSetEquivalents){
                        newSet.insert(nextStateEq);
                    }
                }
            }
            bool allFonded = false;
            for(const auto&  DFAPair: DFAPairSet){
                if(newSet.size() == DFAPair.second.size()){
                    for(const auto&  newState: newSet){
                        bool founded = false;
                        for(const auto&  currentState: DFAPair.second){
                            if(newState == currentState){
                                founded = true;
                                break;
                            }
                        }
                        allFonded &= founded;
                    }
                }
                if(allFonded ){
                    CurrentDFAPair.first->addTransition(i,DFAPair.first);
                    break;
                }
            }
            if(!allFonded && newSet.size() != 0){
                //still need to find most priority token
                shared_ptr<State> newDFAState = StateBuilder::buildState("DFA","");
                for(const auto&  newState: newSet){
                    if(newState->getAcceptedToken().getType() != ""){
                        newDFAState = StateBuilder::buildState("DFA", newState->getAcceptedToken().getType());
                        break;
                    }
                }
                AllDFAStates.insert(newDFAState);
                pair<shared_ptr<State>, unordered_set<shared_ptr<State>>> newPair(newDFAState,newSet);
                DFAPairSet.push_back(newPair);
                DFAStack.push(newPair);
                CurrentDFAPair.first->addTransition(i,newPair.first);
            }
        }
    }
    minimizeDFA(AllDFAStates);
    return DFA;
}


int LexicalAnalyzerGenerator::getFormat(std::string token) {
    char temp = token[0];
    if (temp == '{') return 0;
    if (temp == '[') return 1;
    int i = -1;
    while (++i) {
        if (token[i] == '=') return 2;
        if (token[i] == ':') return 3;
    }
    return -1;
}

void LexicalAnalyzerGenerator::updateKeywords(std::string token) {
    char *temp = new char[token.length() + 1];
    std::string s = "keyword";
    Token t = Token(s);
    strncpy(temp, token.c_str(), token.length() + 1);
    char *word;
    word = strtok(temp, " \t");
    while (word != NULL) {
        keywords.insert(word);

        std::shared_ptr<Automaton>start = getAutomatonForWord("keyword", word);
        /*int j = 0;

        while (word[++j] != '\0') {
            Automaton next = *std::make_shared<Automaton>(word[j]);

            start.concatenateOp(next, t);
        }*/
        if (automatons.find(s) != automatons.end()) {
            std::shared_ptr<Automaton> z = automatons.at(s);
            z->unionOp(*start, t);
            automatons.insert(std::make_pair(s, z));
        } else {
            automatons[s] = start;
        }
        word = strtok(NULL, " \t");
    }
  delete[] temp;
}

void LexicalAnalyzerGenerator::updatePunctuations(std::string token) {
    for (size_t i = 0; i < token.size(); i++) {
        if (token[i] == '\\')
            punctuation.insert((token[++i]));
        else
            punctuation.insert(token[i]);
    }

}

std::shared_ptr<Automaton> LexicalAnalyzerGenerator::createExpAutomaton(std::string tokenName, std::string token) {
    bool prevIsOperand = false;
    operands = std::stack<std::shared_ptr<Automaton>>();
    operators = std::stack<char>();
    for (size_t i = 0; i < token.size(); i++) {
        if (token[i] == ' ') continue;
        if (token[i] == '\\') {
            if (token[++i] == 'L')
                operands.push(std::make_shared<Automaton>(0));
            else
                operands.push(std::make_shared<Automaton>(token[i]));
            if (prevIsOperand)
                operators.push('.');
            prevIsOperand = true;

        } else if (token[i] == '(') {
            operators.push(token[i]);
            if (prevIsOperand)
                operators.push('.');
            prevIsOperand = false;

        } else if (token[i] == ')') {
            while (!operators.empty() && operators.top() != '(') {
                performOp(tokenName);
            }
            prevIsOperand = true;

        } else if (isOperation(token[i])) {
            while (!operators.empty() && precedence(operators.top()) >= precedence(token[i])) {
                performOp(tokenName);
            }
            operators.push(token[i]);
            prevIsOperand = false;

        } else if (token[i] == '-') {
            operands.pop();
            char start, end;
            while (token[--i] == ' ');
            start = token[i];
            while (token[++i] == ' ' || token[i] == '-');
            end = token[i];
            operands.push(std::make_shared<Automaton>(start, end));
            prevIsOperand = true;

        } else {
            if (prevIsOperand)
                operators.push('.');
            prevIsOperand = true;
            operands.push(std::make_shared<Automaton>(token[i]));
        }
    }
    while (!operators.empty()) {
        performOp(tokenName);
    }
    return operands.top();
}

void LexicalAnalyzerGenerator::performOp(std::string tokenName) {
    std::shared_ptr<Automaton> a1 = operands.top();
    operands.pop();
    Token t = Token(tokenName);
    if (operators.top() == '*') {
        a1->kleeneClosureOp(t);
        operands.push(a1);
    } else if (operators.top() == '+') {
        a1->positiveClosureOp(t);
        operands.push(a1);
    } else {
       std::shared_ptr<Automaton> a2 = operands.top();
        operands.pop();
        if (operators.top() == '.') {
            a2->concatenateOp(*a1, t);
            operands.push(a2);
        } else if (operators.top() == '|') {
            a2->unionOp(*a1, t);
            operands.push(a2);
        }
    }
    operators.pop();
}

bool LexicalAnalyzerGenerator::isOperation(char op) {
    return op == '|' || op == '+' || op == '*';

}

int LexicalAnalyzerGenerator::precedence(char op) {
    if (op == '.')
        return 1;
    if (op == '+' || op == '*')
        return 2;
    return 0;
}

std::shared_ptr<Automaton> LexicalAnalyzerGenerator::createDefAutomaton(std::string name, std::string token) {
    operands = std::stack<std::shared_ptr<Automaton>>();
    operators = std::stack<char>();

    std::string c;
    Token t = Token(name);
    bool prevIsOperand = false;

    for (size_t i = 0; i < token.size(); i++) {
        if (!isLetter(token[i])) {
            if (!c.empty()) {
                if (automatons.find(c) != automatons.end()) {
                    if (prevIsOperand)
                        operators.push('.');
                    prevIsOperand = true;
                    operands.push(automatons.at(c));
                } else {
                    operands.push(getAutomatonForWord(name, c));
                }
            }
            c.clear();
            if (token[i] == ' ')
                continue;
            else if (token[i] == '\\') {
                if (token[++i] == 'L')
                    operands.push(std::make_shared<Automaton>(0));
                else
                    operands.push(std::make_shared<Automaton>(token[i]));
                if (prevIsOperand)
                    operators.push('.');
                prevIsOperand = true;

            } else if (token[i] == '(') {
                operators.push(token[i]);
                if (prevIsOperand)
                    operators.push('.');
                prevIsOperand = false;
            } else if (token[i] == ')') {
                while (!operators.empty() && operators.top() != '(') {
                    performOp(name);
                }
                prevIsOperand = true;
            } else if (isOperation(token[i])) {
                while (!operators.empty() && precedence(operators.top()) >= precedence(token[i])) {
                    performOp(name);
                }
                operators.push(token[i]);
                prevIsOperand = false;
            } else if (token[i] == '-') {
                operands.pop();
                char start, end;
                while (token[--i] == ' ');
                start = token[i];
                while (token[++i] == ' ' || token[i] == '-');
                end = token[i];
                operands.push(std::make_shared<Automaton>(start, end));
                prevIsOperand = true;

            } else {
                if (prevIsOperand)
                    operators.push('.');
                prevIsOperand = true;
                operands.push(std::make_shared<Automaton>(token[i]));
            }


        } else {
            c += token[i];
        }
    }
    if (!c.empty()) {
        if (automatons.find(c) != automatons.end()) {
            if (prevIsOperand)
                operators.push('.');
            operands.push(automatons.at(c));
        } else {
            operands.push(getAutomatonForWord(name, c));
        }
    }
    while (!operators.empty()) {
        performOp(name);
    }
    return operands.top();
}

bool LexicalAnalyzerGenerator::isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

std::shared_ptr<Automaton> LexicalAnalyzerGenerator::getAutomatonForWord(std::string name, std::string token) {
    std::shared_ptr<Automaton> a = std::make_shared<Automaton>('\0');
    Token t = Token(name);
    for (char c:token) {
        a->concatenateOp(Automaton(c), t);
    }
    return a;
}

LexicalAnalyzer LexicalAnalyzerGenerator::buildLexicalAnalyzer() {
  return LexicalAnalyzer(convertNFAToDFA(automaton));
}
