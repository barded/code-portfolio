// Copyright (c) 2015-2018 Daniel Keefe, TAs, & Regents of the University of
// Minnesota.

#include <math.h>
#include <iostream>
#include <fstream>
#include "gtest/gtest.h"
#include "imagetools/tool_stamp.h"

namespace image_tools {

class ToolStampTest : public ::testing::Test {
  void SetUp() override {}

 protected:
  ToolStamp stamp_;
};

TEST_F(ToolStampTest, HasCorrectName) {
  EXPECT_EQ(stamp_.name(), "Stamp");
}

TEST_F(ToolStampTest, AppliesPaintWhenStationary) {
  EXPECT_FALSE(stamp_.applies_paint_when_stationary())
            << "Stamp should not apply paint when stationary.";
}
}  // namespace image_tools
