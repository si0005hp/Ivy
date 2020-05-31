parser grammar CSSParser;

options {
	tokenVocab = CSSLexer;
}

stylesheet
    : ruleSet*
    ;

selectorGroup
    : simpleSelectorSequence (COMMA simpleSelectorSequence)*
    ;

simpleSelectorSequence
    : elementName idOrClassNameSelector*
    | idOrClassNameSelector+
    ;

idOrClassNameSelector
    : id
    | className
    ;

elementName
    : IDENT
    ;

id
    : HASH IDENT
    ;

className
    : DOT IDENT
    ;

ruleSet
    : selectorGroup LEFT_BRACE declarationList? RIGHT_BRACE
    ;

declarationList
    : declaration+
    ;

declaration
    : property COLON value SEMICOLON
    ;

property
    : IDENT
    ;

value
    : keyword
    | length
    | color
    ;

keyword
    : IDENT
    ;

length
    : NUMBER unit
    ;

unit
    : PX
    ;

color
    : HASH colorValue
    ;

colorValue
    : IDENT
    | NUMBER
    | HEX
    ;
