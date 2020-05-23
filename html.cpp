#include <iostream>
#include <string>
#include "HTMLParserBaseVisitor.h"

#include "html.h"

antlrcpp::Any HTMLVisitor::visitHtmlDocument(HTMLParser::HtmlDocumentContext *ctx)
{
  return visitChildren(ctx);
}

antlrcpp::Any HTMLVisitor::visitHtmlElement(HTMLParser::HtmlElementContext *ctx)
{
  return visitChildren(ctx);
}

antlrcpp::Any HTMLVisitor::visitHtmlContent(HTMLParser::HtmlContentContext *ctx)
{
  return visitChildren(ctx);
}

antlrcpp::Any HTMLVisitor::visitHtmlAttribute(HTMLParser::HtmlAttributeContext *ctx)
{
  return visitChildren(ctx);
}

antlrcpp::Any HTMLVisitor::visitHtmlAttributeName(HTMLParser::HtmlAttributeNameContext *ctx)
{
  return visitChildren(ctx);
}

antlrcpp::Any HTMLVisitor::visitHtmlAttributeValue(HTMLParser::HtmlAttributeValueContext *ctx)
{
  return visitChildren(ctx);
}

antlrcpp::Any HTMLVisitor::visitHtmlTagName(HTMLParser::HtmlTagNameContext *ctx)
{
  return visitChildren(ctx);
}

antlrcpp::Any HTMLVisitor::visitHtmlChardata(HTMLParser::HtmlChardataContext *ctx)
{
  return visitChildren(ctx);
}
