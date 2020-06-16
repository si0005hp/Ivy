#include <gtest/gtest.h>

#include "ivy.h"
#include "test_util.h"

TEST(html_parse_test, minimal)
{
  std::shared_ptr<ElementNode> root = parseHtml("html/minimal.html");

  ASSERT_EQ(root->getTagName(), "html");
  ASSERT_EQ(root->getChildren().size(), 0);
}

TEST(html_parse_test, basic_elem_and_text)
{
  std::shared_ptr<ElementNode> root = parseHtml("html/basic_elem_and_text.html");
  ASSERT_EQ(root->getTagName(), "html");
  ASSERT_EQ(root->getChildren().size(), 2);

  std::shared_ptr<ElementNode> head = std::static_pointer_cast<ElementNode>(root->getChildren().at(0));
  ASSERT_EQ(head->getTagName(), "head");
  ASSERT_EQ(head->getChildren().size(), 0);

  std::shared_ptr<ElementNode> body = std::static_pointer_cast<ElementNode>(root->getChildren().at(1));
  ASSERT_EQ(body->getTagName(), "body");
  ASSERT_EQ(body->getChildren().size(), 1);

  std::shared_ptr<ElementNode> p = std::static_pointer_cast<ElementNode>(body->getChildren().at(0));
  ASSERT_EQ(p->getTagName(), "p");
  ASSERT_EQ(p->getChildren().size(), 1);

  std::shared_ptr<TextNode> pText = std::static_pointer_cast<TextNode>(p->getChildren().at(0));
  ASSERT_EQ(pText->getText(), "HELLO");
}

TEST(html_parse_test, attributes)
{
  std::shared_ptr<ElementNode> root = parseHtml("html/attributes.html");
  ASSERT_EQ(root->getTagName(), "html");
  ASSERT_EQ(root->getChildren().size(), 2);

  std::shared_ptr<ElementNode> button1 = std::static_pointer_cast<ElementNode>(root->getChildren().at(0));
  ASSERT_EQ(button1->getAttrMap().size(), 2);
  ASSERT_EQ(button1->getAttrMap()["id"], "1");
  ASSERT_EQ(button1->getAttrMap()["class"], "primary btn btn");

  std::shared_ptr<ElementNode> button2 = std::static_pointer_cast<ElementNode>(root->getChildren().at(1));
  ASSERT_EQ(button2->getAttrMap().size(), 1);
  ASSERT_EQ(button2->getAttrMap()["id"], "2");
}

TEST(html_elem_test, accessors)
{
  std::shared_ptr<ElementNode> root = parseHtml("html/attributes.html");

  std::shared_ptr<ElementNode> button1 = std::static_pointer_cast<ElementNode>(root->getChildren().at(0));
  ASSERT_EQ(button1->id().value(), "1");
  ASSERT_EQ(button1->classes().size(), 2);
  ASSERT_EQ(button1->classes().count("primary"), 1);
  ASSERT_EQ(button1->classes().count("btn"), 1);

  std::shared_ptr<ElementNode> button2 = std::static_pointer_cast<ElementNode>(root->getChildren().at(1));
  ASSERT_EQ(button2->id().value(), "2");
  ASSERT_EQ(button2->classes().size(), 0);
}
