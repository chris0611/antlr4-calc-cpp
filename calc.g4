grammar calc;

/* Production Rules */
stmts: stmt+;

stmt: expr NEWLINE                          # ExprStmt
    | NEWLINE                               # EmptyStmt
    ;

expr: NUMBER                                # Number
    | CLEAR                                 # Clear
    | fn=IDENTIFIER LPAREN arg=args RPAREN  # Function
    | var=IDENTIFIER                        # Variable
    | LPAREN expr RPAREN                    # Parens
    | <assoc=right> expr CARET expr         # Exponentiation
    | <assoc=right> op=(PLUS|MINUS) expr    # UnaryPlusOrNegation
    | expr LPAREN expr RPAREN               # ImplicitMultiplication
    | expr op=(ASTERISK|FWDSLASH) expr      # MultiplyOrDivide
    | expr op=(PLUS|MINUS) expr             # PlusOrMinus
    | var=IDENTIFIER ASSIGN expr            # VariableAssignment
    ;

args: expr (COMMA expr)* ;

/* Tokens */
CARET:      '^';
COMMA:      ',';
ASSIGN:     '=';
LPAREN:     '(';
RPAREN:     ')';
PLUS:       '+';
MINUS:      '-';
ASTERISK:   '*';
FWDSLASH:   '/';
CLEAR:      'clear';    // For clearing memory of variables
IDENTIFIER: [A-Za-z]+;
NUMBER:     [0-9]+('.'[0-9]+)?EXPONENT?;
NEWLINE:    '\r'? '\n'+;
WHITESPACE: [ \t]+ -> skip;

fragment
EXPONENT:   [eE][+-]?[0-9]+;
