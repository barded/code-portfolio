// Copyright Deandra Bardell 2018

#include <math.h>
#include <iostream>
#include <fstream>
#include "gtest/gtest.h"
#include "imagetools/tool_highlighter.h"

namespace image_tools {

class ToolHighlighterTest : public ::testing::Test {
  void SetUp() override {}

 protected:
  ToolHighlighter highlighter_;
};

TEST_F(ToolHighlighterTest, HasCorrectName) {
  EXPECT_EQ(highlighter_.name(), "Highlighter");
}

TEST_F(ToolHighlighterTest, AppliesPaintWhenStationary) {
  EXPECT_FALSE(highlighter_.applies_paint_when_stationary())
            << "Highlighter should not apply paint when stationary.";
}
}  // namespace image_tools
