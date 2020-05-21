parser grammar HTMLParser;

options {
	tokenVocab = HTMLLexer;
}

htmlDocument
    : htmlElement
    ;

htmlElement
    : TAG_OPEN htmlTagName htmlAttribute* TAG_CLOSE htmlContent TAG_OPEN TAG_SLASH htmlTagName TAG_CLOSE
    ;

htmlContent
    : htmlChardata? (htmlElement htmlChardata?)*
    ;

htmlAttribute
    : htmlAttributeName TAG_EQUALS htmlAttributeValue
    | htmlAttributeName
    ;

htmlAttributeName
    : TAG_NAME
    ;

htmlAttributeValue
    : ATTVALUE_VALUE
    ;

htmlTagName
    : TAG_NAME
    ;

htmlChardata
    : HTML_TEXT
    | SEA_WS
    ;
