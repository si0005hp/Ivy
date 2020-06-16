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

enum class NodeType
{
  Element,
  Text
};

class Node
{
protected:
  NodeType type;
  std::vector<std::shared_ptr<Node>> children;

  Node(const NodeType &type) : type(type) {}
  Node(const NodeType &type, const std::vector<std::shared_ptr<Node>> &children) : type(type), children(children) {}

public:
  NodeType getType() { return type; }
  std::vector<std::shared_ptr<Node>> getChildren() { return children; }
};

class ElementNode : public Node
{
  std::string tagName;
  AttrMap attrMap;

public:
  ElementNode(const NodeType &type, const std::string &tagName, const AttrMap &attrMap, const std::vector<std::shared_ptr<Node>> &children)
      : tagName(tagName), attrMap(attrMap), Node(type, children) {}

  std::string getTagName() { return tagName; }
  AttrMap getAttrMap() { return attrMap; }
};

class TextNode : public Node
{
  std::string text;

public:
  TextNode(const NodeType &type, const std::string &text) : text(text), Node(type) {}

  std::string getText() { return text; }
};

class IvyHTMLParser : public HTMLParserBaseVisitor
{
  std::shared_ptr<ElementNode> parseHtmlElement(HTMLParser::HtmlElementContext *ctx);
  std::string parseHtmlTagName(HTMLParser::HtmlTagNameContext *ctx);
  std::vector<std::shared_ptr<Node>> parseHtmlContent(HTMLParser::HtmlContentContext *ctx);
  std::shared_ptr<Node> parseHtmlElementOrText(HTMLParser::HtmlElementOrTextContext *ctx);
  AttrMap parseHtmlAttributes(HTMLParser::HtmlAttributesContext *ctx);
  void parseHtmlAttribute(HTMLParser::HtmlAttributeContext *ctx, AttrMap &attrMap);
  std::string parseHtmlAttributeName(HTMLParser::HtmlAttributeNameContext *ctx);
  std::string parseHtmlAttributeValue(HTMLParser::HtmlAttributeValueContext *ctx);
  std::shared_ptr<TextNode> parseHtmlChardata(HTMLParser::HtmlChardataContext *ctx);

public:
  std::shared_ptr<ElementNode> parseHtml(HTMLParser::HtmlDocumentContext *ctx);
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

  std::string getKeyword() { return keyword; }
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

  float getValue() { return value; }
  Unit getUnit() { return unit; }
};

class Color : public Value
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;

public:
  Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}

  uint8_t getR() { return r; }
  uint8_t getG() { return g; }
  uint8_t getB() { return b; }
  uint8_t getA() { return a; }
};

class Declaration
{
  std::string property;
  std::shared_ptr<Value> value;

public:
  Declaration(std::string property, std::shared_ptr<Value> value) : property(property), value(value) {}

  std::string getProperty() { return property; }
  std::shared_ptr<Value> getValue() { return value; }
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

  std::optional<std::string> getTagName() { return tagName; }
  std::optional<std::string> getId() { return id; }
  std::vector<std::string> getClasses() { return classes; }

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

  std::vector<std::shared_ptr<Selector>> getSelectors() { return selectors; }
  std::vector<std::shared_ptr<Declaration>> getDeclarations() { return declarations; }
};

class Stylesheet
{
  std::vector<std::shared_ptr<Rule>> rules;

public:
  Stylesheet(std::vector<std::shared_ptr<Rule>> rules) : rules(rules) {}

  std::vector<std::shared_ptr<Rule>> getRules() { return rules; }
};

class IvyCSSParser : public CSSParserBaseVisitor
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
  std::shared_ptr<Stylesheet> parseCSS(CSSParser::StylesheetContext *ctx);
};
