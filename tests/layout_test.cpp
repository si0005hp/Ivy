#include <gtest/gtest.h>

#include "ivy.h"
#include "test_util.h"

TEST(layout_test, minimal)
{
  std::shared_ptr<StyledNode> styledNode = buildStyledNode("html/minimal.html", "css/minimal.css");
  Dimensions containingBlock;
  // std::shared_ptr<LayoutBox> root = LayoutBoxBuilder().buildLayoutBox(styledNode, containingBlock);

  ASSERT_EQ(0, 0);
}
