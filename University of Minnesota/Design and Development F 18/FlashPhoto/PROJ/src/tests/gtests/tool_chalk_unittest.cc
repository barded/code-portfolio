// Copyright Deandra Bardell 2018

#include <math.h>
#include <iostream>
#include <fstream>
#include "gtest/gtest.h"
#include "imagetools/tool_chalk.h"

namespace image_tools {

class ToolChalkTest : public ::testing::Test {
  void SetUp() override {}

 protected:
  ToolChalk chalk_;
};

TEST_F(ToolChalkTest, HasCorrectName) {
  EXPECT_EQ(chalk_.name(), "Chalk");
}

TEST_F(ToolChalkTest, AppliesPaintWhenStationary) {
  EXPECT_FALSE(chalk_.applies_paint_when_stationary())
            << "Chalk should not apply paint when stationary.";
}

}  // namespace image_tools
