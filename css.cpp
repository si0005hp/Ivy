#include <iostream>
#include <string>
#include <memory>
#include <algorithm>

#include "ivy.h"

std::shared_ptr<Stylesheet> IvyCSSParser::parseCSS(CSSParser::StylesheetContext *ctx)
{
  std::vector<std::shared_ptr<Rule>> rules;

  for (size_t i = 0; i < ctx->ruleSet().size(); i++)
  {
    auto rule = parseRuleSet(ctx->ruleSet().at(i));
    if (rule != nullptr)
    {
      rules.push_back(rule);
    }
  }
  return std::make_shared<Stylesheet>(rules);
}

std::shared_ptr<Rule> IvyCSSParser::parseRuleSet(CSSParser::RuleSetContext *ctx)
{
  auto declarations = parseDeclarationList(ctx->declarationList());
  auto selectors = parseSelectorGroup(ctx->selectorGroup());
  return std::make_shared<Rule>(selectors, declarations);
}

std::vector<std::shared_ptr<Selector>> IvyCSSParser::parseSelectorGroup(CSSParser::SelectorGroupContext *ctx)
{
  std::vector<std::shared_ptr<Selector>> selectors;
  for (size_t i = 0; i < ctx->simpleSelectorSequence().size(); i++)
  {
    auto selector = parseSimpleSelectorSequence(ctx->simpleSelectorSequence().at(i));
    if (selector != nullptr)
    {
      selectors.push_back(selector);
    }
  }
  // sort selectors by specificity as highest to lowest.
  std::sort(selectors.begin(), selectors.end(),
            [](std::shared_ptr<Selector> &s1, std::shared_ptr<Selector> &s2) {
              return s1->specificity() > s2->specificity();
            });
  return selectors;
}

std::shared_ptr<Selector> IvyCSSParser::parseSimpleSelectorSequence(CSSParser::SimpleSelectorSequenceContext *ctx)
{
  std::optional<std::string> tagName = parseElementName(ctx->elementName());

  std::optional<std::string> id;
  std::vector<std::string> classes;
  for (size_t i = 0; i < ctx->idOrClassNameSelector().size(); i++)
  {
    auto idOrClassName = ctx->idOrClassNameSelector().at(i);

    id = parseId(idOrClassName->id());
    if (idOrClassName->className() != nullptr)
    {
      classes.push_back(parseClassName(idOrClassName->className()));
    }
  }
  return std::make_shared<SimpleSelector>(tagName, id, classes);
}

std::optional<std::string> IvyCSSParser::parseElementName(CSSParser::ElementNameContext *ctx)
{
  return ctx == nullptr ? std::nullopt : std::optional<std::string>(ctx->IDENT()->getText());
}

std::optional<std::string> IvyCSSParser::parseId(CSSParser::IdContext *ctx)
{
  return ctx == nullptr ? std::nullopt : std::optional<std::string>(ctx->IDENT()->getText());
}

std::string IvyCSSParser::parseClassName(CSSParser::ClassNameContext *ctx)
{
  return ctx->IDENT()->getText();
}

std::vector<std::shared_ptr<Declaration>> IvyCSSParser::parseDeclarationList(CSSParser::DeclarationListContext *ctx)
{
  std::vector<std::shared_ptr<Declaration>> declarations;
  for (size_t i = 0; i < ctx->declaration().size(); i++)
  {
    auto declaration = parseDeclaration(ctx->declaration().at(i));
    if (declaration != nullptr)
    {
      declarations.push_back(declaration);
    }
  }
  return declarations;
}

std::shared_ptr<Declaration> IvyCSSParser::parseDeclaration(CSSParser::DeclarationContext *ctx)
{
  auto property = parseProperty(ctx->property());
  auto value = parseValue(ctx->value());
  return std::make_shared<Declaration>(property, value);
}

std::string IvyCSSParser::parseProperty(CSSParser::PropertyContext *ctx)
{
  return ctx->IDENT()->getText();
}

std::shared_ptr<Value> IvyCSSParser::parseValue(CSSParser::ValueContext *ctx)
{
  if (ctx->keyword() != nullptr)
  {
    return parseKeyword(ctx->keyword());
  }
  else if (ctx->length() != nullptr)
  {
    return parseLength(ctx->length());
  }
  else if (ctx->color() != nullptr)
  {
    return parseColor(ctx->color());
  }
}

std::shared_ptr<Value> IvyCSSParser::parseKeyword(CSSParser::KeywordContext *ctx)
{
  return std::make_shared<Keyword>(ctx->IDENT()->getText());
}

std::shared_ptr<Value> IvyCSSParser::parseLength(CSSParser::LengthContext *ctx)
{
  auto value = std::stof(ctx->NUMBER()->getText());
  return std::make_shared<Length>(value, Unit::Px); // currently only support Px
}

std::shared_ptr<Value> IvyCSSParser::parseColor(CSSParser::ColorContext *ctx)
{
  std::string colorValueStr = ctx->colorValue()->getText();
  if (colorValueStr.length() != 6)
  {
    throw std::runtime_error("color value must be 6-digit hexadecimal number");
  }

  uint8_t r = std::stoi(colorValueStr.substr(0, 2), nullptr, 16);
  uint8_t g = std::stoi(colorValueStr.substr(2, 2), nullptr, 16);
  uint8_t b = std::stoi(colorValueStr.substr(4, 2), nullptr, 16);
  return std::make_shared<Color>(r, g, b, 255);
}

Specificity SimpleSelector::specificity()
{
  return std::make_tuple(id.has_value(), classes.size(), tagName.has_value());
}

bool SimpleSelector::matches(std::shared_ptr<ElementNode> elem)
{
  if (tagName.value() != elem->getTagName())
  {
    return false;
  }
  if (id != elem->id())
  {
    return false;
  }
  if (std::any_of(classes.begin(), classes.end(), [&](std::string clazz) { return elem->classes().count(clazz) <= 0; }))
  {
    return false;
  }

  return true;
}
