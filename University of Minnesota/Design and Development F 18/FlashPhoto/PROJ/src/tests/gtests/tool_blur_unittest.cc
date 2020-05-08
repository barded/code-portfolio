// Copyright (c) 2015-2018 Daniel Keefe, TAs, & Regents of the University of
// Minnesota.

#include <math.h>
#include <iostream>
#include <fstream>
#include "gtest/gtest.h"
#include "imagetools/tool_blur.h"

namespace image_tools {

class ToolBlurTest : public ::testing::Test {
  void SetUp() override {}

 protected:
  ToolBlur blur_;
};

TEST_F(ToolBlurTest, HasCorrectName) {
  EXPECT_EQ(blur_.name(), "Blur");
}

TEST_F(ToolBlurTest, AppliesPaintWhenStationary) {
  EXPECT_FALSE(blur_.applies_paint_when_stationary())
            << "Blur should not apply paint when stationary.";
}

}  // namespace image_tools
