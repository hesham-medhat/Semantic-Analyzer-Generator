#include "LexicalAnalyzerGenerator.h"
#include <stack>
#include <queue>
#include <string.h>

std::unordered_map<std::string, Automaton *> automatons;
std::unordered_set<std::string> keywords;
std::unordered_set<char> punctuation;
std::stack<Automaton *> operands;
std::stack<char> operators;
Automaton automaton = *new Automaton('\0');

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
                Automaton a = *createExpAutomaton(LHS, RHS);
                automatons.insert(std::make_pair(LHS, &a));
                break;
            }
            case 3:     //definition
            {
                unsigned int pos = token.find(':');
                std::string LHS = token.substr(0, pos - 1);
                std::string RHS = token.substr(pos + 1);
                std::string s = "definition";
                Token t = *new Token(s);
                automaton.unionOp(*createDefAutomaton(LHS, RHS), t);


                break;
            }

            default:
                break;
        }
        std::getline(inputStream, token);

    }


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
    Token t = *new Token(s);
    strcpy(temp, token.c_str());
    char *word;
    word = strtok(temp, " \t");
    while (word != NULL) {
        keywords.insert(word);

        Automaton *start = getAutomatonForWord("keyword", word);
        /*int j = 0;

        while (word[++j] != '\0') {
            Automaton next = *new Automaton(word[j]);

            start.concatenateOp(next, t);
        }*/
        if (automatons.find(s) != automatons.end()) {
            Automaton z = *automatons.at(s);
            z.unionOp(*start, t);
            automatons.insert(std::make_pair(s, &z));
        } else {
            automatons[s] = start;
        }
        word = strtok(NULL, " \t");
    }

}

void LexicalAnalyzerGenerator::updatePunctuations(std::string token) {
    std::string temp;
    for (int i = 0; i < token.size(); i++) {
        if (token[i] == '\\')
            punctuation.insert((token[++i]));
        else
            punctuation.insert(token[i]);
    }

}

Automaton *LexicalAnalyzerGenerator::createExpAutomaton(std::string tokenName, std::string token) {
    bool prevIsOperand = false;
    operands = *new std::stack<Automaton *>();
    operators = *new std::stack<char>();
    for (int i = 0; i < token.size(); i++) {
        if (token[i] == ' ') continue;
        if (token[i] == '\\') {
            if (token[++i] == 'L')
                operands.push(new Automaton(0));
            else
                operands.push(new Automaton(token[i]));
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
            operands.push(new Automaton(start, end));
            prevIsOperand = true;

        } else {
            if (prevIsOperand)
                operators.push('.');
            prevIsOperand = true;
            operands.push(new Automaton(token[i]));
        }
    }
    while (!operators.empty()) {
        performOp(tokenName);
    }
    return operands.top();
}

void LexicalAnalyzerGenerator::performOp(std::string tokenName) {
    Automaton a1 = *operands.top();
    operands.pop();
    Token t = *new Token(tokenName);
    if (operators.top() == '*') {
        a1.kleeneClosureOp(t);
        operands.push(&a1);
    } else if (operators.top() == '+') {
        a1.positiveClosureOp(t);
        operands.push(&a1);
    } else {
        Automaton a2 = *operands.top();
        operands.pop();
        if (operators.top() == '.') {
            a2.concatenateOp(a1, t);
            operands.push(&a2);
        } else if (operators.top() == '|') {
            a2.unionOp(a1, t);
            operands.push(&a2);
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

Automaton *LexicalAnalyzerGenerator::createDefAutomaton(std::string name, std::string token) {
    operands = *new std::stack<Automaton *>();
    operators = *new std::stack<char>();

    std::string c;
    Token t = *new Token(name);
    bool prevIsOperand = false;

    for (int i = 0; i < token.size(); i++) {
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
                    operands.push(new Automaton(0));
                else
                    operands.push(new Automaton(token[i]));
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
                operands.push(new Automaton(start, end));
                prevIsOperand = true;

            } else {
                if (prevIsOperand)
                    operators.push('.');
                prevIsOperand = true;
                operands.push(new Automaton(token[i]));
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

Automaton *LexicalAnalyzerGenerator::getAutomatonForWord(std::string name, std::string token) {
    Automaton a = *new Automaton('\0');
    Token t = *new Token(name);
    for (char c:token) {
        a.concatenateOp(*new Automaton(c), t);
    }
    return &a;
}
