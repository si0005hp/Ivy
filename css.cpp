#include <iostream>
#include <string>
#include <memory>
#include "CSSParserBaseVisitor.h"

#include "ivy.h"

std::shared_ptr<Stylesheet> CSSVisitor::parseCss(CSSParser::StylesheetContext *ctx)
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

std::shared_ptr<Rule> CSSVisitor::parseRuleSet(CSSParser::RuleSetContext *ctx)
{
  auto declarations = parseDeclarationList(ctx->declarationList());
  auto selectors = parseSelectorGroup(ctx->selectorGroup());
  return std::make_shared<Rule>(selectors, declarations);
}

std::vector<std::shared_ptr<Selector>> CSSVisitor::parseSelectorGroup(CSSParser::SelectorGroupContext *ctx)
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
  return selectors;
}

std::shared_ptr<Selector> CSSVisitor::parseSimpleSelectorSequence(CSSParser::SimpleSelectorSequenceContext *ctx)
{
  if (ctx->elementName() != nullptr)
  {
    return nullptr;
  }
  return nullptr;
}

std::vector<std::shared_ptr<Declaration>> CSSVisitor::parseDeclarationList(CSSParser::DeclarationListContext *ctx)
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

std::shared_ptr<Declaration> CSSVisitor::parseDeclaration(CSSParser::DeclarationContext *ctx)
{
  auto property = parseProperty(ctx->property());
  auto value = parseValue(ctx->value());
  return std::make_shared<Declaration>(property, value);
}

std::string CSSVisitor::parseProperty(CSSParser::PropertyContext *ctx)
{
  return ctx->IDENT()->getText();
}

std::shared_ptr<Value> CSSVisitor::parseValue(CSSParser::ValueContext *ctx)
{
  if (ctx->keyword() != nullptr)
  {
    return parseKeyword(ctx->keyword());
  }
  // TODO
  return nullptr;
}

std::shared_ptr<Value> CSSVisitor::parseKeyword(CSSParser::KeywordContext *ctx)
{
  return std::make_shared<Keyword>(ctx->IDENT()->getText());
}
