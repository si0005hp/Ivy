parser grammar HTMLParser;

options {
	tokenVocab = HTMLLexer;
}

htmlDocument
    : htmlElement
    ;

htmlElement
    : TAG_OPEN htmlTagName htmlAttributes TAG_CLOSE htmlContent TAG_OPEN TAG_SLASH htmlTagName TAG_CLOSE
    ;

htmlContent
    : htmlElementOrText*
    ;

htmlElementOrText
    : htmlElement
    | htmlChardata
    ;

htmlAttributes
    : htmlAttribute*
    ;

htmlAttribute
    : htmlKeyValueAttribute
    | htmlBooleanAttribute
    ;

htmlKeyValueAttribute
    : htmlAttributeName TAG_EQUALS htmlAttributeValue
    ;

htmlBooleanAttribute
    : htmlAttributeName
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
