#include <gtest/gtest.h>
#include <cstdlib>
#include <experimental/filesystem>

#include "ivy.h"
#include "test_util.h"

using namespace antlr4;

std::shared_ptr<Stylesheet> parseCSS(std::string file)
{
  CSSParser *cssParser = generateAntlr4Parser<CSSLexer, CSSParser>(testResource(file));
  CSSParser::StylesheetContext *css = cssParser->stylesheet();

  CSSVisitor cssVisitor;
  return cssVisitor.parseCSS(css);
}

TEST(css_parse_test, minimal)
{
  std::shared_ptr<Stylesheet> stylesheet = parseCSS("css/minimal.css");
  ASSERT_EQ(stylesheet->getRules().size(), 1);

  std::shared_ptr<Rule> rule = stylesheet->getRules().at(0);
  ASSERT_EQ(rule->getSelectors().size(), 1);
  ASSERT_EQ(rule->getDeclarations().size(), 1);

  std::shared_ptr<SimpleSelector> simpleSelector = std::static_pointer_cast<SimpleSelector>(rule->getSelectors().at(0));
  ASSERT_EQ(simpleSelector->getTagName().value(), "html");
  ASSERT_EQ(simpleSelector->getId().has_value(), false);
  ASSERT_EQ(simpleSelector->getClasses().size(), 0);

  std::shared_ptr<Declaration> decl = rule->getDeclarations().at(0);
  ASSERT_EQ(decl->getProperty(), "margin");

  std::shared_ptr<Keyword> keyword = std::static_pointer_cast<Keyword>(decl->getValue());
  ASSERT_EQ(keyword->getKeyword(), "auto");
}

TEST(css_parse_test, declarations)
{
  std::shared_ptr<Stylesheet> stylesheet = parseCSS("css/declarations.css");

  std::vector<std::shared_ptr<Declaration>> decls = stylesheet->getRules().at(0)->getDeclarations();
  ASSERT_EQ(decls.size(), 2);

  std::shared_ptr<Declaration> decl1 = decls.at(0);
  std::shared_ptr<Color> color = std::static_pointer_cast<Color>(decl1->getValue());
  ASSERT_EQ(decl1->getProperty(), "background");
  ASSERT_EQ(color->getR(), 0);
  ASSERT_EQ(color->getG(), 204);
  ASSERT_EQ(color->getB(), 255);

  std::shared_ptr<Declaration> decl2 = decls.at(1);
  std::shared_ptr<Length> len = std::static_pointer_cast<Length>(decl2->getValue());
  ASSERT_EQ(decl2->getProperty(), "padding");
  ASSERT_EQ(len->getValue(), 50);
  ASSERT_EQ(len->getUnit(), Unit::Px);
}

TEST(css_parse_test, selectors)
{
  std::shared_ptr<Stylesheet> stylesheet = parseCSS("css/selectors.css");

  std::vector<std::shared_ptr<Selector>> selectors1 = stylesheet->getRules().at(0)->getSelectors();
  ASSERT_EQ(selectors1.size(), 1);
  std::shared_ptr<SimpleSelector> classSelector = std::static_pointer_cast<SimpleSelector>(selectors1.at(0));
  ASSERT_EQ(classSelector->getClasses().size(), 1);
  ASSERT_EQ(classSelector->getClasses().at(0), "primary");

  std::vector<std::shared_ptr<Selector>> selectors2 = stylesheet->getRules().at(1)->getSelectors();
  ASSERT_EQ(selectors2.size(), 1);
  std::shared_ptr<SimpleSelector> idSelector = std::static_pointer_cast<SimpleSelector>(selectors2.at(0));
  ASSERT_EQ(idSelector->getId().value(), "root");

  std::vector<std::shared_ptr<Selector>> selectors3 = stylesheet->getRules().at(2)->getSelectors();
  ASSERT_EQ(selectors3.size(), 2);
  std::shared_ptr<SimpleSelector> selector1 = std::static_pointer_cast<SimpleSelector>(selectors3.at(0));
  std::shared_ptr<SimpleSelector> selector2 = std::static_pointer_cast<SimpleSelector>(selectors3.at(1));
  ASSERT_EQ(selector1->getClasses().at(0), "one");
  ASSERT_EQ(selector2->getClasses().at(0), "two");

  std::vector<std::shared_ptr<Selector>> selectors4 = stylesheet->getRules().at(3)->getSelectors();
  ASSERT_EQ(selectors4.size(), 1);
  std::shared_ptr<SimpleSelector> miscSelector = std::static_pointer_cast<SimpleSelector>(selectors4.at(0));
  ASSERT_EQ(miscSelector->getTagName(), "hoge");
  ASSERT_EQ(miscSelector->getClasses().size(), 2);
  ASSERT_EQ(miscSelector->getClasses().at(0), "fuga");
  ASSERT_EQ(miscSelector->getClasses().at(1), "foo");
  ASSERT_EQ(miscSelector->getId().value(), "bar");
}
