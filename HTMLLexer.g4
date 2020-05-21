lexer grammar HTMLLexer;

SEA_WS
    :  (' '|'\t'|'\r'? '\n')+
    ;

TAG_OPEN
    : '<' -> pushMode(TAG)
    ;

HTML_TEXT
    : ~'<'+
    ;

//
// tag declarations
//
mode TAG;

TAG_CLOSE
    : '>' -> popMode
    ;

TAG_SLASH
    : '/'
    ;

//
// lexing mode for attribute values
//
TAG_EQUALS
    : '=' -> pushMode(ATTVALUE)
    ;

TAG_NAME
    : Nondigit (Nondigit | Digit)*
    ;

fragment Nondigit
    : [a-zA-Z_]
    ;

fragment Digit
    : [0-9]
    ;

//
// attribute values
//
mode ATTVALUE;

ATTVALUE_VALUE
    : [ ]* ATTRIBUTE -> popMode
    ;

ATTRIBUTE
    : DOUBLE_QUOTE_STRING
    ;

fragment DOUBLE_QUOTE_STRING
    : '"' ~[<"]* '"'
    ;
