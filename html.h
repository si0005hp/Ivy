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
  std::shared_ptr<Node> parseHtmlElement(HTMLParser::HtmlElementContext *ctx);
  std::string parseHtmlTagName(HTMLParser::HtmlTagNameContext *ctx);
  std::vector<std::shared_ptr<Node>> parseHtmlContent(HTMLParser::HtmlContentContext *ctx);
  std::shared_ptr<Node> parseHtmlElementOrText(HTMLParser::HtmlElementOrTextContext *ctx);
  AttrMap parseHtmlAttributes(HTMLParser::HtmlAttributesContext *ctx);
  void parseHtmlAttribute(HTMLParser::HtmlAttributeContext *ctx, AttrMap &attrMap);
  std::shared_ptr<Node> parseHtmlChardata(HTMLParser::HtmlChardataContext *ctx);

public:
  std::shared_ptr<Node> parseHtml(HTMLParser::HtmlDocumentContext *ctx);
};
