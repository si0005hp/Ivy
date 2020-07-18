#include <iostream>
#include <string>
#include <memory>
#include <numeric>

#include "ivy.h"

float sum(const std::vector<std::shared_ptr<Value>> &vs);

std::shared_ptr<LayoutBox> LayoutBoxBuilder::buildLayoutBox(std::shared_ptr<StyledNode> styledNode, Dimensions &containingBlock)
{
  ASSERT(styledNode->display() == Display::Block, "Root node must be display: block.");

  auto layoutBoxRoot = buildLayoutTree(styledNode);
  layoutBoxRoot->layout(containingBlock);
  return layoutBoxRoot;
}

std::shared_ptr<LayoutBox> LayoutBoxBuilder::buildLayoutTree(std::shared_ptr<StyledNode> styledNode)
{
  auto root = newLayoutBoxRoot(styledNode);

  for (size_t i = 0; i < styledNode->getChildren().size(); i++)
  {
    auto child = styledNode->getChildren().at(i);
    switch (child->display())
    {
    case Display::Block:
      root->getChildren().push_back(buildLayoutTree(child));
    case Display::Inline:
      root->getInlineContainer()->getChildren().push_back(buildLayoutTree(child));
    }
  }
  return root;
}

std::shared_ptr<LayoutBox> LayoutBoxBuilder::newLayoutBoxRoot(std::shared_ptr<StyledNode> styledNode)
{
  switch (styledNode->display())
  {
  case Display::Block:
    return std::make_shared<BlockNode>(styledNode);
  case Display::Inline:
    return std::make_shared<InlineNode>(styledNode);
  default:
    throw std::runtime_error("Root node has display: none.");
  }
}

std::shared_ptr<LayoutBox> LayoutBox::getInlineContainer()
{
  switch (type)
  {
  case BoxType::Inline:
  case BoxType::AnonymousBlock:
    return std::make_shared<LayoutBox>(*this);
  case BoxType::Block:
    // Create AnonymousBlock if it's not there.
    if (this->getChildren().back()->getType() != BoxType::AnonymousBlock)
    {
      this->getChildren().push_back(std::make_shared<AnonymousBlock>());
    }
    return this->getChildren().back();
  }
}

void BlockNode::layout(const Dimensions &containingBlock)
{
  calculateBlockWidth(containingBlock);
  calculateBlockPosition(containingBlock);
  layoutBlockChildren();
  calculateBlockHeight();
}

/// Calculate the width of a block-level non-replaced element in normal flow.
///
/// http://www.w3.org/TR/CSS2/visudet.html#blockwidth
///
/// Sets the horizontal margin/padding/border dimensions, and the `width`.
void BlockNode::calculateBlockWidth(const Dimensions &containingBlock)
{
  // `width` has initial value `auto`.
  const auto v_auto = std::make_shared<Keyword>("auto");
  auto width = styledNode->value("width").value_or(v_auto);

  // margin, border, and padding have initial value 0.
  const auto zero = std::make_shared<Length>(0.0f, Unit::Px);

  auto marginLeft = styledNode->lookup("margin-left", "margin", zero);
  auto marginRight = styledNode->lookup("margin-right", "margin", zero);

  auto borderLeft = styledNode->lookup("border-left-width", "border-width", zero);
  auto borderRight = styledNode->lookup("border-right-width", "border-width", zero);

  auto paddingLeft = styledNode->lookup("padding-left", "padding", zero);
  auto paddingRight = styledNode->lookup("padding-right", "padding", zero);

  const auto total = sum({marginLeft, marginRight, borderLeft, borderRight, paddingLeft, paddingRight, width});

  // If width is not auto and the total is wider than the container, treat auto margins as 0.
  if (width != v_auto && total > containingBlock.content.width)
  {
    if (marginLeft == v_auto)
      marginLeft = zero;

    if (marginRight == v_auto)
      marginRight = zero;
  }

  // Adjust used values so that the above sum equals `containing_block.width`.
  // Each conditions should increase the total width by exactly `underflow`, and afterward all values should be absolute lengths in px.
  auto underflow = containingBlock.content.width - total;

  // If the values are overconstrained, calculate margin_right.
  if (width != v_auto && marginRight != v_auto && marginLeft != v_auto)
  {
    marginRight = std::make_shared<Length>(marginRight->toPx() + underflow, Unit::Px);
  }
  // If exactly one size is auto, its used value follows from the equality.
  else if (width != v_auto && marginRight != v_auto && marginLeft == v_auto)
  {
    marginRight = std::make_shared<Length>(underflow, Unit::Px);
  }
  else if (width != v_auto && marginRight == v_auto && marginLeft != v_auto)
  {
    marginLeft = std::make_shared<Length>(underflow, Unit::Px);
  }
  // If width is set to auto, any other auto values become 0.
  else if (width == v_auto)
  {
    if (marginRight == v_auto)
      marginRight = zero;

    if (marginLeft == v_auto)
      marginLeft = zero;

    if (underflow > 0.0f)
    {
      width = std::make_shared<Length>(underflow, Unit::Px);
    }
    else
    {
      width = zero;
      marginRight = std::make_shared<Length>(marginRight->toPx() + underflow, Unit::Px);
    }
  }
  // If margin-left and margin-right are both auto, their used values are equal.
  else if (width != v_auto && marginRight == v_auto && marginLeft == v_auto)
  {
    marginRight = std::make_shared<Length>(underflow / 2.0f, Unit::Px);
    marginLeft = std::make_shared<Length>(underflow / 2.0f, Unit::Px);
  }

  dimensions.content.width = width->toPx();
  dimensions.padding.left = paddingLeft->toPx();
  dimensions.padding.right = paddingRight->toPx();
  dimensions.border.left = borderLeft->toPx();
  dimensions.border.right = borderRight->toPx();
  dimensions.margin.left = marginLeft->toPx();
  dimensions.margin.right = marginRight->toPx();
}

