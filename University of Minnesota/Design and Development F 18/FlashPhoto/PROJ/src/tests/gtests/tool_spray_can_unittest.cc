// Copyright Deandra Bardell 2018


#include <math.h>
#include <iostream>
#include <fstream>
#include "gtest/gtest.h"
#include "imagetools/tool_spray_can.h"

namespace image_tools {

class ToolSprayCanTest : public ::testing::Test {
  void SetUp() override {}

 protected:
  ToolSprayCan spray_can_;
};

TEST_F(ToolSprayCanTest, HasCorrectName) {
  EXPECT_EQ(spray_can_.name(), "Spray Can");
}

TEST_F(ToolSprayCanTest, AppliesPaintWhenStationary) {
  EXPECT_TRUE(spray_can_.applies_paint_when_stationary())
            << "Spray Can should apply paint when stationary.";
}
}  // namespace image_tools
