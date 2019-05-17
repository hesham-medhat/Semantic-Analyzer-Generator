#include <regex>
#include "ParserGenerator.h"

const std::string ParserGenerator::reserved = "#=|'\\";
const std::string ParserGenerator::whitespaces = " \f\n\r\t\v";

Parser ParserGenerator::generateParser(std::istream &rulesIstream,
                                       LexicalAnalyzer &lex) {
    if (!rulesIstream) {
        throw std::invalid_argument("invalid stream passed to generateParser()");
    }

    NonTerminalSymbol::ptr startingSymbol;
    std::unordered_map<std::string, TerminalSymbol::ptr> terminals;
    std::unordered_map<std::string, NonTerminalSymbol::ptr> nonTerminals;

    // Parse initial code block
    std::string initialCodeBlock;
    if (skip(rulesIstream, "$")) {
      initialCodeBlock = getUntil(rulesIstream, "$");
    }

    if (!rulesIstream) {
      throw std::runtime_error("no rules in file");
    }

    // Parse next rule
    while (rulesIstream) {
        if (!skip(rulesIstream, "#")) {
            throw std::runtime_error("expected '#' at the beginning of rule");
        }

        // Extract LHS
        std::string lhs = getUntil(rulesIstream, whitespaces + "=");
        if (!isValidSymbolName(lhs)) {
          throw std::runtime_error(
              std::string("invalid symbol name in LHS: ") + lhs);
        }

        NonTerminalSymbol::ptr symbol;
        if (contains(nonTerminals, lhs)) {
            symbol = nonTerminals[lhs];
        } else {
            symbol = std::make_shared<NonTerminalSymbol>(lhs);
            nonTerminals[lhs] = symbol;
            if (!startingSymbol) { startingSymbol = symbol; }
        }

        // Skip over "="
        if (!skip(rulesIstream, "=")) {
            throw std::runtime_error("expected '=' after LHS");
        }

        // Extract RHS
        GrammarSymbol::Production prod =
                getProduction(rulesIstream, terminals, nonTerminals);
        if (prod.empty()) {
            throw std::runtime_error(
                    std::string("expected at least one production in rule: ") + lhs);
        }
        symbol->addProduction(prod);
        while (skip(rulesIstream, "|")) {
            prod = getProduction(rulesIstream, terminals, nonTerminals);
            if (prod.empty()) {
                throw std::runtime_error("expected production after '|'");
            }
            symbol->addProduction(prod);
        }
    }



    Parser parser(lex, startingSymbol, terminals, nonTerminals);
    removeLeftRecursion(parser);
    leftFactoring(parser);

    for (const auto& nonTerminal : parser.nonTerminals) {
        for (const auto& prod : nonTerminal.second->productions) {
            for (const auto& term : prod) {
                if (term->getType() == GrammarSymbol::NonTerminal) {
                    NonTerminalSymbol::ptr termPtr =
                            std::dynamic_pointer_cast<NonTerminalSymbol>(term);
                    termPtr->addUsingProduction(nonTerminal.second, prod);
                }
            }
        }
    }

    for (const auto& nonTerminal : parser.nonTerminals) {
        nonTerminal.second->getFirst(std::unordered_set<std::string>());
        nonTerminal.second->firstCalculated = true;
    }

    for (const auto& nonTerminal : parser.nonTerminals) {
        nonTerminal.second->getFollow(std::unordered_set<std::string>());
        nonTerminal.second->followCalculated = true;
    }

    return parser;
}

bool ParserGenerator::skip(std::istream &is, const std::string &str) {
    while (std::isspace(is.peek())) { is.get(); }
    std::istream::pos_type originalPos = is.tellg();
    for (const char &current : str) {
        if (is.peek() != current) {
          is.seekg(originalPos);
          return false;
        }
        is.seekg(1, std::ios::cur);
    }
    return true;
}

std::string ParserGenerator::getUntil(std::istream& is,
                                      const std::string& delim) {
  std::string input;
  skip(is);
  char next = is.peek();
  while (next != eof && !contains(delim, next)) {
    if (next == '\\') {
      is.get();
      next = is.peek();
      if (!contains(reserved, next)) {
        throw std::runtime_error(
            std::string("unrecognized escape sequence: ") + "\\" + next);
      }
    }
    input += is.get();
    next = is.peek();
  }
  return input;
}

