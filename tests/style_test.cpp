#include <gtest/gtest.h>

#include "ivy.h"
#include "test_util.h"

TEST(style_test, minimal)
{
  std::shared_ptr<Node> htmlNode = parseHtml("html/minimal.html");
  std::shared_ptr<Stylesheet> stylesheet = parseCSS("css/minimal.css");

  StyledNodeBuilder styledNodeBuilder;
  std::shared_ptr<StyledNode> styledNode = styledNodeBuilder.buildStyledNode(htmlNode, stylesheet);

  ASSERT_EQ(0, 0);
}
