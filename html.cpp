#include <iostream>
#include <string>
#include <memory>
#include <sstream>
#include <unordered_set>

#include "ivy.h"

std::shared_ptr<ElementNode> IvyHTMLParser::parseHtml(HTMLParser::HtmlDocumentContext *ctx)
{
  std::shared_ptr<Node> root = parseHtmlElement(ctx->htmlElement());
  return std::static_pointer_cast<ElementNode>(root); // root node should be ElementNode
}

std::shared_ptr<ElementNode> IvyHTMLParser::parseHtmlElement(HTMLParser::HtmlElementContext *ctx)
{
  auto tagName = parseHtmlTagName(ctx->htmlTagName().at(0));
  auto map = parseHtmlAttributes(ctx->htmlAttributes());
  auto children = parseHtmlContent(ctx->htmlContent());

  return std::make_shared<ElementNode>(NodeType::Element, tagName, map, children);
}

std::string IvyHTMLParser::parseHtmlTagName(HTMLParser::HtmlTagNameContext *ctx)
{
  return ctx->TAG_NAME()->getText();
}

std::vector<std::shared_ptr<Node>> IvyHTMLParser::parseHtmlContent(HTMLParser::HtmlContentContext *ctx)
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

std::shared_ptr<Node> IvyHTMLParser::parseHtmlElementOrText(HTMLParser::HtmlElementOrTextContext *ctx)
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

AttrMap IvyHTMLParser::parseHtmlAttributes(HTMLParser::HtmlAttributesContext *ctx)
{
  AttrMap attrMap;
  for (size_t i = 0; i < ctx->htmlAttribute().size(); i++)
  {
    parseHtmlAttribute(ctx->htmlAttribute().at(i), attrMap);
  }
  return attrMap;
}

void IvyHTMLParser::parseHtmlAttribute(HTMLParser::HtmlAttributeContext *ctx, AttrMap &attrMap)
{
  if (ctx->htmlKeyValueAttribute() != nullptr)
  {
    auto attrName = parseHtmlAttributeName(ctx->htmlKeyValueAttribute()->htmlAttributeName());
    auto attrValue = parseHtmlAttributeValue(ctx->htmlKeyValueAttribute()->htmlAttributeValue());
    attrMap.emplace(attrName, attrValue);
  }
  else if (ctx->htmlBooleanAttribute() != nullptr)
  {
    throw std::runtime_error("not inmplemented yet");
  }
}

std::string IvyHTMLParser::parseHtmlAttributeName(HTMLParser::HtmlAttributeNameContext *ctx)
{
  return ctx->TAG_NAME()->getText();
}

std::string IvyHTMLParser::parseHtmlAttributeValue(HTMLParser::HtmlAttributeValueContext *ctx)
{
  std::string value = ctx->ATTVALUE_VALUE()->getText();
  // trim double quote
  value.erase(value.begin());
  value.erase(value.end() - 1);
  return value;
}

std::shared_ptr<TextNode> IvyHTMLParser::parseHtmlChardata(HTMLParser::HtmlChardataContext *ctx)
{
  if (ctx->HTML_TEXT() == nullptr)
  {
    return nullptr;
  }
  return std::make_shared<TextNode>(NodeType::Text, ctx->HTML_TEXT()->getText());
}

std::optional<std::string> ElementNode::id()
{
  return attrMap.count("id") > 0 ? std::make_optional(attrMap["id"]) : std::nullopt;
}

std::unordered_set<std::string> ElementNode::classes()
{
  std::unordered_set<std::string> classes;
  std::stringstream ss(attrMap["class"]);
  std::string item;
  while (getline(ss, item, ' '))
  {
    if (!item.empty())
    {
      classes.insert(item);
    }
  }
  return classes;
}
