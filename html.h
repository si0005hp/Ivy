#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>

#include "HTMLParserBaseVisitor.h"

#pragma once

using AttrMap = std::unordered_map<std::string, std::string>;

class Node
{
protected:
  std::vector<std::shared_ptr<Node>> children;

  Node() {}
  Node(const std::vector<std::shared_ptr<Node>> &children) : children(children) {}
};

class ElementNode : public Node
{
  std::string tagName;
  AttrMap attrMap;

public:
  ElementNode(const std::string &tagName, const AttrMap &attrMap, const std::vector<std::shared_ptr<Node>> &children)
      : tagName(tagName), attrMap(attrMap), Node(children) {}
};

class TextNode : public Node
{
  std::string text;

public:
  TextNode(const std::string &text) : text(text) {}
};

class HTMLVisitor : public HTMLParserBaseVisitor
{
  void visitHtmlAttribute(HTMLParser::HtmlAttributeContext *ctx, AttrMap &attrMap);

public:
  antlrcpp::Any visitHtmlDocument(HTMLParser::HtmlDocumentContext *ctx) override;
  antlrcpp::Any visitHtmlElement(HTMLParser::HtmlElementContext *ctx) override;
  antlrcpp::Any visitHtmlContent(HTMLParser::HtmlContentContext *ctx) override;
  antlrcpp::Any visitHtmlElementOrText(HTMLParser::HtmlElementOrTextContext *ctx) override;
  antlrcpp::Any visitHtmlAttributes(HTMLParser::HtmlAttributesContext *ctx) override;
  antlrcpp::Any visitHtmlKeyValueAttribute(HTMLParser::HtmlKeyValueAttributeContext *ctx) override;
  antlrcpp::Any visitHtmlBooleanAttribute(HTMLParser::HtmlBooleanAttributeContext *ctx) override;
  antlrcpp::Any visitHtmlAttributeName(HTMLParser::HtmlAttributeNameContext *ctx) override;
  antlrcpp::Any visitHtmlAttributeValue(HTMLParser::HtmlAttributeValueContext *ctx) override;
  antlrcpp::Any visitHtmlTagName(HTMLParser::HtmlTagNameContext *ctx) override;
  antlrcpp::Any visitHtmlChardata(HTMLParser::HtmlChardataContext *ctx) override;
};
