#include <iostream>
#include "HTMLParserBaseVisitor.h"

#pragma once

class HTMLVisitor : public HTMLParserBaseVisitor
{
public:
  antlrcpp::Any visitHtmlDocument(HTMLParser::HtmlDocumentContext *ctx) override;
  antlrcpp::Any visitHtmlElement(HTMLParser::HtmlElementContext *ctx) override;
  antlrcpp::Any visitHtmlContent(HTMLParser::HtmlContentContext *ctx) override;
  antlrcpp::Any visitHtmlAttribute(HTMLParser::HtmlAttributeContext *ctx) override;
  antlrcpp::Any visitHtmlAttributeName(HTMLParser::HtmlAttributeNameContext *ctx) override;
  antlrcpp::Any visitHtmlAttributeValue(HTMLParser::HtmlAttributeValueContext *ctx) override;
  antlrcpp::Any visitHtmlTagName(HTMLParser::HtmlTagNameContext *ctx) override;
  antlrcpp::Any visitHtmlChardata(HTMLParser::HtmlChardataContext *ctx) override;
};
