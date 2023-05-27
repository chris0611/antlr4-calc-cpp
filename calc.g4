grammar calc;

/* Production Rules */
stmt: expr NEWLINE                          # ExprStmt
    | NEWLINE                               # EmptyStmt
    ;

expr: NUMBER                                # Number
    | LPAREN expr RPAREN                    # Parens
    | <assoc=right> expr CARET expr         # Exponentiation
    | <assoc=right> op=(PLUS|MINUS) expr    # NegationOrUnaryPlus
    | expr LPAREN expr RPAREN               # ImplicitMultiplication
    | expr op=(ASTERISK|FWDSLASH) expr      # MultiplyOrDivide
    | expr op=(PLUS|MINUS) expr             # PlusOrMinus
    ;

/* Tokens */
CARET:      '^';
LPAREN:     '(';
RPAREN:     ')';
PLUS:       '+';
MINUS:      '-';
ASTERISK:   '*';
FWDSLASH:   '/';
NUMBER:     [0-9]+;
NEWLINE:    '\r'? '\n'+;
WHITESPACE: [ \t]+ -> skip;