#include <iostream>
#include <string>
#include <memory>

#include "ivy.h"

std::shared_ptr<Node> HTMLVisitor::parseHtml(HTMLParser::HtmlDocumentContext *ctx)
{
  return parseHtmlElement(ctx->htmlElement());
}

std::shared_ptr<Node> HTMLVisitor::parseHtmlElement(HTMLParser::HtmlElementContext *ctx)
{
  auto tagName = parseHtmlTagName(ctx->htmlTagName().at(0));
  auto map = parseHtmlAttributes(ctx->htmlAttributes());
  auto children = parseHtmlContent(ctx->htmlContent());

  return std::make_shared<ElementNode>(tagName, map, children);
}

std::string HTMLVisitor::parseHtmlTagName(HTMLParser::HtmlTagNameContext *ctx)
{
  return ctx->TAG_NAME()->getText();
}

std::vector<std::shared_ptr<Node>> HTMLVisitor::parseHtmlContent(HTMLParser::HtmlContentContext *ctx)
{
  std::vector<std::shared_ptr<Node>> nodes;

  for (size_t i = 0; i < ctx->htmlElementOrText().size(); i++)
  {
    auto node = parseHtmlElementOrText(ctx->htmlElementOrText().at(i));
    if (node != nullptr)
    {
      nodes.push_back(node);
    }
  }
  return nodes;
}

std::shared_ptr<Node> HTMLVisitor::parseHtmlElementOrText(HTMLParser::HtmlElementOrTextContext *ctx)
{
  if (ctx->htmlElement() != nullptr)
  {
    return parseHtmlElement(ctx->htmlElement());
  }
  else if (ctx->htmlChardata() != nullptr)
  {
    return parseHtmlChardata(ctx->htmlChardata());
  }
}

AttrMap HTMLVisitor::parseHtmlAttributes(HTMLParser::HtmlAttributesContext *ctx)
{
  AttrMap attrMap;
  for (size_t i = 0; i < ctx->htmlAttribute().size(); i++)
  {
    parseHtmlAttribute(ctx->htmlAttribute().at(i), attrMap);
  }
  return attrMap;
}

void HTMLVisitor::parseHtmlAttribute(HTMLParser::HtmlAttributeContext *ctx, AttrMap &attrMap)
{
  if (ctx->htmlKeyValueAttribute() != nullptr)
  {
    auto attrName = ctx->htmlKeyValueAttribute()->htmlAttributeName()->TAG_NAME()->getText();
    auto attrValue = ctx->htmlKeyValueAttribute()->htmlAttributeValue()->ATTVALUE_VALUE()->getText();
    attrMap[attrName] = attrValue;
  }
  else if (ctx->htmlBooleanAttribute() != nullptr)
  {
    throw std::runtime_error("not inmplemented yet");
  }
}

std::shared_ptr<Node> HTMLVisitor::parseHtmlChardata(HTMLParser::HtmlChardataContext *ctx)
{
  if (ctx->HTML_TEXT() == nullptr)
  {
    return nullptr;
  }
  return std::make_shared<TextNode>(ctx->HTML_TEXT()->getText());
}
