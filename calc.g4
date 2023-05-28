grammar calc;

/* Production Rules */
stmts
    : stmts stmt                            # MultipleStmt
    | stmt                                  # SingleStmt
    ;

stmt: expr NEWLINE                          # ExprStmt
    | NEWLINE                               # EmptyStmt
    ;

expr: NUMBER                                # Number
    | fn=IDENTIFIER LPAREN arg=args RPAREN  # Function
    | LPAREN expr RPAREN                    # Parens
    | <assoc=right> expr CARET expr         # Exponentiation
    | <assoc=right> op=(PLUS|MINUS) expr    # UnaryPlusOrNegation
    | expr LPAREN expr RPAREN               # ImplicitMultiplication
    | expr op=(ASTERISK|FWDSLASH) expr      # MultiplyOrDivide
    | expr op=(PLUS|MINUS) expr             # PlusOrMinus
    ;

args: expr (COMMA expr)* ;

/* Tokens */
CARET:      '^';
COMMA:      ',';
LPAREN:     '(';
RPAREN:     ')';
PLUS:       '+';
MINUS:      '-';
ASTERISK:   '*';
FWDSLASH:   '/';
IDENTIFIER: [A-Za-z]+;
NUMBER:     [0-9]+('.'[0-9]+)?EXPONENT?;
NEWLINE:    '\r'? '\n'+;
WHITESPACE: [ \t]+ -> skip;

fragment
EXPONENT:   [eE][+-]?[0-9]+;
