#include <iostream>
#include <string>
#include <memory>
#include <tuple>

#include "ivy.h"

std::shared_ptr<StyledNode> StyledNodeBuilder::buildStyledNode(std::shared_ptr<Node> htmlNode, std::shared_ptr<Stylesheet> stylesheet)
{
  std::vector<std::shared_ptr<StyledNode>> children;
  for (size_t i = 0; i < htmlNode->getChildren().size(); i++)
  {
    children.push_back(buildStyledNode(htmlNode->getChildren().at(i), stylesheet));
  }
  return std::make_shared<StyledNode>(htmlNode, specifiedValues(htmlNode, stylesheet), children);
}

PropertyMap StyledNodeBuilder::specifiedValues(std::shared_ptr<Node> node, std::shared_ptr<Stylesheet> stylesheet)
{
  PropertyMap specifiedValues;
  if (node->getType() != NodeType::Element)
  {
    return specifiedValues;
  }

  std::shared_ptr<ElementNode> elem = std::static_pointer_cast<ElementNode>(node);
  std::vector<MatchedRule> rules = matchingRules(elem, stylesheet);

  // sort selectors by specificity as lowest to highest.
  std::sort(rules.begin(), rules.end(), [](MatchedRule &r1, MatchedRule &r2) {
    return std::get<0>(r1) < std::get<0>(r2);
  });

  for (size_t i = 0; i < rules.size(); i++)
  {
    std::shared_ptr<Rule> rule = std::get<1>(rules.at(i));
    for (size_t i = 0; i < rule->getDeclarations().size(); i++)
    {
      std::shared_ptr<Declaration> decl = rule->getDeclarations().at(i);
      specifiedValues[decl->getProperty()] = decl->getValue();
    }
  }
  return specifiedValues;
}

std::vector<MatchedRule> StyledNodeBuilder::matchingRules(std::shared_ptr<ElementNode> elem, std::shared_ptr<Stylesheet> stylesheet)
{
  std::vector<MatchedRule> rules;
  for (size_t i = 0; i < stylesheet->getRules().size(); i++)
  {
    auto optRule = matchRule(elem, stylesheet->getRules().at(i));
    if (optRule.has_value())
    {
      rules.push_back(optRule.value());
    }
  }
  return rules;
}

std::optional<MatchedRule> StyledNodeBuilder::matchRule(std::shared_ptr<ElementNode> elem, std::shared_ptr<Rule> rule)
{
  for (size_t i = 0; i < rule->getSelectors().size(); i++)
  {
    auto selector = rule->getSelectors().at(i);
    if (selector->matches(elem))
    {
      return std::make_optional(std::make_tuple(selector->specificity(), rule));
    }
  }
  return std::nullopt;
}
