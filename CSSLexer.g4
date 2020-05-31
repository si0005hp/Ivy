lexer grammar CSSLexer;

PX : 'px' ;

COMMA : ',' ;
HASH : '#' ;
DOT : '.';
LEFT_BRACE : '{';
RIGHT_BRACE : '}';
COLON : ':' ;
SEMICOLON : ';' ;

IDENT
    : '-'? Nondigit Char*
    ;

fragment Nondigit
    : [_a-zA-Z]
    ;

fragment Digit
    : [0-9]
    ;

NUMBER
    : Int
    | Float
    ;

fragment Int
    : Digit+
    ;

fragment Float
    : Digit* '.' Digit+
    ;

fragment Char
    : Digit
    | Nondigit
    | '-'
    ;

fragment HexDigit
    : ('0' .. '9' | 'a' .. 'f' | 'A' .. 'F')
    ;

HEX
   : HexDigit+
   ;

WS
    : Space+ -> skip
    ;

fragment Space
    : [ \t\r\n\f]+
    ;
