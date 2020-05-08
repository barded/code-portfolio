// Copyright (c) 2015-2018 Daniel Keefe, TAs, & Regents of the University of
// Minnesota.

#include <math.h>
#include <iostream>
#include <fstream>
#include "gtest/gtest.h"
#include "imagetools/tool_eraser.h"

namespace image_tools {

class ToolEraserTest : public ::testing::Test {
  void SetUp() override {}

 protected:
  ToolEraser eraser_;
};

TEST_F(ToolEraserTest, HasCorrectName) {
  EXPECT_EQ(eraser_.name(), "Eraser");
}

TEST_F(ToolEraserTest, AppliesPaintWhenStationary) {
  EXPECT_FALSE(eraser_.applies_paint_when_stationary())
            << "Eraser should not apply paint when stationary.";
}
}  // namespace image_tools
