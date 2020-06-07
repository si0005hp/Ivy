#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>

#include "HTMLParserBaseVisitor.h"
#include "CSSParserBaseVisitor.h"

#pragma once

/* html */

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

/* css */

class Value
{
};

class Keyword : public Value
{
  std::string keyword;

public:
  Keyword(const std::string &keyword) : keyword(keyword) {}
};

class Declaration
{
  std::string property;
  std::shared_ptr<Value> value;

public:
  Declaration(std::string property, std::shared_ptr<Value> value) : property(property), value(value) {}
};

class Selector
{
};

class SimpleSelector : public Selector
{
  std::string tagName;
  std::string id;
  std::vector<std::string> classes;
};

class Rule
{
  std::vector<std::shared_ptr<Selector>> selectors;
  std::vector<std::shared_ptr<Declaration>> declarations;

public:
  Rule(std::vector<std::shared_ptr<Selector>> selectors, std::vector<std::shared_ptr<Declaration>> declarations)
      : selectors(selectors), declarations(declarations) {}
};

class Stylesheet
{
  std::vector<std::shared_ptr<Rule>> rules;

public:
  Stylesheet(std::vector<std::shared_ptr<Rule>> rules) : rules(rules) {}
};

class CSSVisitor : public CSSParserBaseVisitor
{
  std::shared_ptr<Rule> parseRuleSet(CSSParser::RuleSetContext *ctx);
  std::vector<std::shared_ptr<Selector>> parseSelectorGroup(CSSParser::SelectorGroupContext *ctx);
  std::shared_ptr<Selector> parseSimpleSelectorSequence(CSSParser::SimpleSelectorSequenceContext *ctx);
  std::vector<std::shared_ptr<Declaration>> parseDeclarationList(CSSParser::DeclarationListContext *ctx);
  std::shared_ptr<Declaration> parseDeclaration(CSSParser::DeclarationContext *ctx);
  std::string parseProperty(CSSParser::PropertyContext *ctx);
  std::shared_ptr<Value> parseValue(CSSParser::ValueContext *ctx);
  std::shared_ptr<Value> parseKeyword(CSSParser::KeywordContext *ctx);

public:
  std::shared_ptr<Stylesheet> parseCss(CSSParser::StylesheetContext *ctx);
};
