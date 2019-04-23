#ifndef PARSER_GENERATOR_PARSERGENERATOR_H
#define PARSER_GENERATOR_PARSERGENERATOR_H


#include "../LexicalAnalysis/LexicalAnalyzer.h"
#include "Parser.h"

/* Utility class that is used to generate the parser given input
 * stream of grammar rules and their lexical analyzer */
class ParserGenerator {
public:
    /// Generates LL(1) parser using rules from given input stream.
    ///
    /// @param  rulesIstream  stream to read rules from
    /// @param  lex           LexicalAnalyzer to use in generated parser
    ///
    /// @return  generated Parser instance
    static Parser generateParser(std::istream& rulesIstream,
                                 LexicalAnalyzer& lex);

private:
    ParserGenerator(); // Not implemented on purpose
    static constexpr char eof = std::string::traits_type::eof();
    static const std::string reserved;
    static bool skip(std::istream&, const std::string& = "");
    static std::string getUntil(std::istream&, const std::string&);
    static GrammarSymbol::Production getProduction(
        std::istream&,
        std::unordered_map<std::string, TerminalSymbol::ptr>&,
        std::unordered_map<std::string, NonTerminalSymbol::ptr>&);
    static inline bool isValidSymbolName(const std::string&);
    static inline bool contains(const std::string&, const char&);
    template<typename K, typename V>
    static inline bool contains(const std::unordered_map<K, V>&, const K&);
    static void removeLeftRecursion(Parser&);
    static void leftFactoring(Parser&);
};


#endif //PARSER_GENERATOR_PARSERGENERATOR_H
