grammar calc;

/* Production Rules */
stmt: expr NEWLINE                      # ExprStmt
    | NEWLINE                           # EmptyStmt
    ;

expr: <assoc=right> expr CARET expr     # Exponentiation
    | expr op=(ASTERISK|FWDSLASH) expr  # MultiplyOrDivide
    | expr op=(PLUS|MINUS) expr         # PlusOrMinus
    | op=(PLUS|MINUS) expr              # NegationOrUnaryPlus
    | LPAREN expr RPAREN                # Parens
    | NUMBER                            # Number
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
NEWLINE:    [\r\n];
WHITESPACE: [ \t]+ -> skip;