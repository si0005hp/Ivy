#include <gtest/gtest.h>

#include "ivy.h"
#include "test_util.h"

TEST(style_test, minimal)
{
  std::shared_ptr<Node> htmlNode = parseHtml("html/minimal.html");
  std::shared_ptr<Stylesheet> stylesheet = parseCSS("css/minimal.css");

  StyledNodeBuilder styledNodeBuilder;
  std::shared_ptr<StyledNode> root = styledNodeBuilder.buildStyledNode(htmlNode, stylesheet);

  ASSERT_EQ(std::static_pointer_cast<ElementNode>(root->getNode())->getTagName(), "html");
  ASSERT_EQ(root->getChildren().size(), 0);
  ASSERT_EQ(root->getSpecifiedValues().size(), 1);
  ASSERT_EQ(std::static_pointer_cast<Keyword>(root->getSpecifiedValues()["margin"])->getKeyword(), "auto");
}

TEST(style_test, basic)
{
  std::shared_ptr<Node> htmlNode = parseHtml("html/attributes2.html");
  std::shared_ptr<Stylesheet> stylesheet = parseCSS("css/selectors.css");

  StyledNodeBuilder styledNodeBuilder;
  std::shared_ptr<StyledNode> root = styledNodeBuilder.buildStyledNode(htmlNode, stylesheet);

  ASSERT_EQ(root->getSpecifiedValues().size(), 0);
  ASSERT_EQ(root->getChildren().size(), 2);

  auto div1 = root->getChildren().at(0);
  ASSERT_EQ(std::static_pointer_cast<ElementNode>(div1->getNode())->getTagName(), "div");
  ASSERT_EQ(div1->getSpecifiedValues().size(), 1);
  ASSERT_TRUE(div1->getSpecifiedValues().find("border-color") != div1->getSpecifiedValues().end());
  ASSERT_EQ(div1->getChildren().size(), 1);

  auto button = div1->getChildren().at(0);
  ASSERT_EQ(std::static_pointer_cast<ElementNode>(button->getNode())->getTagName(), "button");
  ASSERT_EQ(button->getSpecifiedValues().size(), 1);
  ASSERT_EQ(std::static_pointer_cast<Length>(button->getSpecifiedValues()["padding"])->getValue(), 50);
  ASSERT_EQ(button->getChildren().size(), 0);

  auto div2 = root->getChildren().at(1);
  ASSERT_EQ(std::static_pointer_cast<ElementNode>(div2->getNode())->getTagName(), "div");
  ASSERT_EQ(div2->getSpecifiedValues().size(), 1);
  ASSERT_EQ(std::static_pointer_cast<Keyword>(div2->getSpecifiedValues()["display"])->getKeyword(), "block");
  ASSERT_EQ(div2->getChildren().size(), 1);

  auto text = div2->getChildren().at(0);
  ASSERT_EQ(std::static_pointer_cast<TextNode>(text->getNode())->getText(), "blah");
  ASSERT_EQ(text->getSpecifiedValues().size(), 0);
  ASSERT_EQ(text->getChildren().size(), 0);
}

TEST(style_test, specificity)
{
  std::shared_ptr<Node> htmlNode = parseHtml("html/attributes3.html");
  std::shared_ptr<Stylesheet> stylesheet = parseCSS("css/selectors.css");

  StyledNodeBuilder styledNodeBuilder;
  std::shared_ptr<StyledNode> root = styledNodeBuilder.buildStyledNode(htmlNode, stylesheet);

  ASSERT_EQ(root->getSpecifiedValues().size(), 1);
  auto color = std::static_pointer_cast<Color>(root->getSpecifiedValues()["border-color"]);
  ASSERT_EQ(color->getR(), 204);
  ASSERT_EQ(color->getG(), 0);
  ASSERT_EQ(color->getB(), 0);

  ASSERT_EQ(root->getChildren().size(), 0);
}
