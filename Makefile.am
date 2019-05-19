bin_PROGRAMS = pgen parser dump_parser

lexical_analyzer_generator_src = \
  LexicalAnalysis/Token.cpp \
  LexicalAnalysis/LexicalAnalyzer.cpp \
  LexicalAnalysis/LexicalAnalyzerGenerator.cpp \
  LexicalAnalysis/Automata/State.cpp \
  LexicalAnalysis/Automata/NFAState.cpp \
  LexicalAnalysis/Automata/DFAState.cpp \
  LexicalAnalysis/Automata/StateBuilder.cpp \
  LexicalAnalysis/Automata/Automaton.cpp

parser_generator_src = \
  ParserGenerator/GrammarSymbol.cpp \
  ParserGenerator/NonTerminalSymbol.cpp \
  ParserGenerator/TerminalSymbol.cpp \
  ParserGenerator/Parser.cpp \
  ParserGenerator/ParserGenerator.cpp

semantic_analyzer_generator_src = \
  SemanticAnalyzerGenerator/SemanticAction.cpp \
  SemanticAnalyzerGenerator/SemanticAnalyzer.cpp \
  SemanticAnalyzerGenerator/SemanticAnalyzerGenerator.cpp

pgen_SOURCES = \
  main.cpp \
  $(lexical_analyzer_generator_src) \
  $(parser_generator_src) \
  $(semantic_analyzer_generator_src)

parser_SOURCES = \
  parser.pgen.cpp \
  $(lexical_analyzer_generator_src) \
  $(parser_generator_src) \
  $(semantic_analyzer_generator_src)

dump_parser_SOURCES = \
  dump-parser.cpp \
  $(lexical_analyzer_generator_src) \
  $(parser_generator_src) \
  $(semantic_analyzer_generator_src)