GrammarSymbol::Production ParserGenerator::getProduction(
    std::istream& is,
    std::unordered_map<std::string, TerminalSymbol::ptr>& terminals,
    std::unordered_map<std::string, NonTerminalSymbol::ptr>& nonTerminals) {
  constexpr char rhsTermDelim[] = "#|";
  GrammarSymbol::Production prod;
  if (skip(is, "'\\L'") && skip(is)) {
    if (!contains(rhsTermDelim, is.peek())) {
      throw std::runtime_error("\\L production should not have other symbols");
    }
    TerminalSymbol::ptr epsilon =
      contains(terminals, std::string(""))
      ? terminals[""]
      : terminals[""] = std::make_shared<TerminalSymbol>("");
    prod.push_back(epsilon);
  } else if (skip(is), "{") {
    std::string codeFragment = getUntil(is, "}");
    if (!is) {
      throw std::runtime_error("premature end of file, expected '}'");

    }
    // TODO(ahmed127011): write to parser source (create SemanticAction, etc..)
  } else {
    std::string rhsTerm = getUntil(is, whitespaces + rhsTermDelim);
    while (!rhsTerm.empty()) {
      if (isValidSymbolName(rhsTerm)) {
        NonTerminalSymbol::ptr nonTermSymbol =
          contains(nonTerminals, rhsTerm)
          ? nonTerminals[rhsTerm]
          : nonTerminals[rhsTerm] =
              std::make_shared<NonTerminalSymbol>(rhsTerm);
        prod.push_back(nonTermSymbol);
      } else if (rhsTerm.front() == '\'' && rhsTerm.back() == '\'') {
        rhsTerm = rhsTerm.substr(1, rhsTerm.length() - 2);
        TerminalSymbol::ptr terminalSymbol =
          contains(terminals, rhsTerm)
          ? terminals[rhsTerm]
          : terminals[rhsTerm] = std::make_shared<TerminalSymbol>(rhsTerm);
        prod.push_back(terminalSymbol);
      } else {
        throw std::runtime_error(
            std::string("invalid symbol name in RHS: ") + rhsTerm);
      }
      rhsTerm = getUntil(is, whitespaces + rhsTermDelim);
    }
  }
  return prod;
}

inline bool ParserGenerator::isValidSymbolName(const std::string& str) {
  static const std::regex regexSymbolName("[_[:alpha:]][\\w&]*");
  return std::regex_match(str, regexSymbolName);
}

inline bool ParserGenerator::contains(const std::string &str, const char &c) {
    return str.find(c) != std::string::npos;
}

template<typename K, typename V>
inline bool ParserGenerator::contains(const std::unordered_map<K, V> &map,
                                      const K &key) {
    return map.find(key) != map.end();
}

void ParserGenerator::removeLeftRecursion(Parser &parser) {
    std::unordered_map<std::string, std::shared_ptr<NonTerminalSymbol>> nonTerminals = parser.nonTerminals;
    for (auto &nonTerminal : nonTerminals) {
        std::vector<GrammarSymbol::Production> production = nonTerminal.second->productions;
        std::vector<GrammarSymbol::Production> a_vector, a_dash_vector;

        for (auto &i : production) {
            if ((*(i.begin()))->getType() == GrammarSymbol::Type::NonTerminal
                && (*(i.begin()))->getName() == nonTerminal.first) {
                i.pop_front();
                a_dash_vector.push_back(i);
            } else {
                a_vector.push_back(i);
            }
        }
        if (!a_dash_vector.empty()) {
            std::shared_ptr<NonTerminalSymbol> a_dash(new NonTerminalSymbol(nonTerminal.first + "'"));

            for (auto &i : a_dash_vector) {
                i.push_back(a_dash);
            }
            GrammarSymbol::ptr epsilon = std::make_shared<TerminalSymbol>("");
            std::deque<std::shared_ptr<GrammarSymbol>> epsilonProduction;
            epsilonProduction.push_back(epsilon);
            a_dash_vector.push_back(epsilonProduction);
            for (auto &i : a_vector) {
                i.push_back(a_dash);
            }
            a_dash->productions = a_dash_vector;
            nonTerminal.second->productions = a_vector;
            parser.nonTerminals[a_dash->getName()] = a_dash;
        }


    }

}

void ParserGenerator::leftFactoring(Parser &parser) {
    bool stillFactoring;
    do {
        stillFactoring = false;
        for (auto &nonTerminal : parser.nonTerminals) {
            std::vector<GrammarSymbol::Production> production = nonTerminal.second->productions;
            std::vector<GrammarSymbol::Production> betas;
            for (int i = 0; i < production.size(); i++) {
                betas.clear();
                for (int j = i + 1; j < production.size(); j++) {
                    if ((*production[i].begin())->getName() == (*production[j].begin())->getName()) {
                        production[j].pop_front();
                        if (production[j].empty()) {
                            GrammarSymbol::ptr epsilon = std::make_shared<TerminalSymbol>("");
                            std::deque<std::shared_ptr<GrammarSymbol>> epsilonProduction;
                            epsilonProduction.push_back(epsilon);
                            betas.push_back(epsilonProduction);
                        } else {
                            betas.push_back(production[j]);
                        }
                        production.erase(production.begin() + j);
                    }
                }
                if (!betas.empty()) {
                    stillFactoring = true;
                    GrammarSymbol::Production temp = production[i];
                    temp.pop_front();
                    if (temp.empty()) {
                        GrammarSymbol::ptr epsilon = std::make_shared<TerminalSymbol>("");
                        std::deque<std::shared_ptr<GrammarSymbol>> epsilonProduction;
                        epsilonProduction.push_back(epsilon);
                        betas.push_back(epsilonProduction);
                    } else {
                        betas.push_back(temp);
                    }
                    while (production[i].size() > 1) {
                        production[i].pop_back();
                    }
                    std::shared_ptr<NonTerminalSymbol> a_dash(new NonTerminalSymbol(nonTerminal.first + "'"));
                    a_dash->productions = betas;
                    parser.nonTerminals[a_dash->getName()] = a_dash;
                    production[i].push_back(a_dash);

                }

            }
            nonTerminal.second->productions = production;


        }
    } while (stillFactoring);

}
