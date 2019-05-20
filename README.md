# Parser Generator with SDTS Support

This is an LL(1) parser generator which supports Syntax-Directed Translation Schemes with C/C++ code fragments within the production bodies of grammar rules.

## Usage

```
$ mkdir build && cd build
$ ../configure
$ make
$ ./pgen
USAGE: pgen <lexer-rules-file> <parser-rules-file>
```

## Lexical Analyzer

In order for the included lexical analyzer to tokenize the target input stream, you must provide a lexical rules file declaring the different types of tokens together with their regex patterns. The longest matching prefix is always chosen, where tokens corresponding to rules appearing first in the rules file have higher priorities in case of ties.

The following syntax for regular expressions is supported (whitespaces are omitted):
- `a-z`: matches any of `a`, `b`, ..., `z`. [Character Classes]
- `a*`: matches zero or more `a`'s. [Kleene Closure]
- `a+`: matches one or more `a`'s. [Positive Closure]
- `ab`: matches `ab`. [Concatenation]
- `a|b`: matches `a` or `b`. [Alternation]
- `\L`: matches an empty string. [Epsilon/Lambda]

Any reserved character needed to be used in the language is preceded by an escape backslash character.
Character classes have the highest precedence, followed by positive and Kleene closure, then concatenation, and finally alternation. Parentheses can be used to override these precedence rules; expressions appearing in parentheses are evaluated first. (i.e.: they have higher precedence than any other operator).

Regular definitions that are to be used in subsequent rules take the following form:
```
<LHS> = <RHS>
```

Token rules take the following form:
```
<LHS>: <RHS>
```

Keywords are enclosed by dollar signs on separate lines and have the highest priority regardless of their location.
Punctuation characters are enclosed by square brackets on separate lines as well.

### Example:

```
letter = a-z | A-Z
digit = 0 - 9
id: letter (letter|digit)*
digits = digit+
{boolean int float}
num: digit+ | digit+ . digits ( \L | E digits)
relop: \=\= | !\= | > | >\= | < | <\=
assign: \=
{ if else while }
[; , \( \) { }]
addop: \+ | \-
mulop: \* | /
```

## Parser Generator

The input grammar is assumed to be an LL(1) left-factored grammar with no left recursion.
Parsing rules are specified in a text file that takes the following general form:
```
$
/* C/C++ code (e.g.: header includes and struct definitions, more on that below) */
$
/* RULES */
```

Rules start with hash signs (`#`) and take the following form (whitespaces are omitted):
```
# <LHS> = <RHS>
```

The left-hand side is the name of a non-terminal symbol, and the right-hand side is a list of production bodies separated by `|`'s. Terminal symbols are enclosed in single quotes and must match the names of the corresponding tokens in the lexical rules file. The names of all grammar symbols must be valid C/C++ identifiers. `'\L'` is a special symbol for the empty string.

### Example:

```
# METHOD_BODY = STATEMENT_LIST
# STATEMENT_LIST = STATEMENT | STATEMENT_LIST STATEMENT
# STATEMENT = DECLARATION
| IF
| WHILE
| ASSIGNMENT
# DECLARATION = PRIMITIVE_TYPE 'id' ';'
# PRIMITIVE_TYPE = 'int' | 'float'
# IF = 'if' '(' EXPRESSION ')' '{' STATEMENT '}' 'else' '{' STATEMENT '}'
# WHILE = 'while' '(' EXPRESSION ')' '{' STATEMENT '}'
# ASSIGNMENT = 'id' '=' EXPRESSION ';'
# EXPRESSION = SIMPLE_EXPRESSION
| SIMPLE_EXPRESSION 'relop' SIMPLE_EXPRESSION
# SIMPLE_EXPRESSION = TERM | SIGN TERM | SIMPLE_EXPRESSION 'addop' TERM
# TERM = FACTOR | TERM 'mulop' FACTOR
# FACTOR = 'id' | 'num' | '(' EXPRESSION ')'
# SIGN = '+' | '-'
```

### Includes and Attribute Definitions

The code block at the top of the parsing rules file is taken as is and placed in the generated parser. You can put any `#include` directives you need in your code fragments there, and you can even define your own functions there and call them in your code fragments to make your rules more readable and concise.

Any semantic attributes of non-terminal symbols _must_ be defined in the initial code block as members of `struct`'s with exactly the same names as the associated non-terminal symbols.

Code fragments are given in production bodies and enclosed between dollar signs. The head of a production is referred to in code fragments by its name, while occurrences of non-terminal and terminal symbols are referred to by numbers, denoting their orders of occurrence in the production body with respect to other symbols of the same type, concatenated to their names. (e.g.: `num1` for the first occurrence of `num`). Terminal symbols are empty `std::string`'s; to capture the value of a terminal symbol, you _have to_ set the corresponding `std::string` to `_input` in a code fragment right after the occurrence of the symbol. Non-terminal symbols are `struct`'s with the same names.

#### Example:

```c
$
#include <iostream>

struct SIMPLE_EXPR {
  int value;
};

struct ADDITION {
  int prevNum;
  int value;
};
$
# SIMPLE_EXPR = 'num'
                $ num1 = _input;
                  ADDITION1.prevNum = std::stoi(num1); $
                ADDITION
                $ SIMPLE_EXPR.value = ADDITION1.value;
                  std::cout << "Result: " << SIMPLE_EXPR.value << std::endl; $
# ADDITION = 'addop' 'num'
             $ num1 = _input;
               ADDITION1.prevNum = ADDITION.prevNum + std::stoi(num1); $
             ADDITION
             $ ADDITION.value = ADDITION1.value; $ |
             '\L' $ ADDITION.value = ADDITION.prevNum; $
```
