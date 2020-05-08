// Copyright (c) 2015-2018 Daniel Keefe, TAs, & Regents of the University of
// Minnesota.

#include <math.h>
#include <iostream>
#include <fstream>
#include "gtest/gtest.h"
#include "imagetools/tool_calligraphy_pen.h"

namespace image_tools {

class ToolCalligraphyPenTest : public ::testing::Test {
  void SetUp() override {}

 protected:
  ToolCalligraphyPen calligraphy_pen_;
};

TEST_F(ToolCalligraphyPenTest, HasCorrectName) {
  EXPECT_EQ(calligraphy_pen_.name(), "Calligraphy Pen");
}

TEST_F(ToolCalligraphyPenTest, AppliesPaintWhenStationary) {
  EXPECT_FALSE(calligraphy_pen_.applies_paint_when_stationary())
            << "Calligraphy Pen should not apply paint when stationary.";
}
}  // namespace image_tools
