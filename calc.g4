grammar calc;

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

/* Production Rules */
statement
    : expression NEWLINE    # ExprStmt
    | NEWLINE               # EmptyStmt
    ;

expression
    : additive              # Expr
    ;

additive
    : multiplicative op=(PLUS|MINUS) additive   # PlusOrMinus
    | multiplicative                            # PropagateAdd
    ;

multiplicative
    : right=unary op=(ASTERISK|FWDSLASH) left=multiplicative    # MultiplyOrDivide
    | unary                                                     # PropagateMul
    ;

unary
    : op=(PLUS|MINUS) unary     # NegationOrUnaryPlus
    | exponent                  # UnaryExponent
    ;

exponent
    : factor CARET exponent     # Exponentiation
    | factor                    # ExponentFactor
    ;

factor
    : NUMBER                    # Number
    | LPAREN expression RPAREN  # Parenthesis
    ;
