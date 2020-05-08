// Copyright (c) 2015-2018 Daniel Keefe, TAs, & Regents of the University of
// Minnesota.

#include <math.h>
#include <iostream>
#include <fstream>
#include "gtest/gtest.h"
#include "imagetools/tool_pen.h"

namespace image_tools {

class ToolPenTest : public ::testing::Test {
  void SetUp() override {}

 protected:
  ToolPen pen_;
};

TEST_F(ToolPenTest, HasCorrectName) {
  EXPECT_EQ(pen_.name(), "Pen");
}

TEST_F(ToolPenTest, AppliesPaintWhenStationary) {
  EXPECT_FALSE(pen_.applies_paint_when_stationary())
            << "Pen should not apply paint when stationary.";
}
}  // namespace image_tools
