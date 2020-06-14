#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include <optional>
#include <tuple>

#include "antlr4-runtime.h"

#include "HTMLParserBaseVisitor.h"
#include "CSSParserBaseVisitor.h"
#include "HTMLLexer.h"
#include "HTMLParser.h"
#include "CSSLexer.h"
#include "CSSParser.h"

#pragma once

using namespace antlr4;

template <typename L, typename P>
P *generateAntlr4Parser(std::string fpath)
{
  std::ifstream stream;
  stream.open(fpath);
  ANTLRInputStream *input = new ANTLRInputStream(stream);
  L *lexer = new L(input);
  CommonTokenStream *tokens = new CommonTokenStream(lexer);
  P *parser = new P(tokens);
  return parser;
}

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

enum class Unit
{
  Px
};

class Length : public Value
{
  float value;
  Unit unit;

public:
  Length(float value, Unit unit) : value(value), unit(unit) {}
};

class Color : public Value
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;

public:
  Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}
};

class Declaration
{
  std::string property;
  std::shared_ptr<Value> value;

public:
  Declaration(std::string property, std::shared_ptr<Value> value) : property(property), value(value) {}
};

using Specificity = std::tuple<size_t, size_t, size_t>;

class Selector
{
public:
  virtual Specificity specificity() = 0;
};

class SimpleSelector : public Selector
{
  std::optional<std::string> tagName;
  std::optional<std::string> id;
  std::vector<std::string> classes;

public:
  SimpleSelector(std::optional<std::string> tagName, std::optional<std::string> id, std::vector<std::string> classes)
      : tagName(tagName), id(id), classes(classes) {}

  Specificity specificity()
  {
    return std::make_tuple<size_t, size_t, size_t>(
        id.has_value(),
        classes.size(),
        tagName.has_value());
  }
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
  std::optional<std::string> parseElementName(CSSParser::ElementNameContext *ctx);
  std::optional<std::string> parseId(CSSParser::IdContext *ctx);
  std::string parseClassName(CSSParser::ClassNameContext *ctx);
  std::vector<std::shared_ptr<Declaration>> parseDeclarationList(CSSParser::DeclarationListContext *ctx);
  std::shared_ptr<Declaration> parseDeclaration(CSSParser::DeclarationContext *ctx);
  std::string parseProperty(CSSParser::PropertyContext *ctx);
  std::shared_ptr<Value> parseValue(CSSParser::ValueContext *ctx);
  std::shared_ptr<Value> parseKeyword(CSSParser::KeywordContext *ctx);
  std::shared_ptr<Value> parseLength(CSSParser::LengthContext *ctx);
  std::shared_ptr<Value> parseColor(CSSParser::ColorContext *ctx);

public:
  std::shared_ptr<Stylesheet> parseCss(CSSParser::StylesheetContext *ctx);
};
