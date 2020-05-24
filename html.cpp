#include <iostream>
#include <string>
#include <memory>
#include "HTMLParserBaseVisitor.h"

#include "html.h"

void HTMLVisitor::visitHtmlAttribute(HTMLParser::HtmlAttributeContext *ctx, AttrMap &attrMap)
{
  if (ctx->htmlKeyValueAttribute() != nullptr)
  {
    auto attrName = ctx->htmlKeyValueAttribute()->htmlAttributeName()->TAG_NAME()->getText();
    auto attrValue = ctx->htmlKeyValueAttribute()->htmlAttributeValue()->ATTVALUE_VALUE()->getText();
    attrMap[attrName] = attrValue;
  }
  else if (ctx->htmlBooleanAttribute() != nullptr)
  {
    // TODO
  }
}

antlrcpp::Any HTMLVisitor::visitHtmlDocument(HTMLParser::HtmlDocumentContext *ctx)
{
  return visitChildren(ctx);
}

antlrcpp::Any HTMLVisitor::visitHtmlElement(HTMLParser::HtmlElementContext *ctx)
{
  auto tagName = visitHtmlTagName(ctx->htmlTagName().at(0));
  auto map = visitHtmlAttributes(ctx->htmlAttributes());
  auto children = visitHtmlContent(ctx->htmlContent());

  return std::make_shared<ElementNode>(ElementNode(tagName, map, children));
}

antlrcpp::Any HTMLVisitor::visitHtmlContent(HTMLParser::HtmlContentContext *ctx)
{
  std::vector<std::shared_ptr<Node>> nodes;

  for (size_t i = 0; i < ctx->htmlElementOrText().size(); i++)
  {
    auto node = visitHtmlElementOrText(ctx->htmlElementOrText().at(i));
    nodes.push_back(node);
  }
  return nodes;
}

antlrcpp::Any HTMLVisitor::visitHtmlElementOrText(HTMLParser::HtmlElementOrTextContext *ctx)
{
  if (ctx->htmlElement() != nullptr)
  {
    return visitHtmlElement(ctx->htmlElement());
  }
  else if (ctx->htmlChardata() != nullptr)
  {
    return visitHtmlChardata(ctx->htmlChardata());
  }
}

antlrcpp::Any HTMLVisitor::visitHtmlAttributes(HTMLParser::HtmlAttributesContext *ctx)
{
  AttrMap attrMap;
  for (size_t i = 0; i < ctx->htmlAttribute().size(); i++)
  {
    visitHtmlAttribute(ctx->htmlAttribute().at(i), attrMap);
  }
  return attrMap;
}

antlrcpp::Any HTMLVisitor::visitHtmlKeyValueAttribute(HTMLParser::HtmlKeyValueAttributeContext *ctx)
{
  return visitChildren(ctx);
}

antlrcpp::Any HTMLVisitor::visitHtmlBooleanAttribute(HTMLParser::HtmlBooleanAttributeContext *ctx)
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
  return ctx->TAG_NAME()->getText();
}

antlrcpp::Any HTMLVisitor::visitHtmlChardata(HTMLParser::HtmlChardataContext *ctx)
{
  if (ctx->HTML_TEXT() != nullptr)
  {
    // return std::make_shared<TextNode>(ctx->HTML_TEXT()->getText()); // TODO: Figure out why this fails
    return std::static_pointer_cast<Node>(std::make_shared<TextNode>(ctx->HTML_TEXT()->getText()));
  }
  return nullptr;
}