/// Finish calculating the block's edge sizes, and position it within its containing block.
///
/// http://www.w3.org/TR/CSS2/visudet.html#normal-block
///
/// Sets the vertical margin/padding/border dimensions, and the `x`, `y` values.
void BlockNode::calculateBlockPosition(const Dimensions &containingBlock)
{
  // margin, border, and padding have initial value 0.
  const auto zero = std::make_shared<Length>(0.0f, Unit::Px);

  // If margin-top or margin-bottom is `auto`, the used value is zero.
  dimensions.margin.top = styledNode->lookup("margin-top", "margin", zero)->toPx();
  dimensions.margin.bottom = styledNode->lookup("margin-bottom", "margin", zero)->toPx();

  dimensions.border.top = styledNode->lookup("border-top-width", "border-width", zero)->toPx();
  dimensions.border.bottom = styledNode->lookup("border-bottom-width", "border-width", zero)->toPx();

  dimensions.padding.top = styledNode->lookup("padding-top", "padding", zero)->toPx();
  dimensions.padding.bottom = styledNode->lookup("padding-bottom", "padding", zero)->toPx();

  dimensions.content.x = containingBlock.content.x + dimensions.margin.left + dimensions.border.left + dimensions.padding.left;
  // Position the box below all the previous boxes in the container.
  dimensions.content.y = containingBlock.content.height + containingBlock.content.y +
                         dimensions.margin.top + dimensions.border.top + dimensions.padding.top;
}

/// Lay out the block's children within its content area.
///
/// Sets `self.dimensions.height` to the total content height.
void BlockNode::layoutBlockChildren()
{
  for (size_t i = 0; i < children.size(); i++)
  {
    auto child = children.at(i);
    child->layout(dimensions);
    // Increment the height so each child is laid out below the previous one.
    dimensions.content.height = dimensions.content.height + child->getDimensions().margingBox().height;
  }
}

/// Height of a block-level non-replaced element in normal flow with overflow visible.
void BlockNode::calculateBlockHeight()
{
  std::optional<std::shared_ptr<Value>> heightOpt = styledNode->value("height");
  if (heightOpt.has_value())
    dimensions.content.height = std::static_pointer_cast<Length>(heightOpt.value())->getValue();
}

const Rect &Rect::expandedBy(const EdgeSizes &edge) const
{
  return Rect(
      x - edge.left,
      y - edge.top,
      width + edge.left + edge.right,
      height + edge.top + edge.bottom);
}

const Rect &Dimensions::paddingBox() const
{
  return content.expandedBy(padding);
}

const Rect &Dimensions::borderBox() const
{
  return paddingBox().expandedBy(border);
}

const Rect &Dimensions::margingBox() const
{
  return borderBox().expandedBy(margin);
}

void AnonymousBlock::layout(const Dimensions &containingBlock)
{
  // TODO
}

float sum(const std::vector<std::shared_ptr<Value>> &vs)
{
  std::vector<float> fs(vs.size());
  transform(vs.begin(), vs.end(), fs.begin(), [](auto v) { return v->toPx(); });
  return accumulate(begin(fs), end(fs), 0, [](auto v1, auto v2) { return v1 + v2; });
}
