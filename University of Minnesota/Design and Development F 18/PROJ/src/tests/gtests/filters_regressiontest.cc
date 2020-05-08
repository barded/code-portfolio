// Copyright (c) 2015-2018 Daniel Keefe, TAs, & Regents of the University of
// Minnesota.

#include <math.h>
#include <iostream>
#include <fstream>
#include "gtest/gtest.h"
#include "imagetools/image_editor.h"
#include "imagetools/pixel_buffer.h"
#include "imagetools/color_data.h"

// filters
#include "imagetools/convolution_filter_blur.h"
#include "imagetools/convolution_filter_edge.h"
#include "imagetools/convolution_filter_motion_blur.h"
#include "imagetools/convolution_filter_sharpen.h"
#include "imagetools/filter_channels.h"
#include "imagetools/filter_quantize.h"
#include "imagetools/filter_saturate.h"
#include "imagetools/filter_threshold.h"


namespace image_tools {

class FiltersRegressionTest : public ::testing::Test {
  void SetUp() override {}
};

TEST_F(FiltersRegressionTest, Blur) {
  ImageEditor *image_editor;
  PixelBuffer *buffer = new PixelBuffer("resources/truth/flower.png");
  image_editor = new ImageEditor(buffer);

  image_editor->ApplyBlurFilter(5.0);

  image_editor->SaveToFile("resources/blur5-test.png");
  PixelBuffer truth("resources/truth/blur5.png");
  PixelBuffer saturday("resources/sat/blur5-test-sat.png");

  std::ifstream ifile("resources/blur5-test.png");

  ASSERT_TRUE(static_cast<bool>(ifile)) << "blur5-test.png didn't save.";
  ifile.close();


  PixelBuffer today("resources/blur5-test.png");


  EXPECT_TRUE(truth == saturday)
         << "truth/blur5.png and Saturday blur5.png don't match.";

  EXPECT_TRUE(truth == today)
        << "truth/blur5.png and today's blur5.png don't match.";
}

TEST_F(FiltersRegressionTest, EdgeDetect) {
  ImageEditor *image_editor;
  PixelBuffer *buffer = new PixelBuffer("resources/truth/flower.png");
  image_editor = new ImageEditor(buffer);

  image_editor->ApplyEdgeDetectFilter();

  image_editor->SaveToFile("resources/edge-test.png");
  PixelBuffer truth("resources/truth/edge.png");
  PixelBuffer saturday("resources/sat/edge-test-sat.png");

  std::ifstream ifile("resources/edge-test.png");

  ASSERT_TRUE(static_cast<bool>(ifile)) << "edge-test.png didn't save.";
  ifile.close();

  PixelBuffer today("resources/edge-test.png");

  EXPECT_TRUE(truth == saturday)
         << "truth/edge.png and Saturday edge.png don't match.";

  EXPECT_TRUE(truth == today)
        << "truth/edge.png and today's edge.png don't match.";
}

TEST_F(FiltersRegressionTest, MotionBlurNS) {
  ImageEditor *image_editor;
  PixelBuffer *buffer = new PixelBuffer("resources/truth/flower.png");
  image_editor = new ImageEditor(buffer);

  image_editor->ApplyMotionBlurFilter(5.0,
                                    ConvolutionFilterMotionBlur::BLUR_DIR_N_S);

  image_editor->SaveToFile("resources/mblurns5-test.png");
  PixelBuffer truth("resources/truth/mblurns5.png");
  PixelBuffer saturday("resources/sat/mblurns5-test-sat.png");

  std::ifstream ifile("resources/mblurns5-test.png");

  ASSERT_TRUE(static_cast<bool>(ifile)) << "mblurns5-test.png didn't save.";
  ifile.close();

  PixelBuffer today("resources/mblurns5-test.png");

  EXPECT_TRUE(truth == saturday)
         << "truth/mblurns5.png and Saturday mblurns5.png don't match.";

  EXPECT_TRUE(truth == today)
        << "truth/mblurns5.png and today's mblurns5.png don't match.";
}

TEST_F(FiltersRegressionTest, MotionBlurEW) {
  ImageEditor *image_editor;
  PixelBuffer *buffer = new PixelBuffer("resources/truth/flower.png");
  image_editor = new ImageEditor(buffer);

  image_editor->ApplyMotionBlurFilter(5.0,
                                    ConvolutionFilterMotionBlur::BLUR_DIR_E_W);

  image_editor->SaveToFile("resources/mblurew5-test.png");
  PixelBuffer truth("resources/truth/mblurew5.png");
  PixelBuffer saturday("resources/sat/mblurew5-test-sat.png");

  std::ifstream ifile("resources/mblurew5-test.png");

  ASSERT_TRUE(static_cast<bool>(ifile)) << "mblurew5-test.png didn't save.";
  ifile.close();

  PixelBuffer today("resources/mblurew5-test.png");

  EXPECT_TRUE(truth == saturday)
         << "truth/mblurew5.png and Saturday mblurew5.png don't match.";

  EXPECT_TRUE(truth == today)
        << "truth/mblurew5.png and today's mblurew5.png don't match.";
}

TEST_F(FiltersRegressionTest, MotionBlurNESW) {
  ImageEditor *image_editor;
  PixelBuffer *buffer = new PixelBuffer("resources/truth/flower.png");
  image_editor = new ImageEditor(buffer);

  image_editor->ApplyMotionBlurFilter(5.0,
                                  ConvolutionFilterMotionBlur::BLUR_DIR_NE_SW);

  image_editor->SaveToFile("resources/mblurnesw5-test.png");
  PixelBuffer truth("resources/truth/mblurnesw5.png");
  PixelBuffer saturday("resources/sat/mblurnesw5-test-sat.png");

  std::ifstream ifile("resources/mblurnesw5-test.png");

  ASSERT_TRUE(static_cast<bool>(ifile)) << "mblurnesw5-test.png didn't save.";
  ifile.close();

  PixelBuffer today("resources/mblurnesw5-test.png");

  EXPECT_TRUE(truth == saturday)
         << "truth/mblurnesw5.png and Saturday mblurnesw5.png don't match.";

  EXPECT_TRUE(truth == today)
        << "truth/mblurnesw5.png and today's mblurnesw5.png don't match.";
}

TEST_F(FiltersRegressionTest, MotionBlurNWSE) {
  ImageEditor *image_editor;
  PixelBuffer *buffer = new PixelBuffer("resources/truth/flower.png");
  image_editor = new ImageEditor(buffer);

  image_editor->ApplyMotionBlurFilter(5.0,
                                  ConvolutionFilterMotionBlur::BLUR_DIR_NW_SE);

  image_editor->SaveToFile("resources/mblurnwse5-test.png");
  PixelBuffer truth("resources/truth/mblurnwse5.png");
  PixelBuffer saturday("resources/sat/mblurnwse5-test-sat.png");

  std::ifstream ifile("resources/mblurnwse5-test.png");

  ASSERT_TRUE(static_cast<bool>(ifile)) << "mblurnwse5-test.png didn't save.";
  ifile.close();

  PixelBuffer today("resources/mblurnwse5-test.png");

  EXPECT_TRUE(truth == saturday)
         << "truth/mblurnwse5.png and Saturday mblurnwse5.png don't match.";

  EXPECT_TRUE(truth == today)
        << "truth/mblurnwse5.png and today's mblurnwse5.png don't match.";
}

TEST_F(FiltersRegressionTest, ChannelsRed) {
  ImageEditor *image_editor;
  PixelBuffer *buffer = new PixelBuffer("resources/truth/flower.png");
  image_editor = new ImageEditor(buffer);

  image_editor->ApplyChannelsFilter(0.5, 1.0, 1.0);

  image_editor->SaveToFile("resources/redp5-test.png");
  PixelBuffer truth("resources/truth/redp5.png");
  PixelBuffer saturday("resources/sat/redp5-test-sat.png");

  std::ifstream ifile("resources/redp5-test.png");

  ASSERT_TRUE(static_cast<bool>(ifile)) << "redp5-test.png didn't save.";
  ifile.close();

  PixelBuffer today("resources/redp5-test.png");

  EXPECT_TRUE(truth == saturday)
         << "truth/redp5.png and Saturday redp5.png don't match.";

  EXPECT_TRUE(truth == today)
        << "truth/redp5.png and today's redp5.png don't match.";
}

TEST_F(FiltersRegressionTest, ChannelsGreen) {
  ImageEditor *image_editor;
  PixelBuffer *buffer = new PixelBuffer("resources/truth/flower.png");
  image_editor = new ImageEditor(buffer);

  image_editor->ApplyChannelsFilter(1.0, 0.5, 1.0);

  image_editor->SaveToFile("resources/greenp5-test.png");
  PixelBuffer truth("resources/truth/greenp5.png");
  PixelBuffer saturday("resources/sat/greenp5-test-sat.png");

  std::ifstream ifile("resources/greenp5-test.png");

  ASSERT_TRUE(static_cast<bool>(ifile)) << "greenp5-test.png didn't save.";
  ifile.close();

  PixelBuffer today("resources/greenp5-test.png");

  EXPECT_TRUE(truth == saturday)
         << "truth/greenp5.png and Saturday greenp5.png don't match.";

  EXPECT_TRUE(truth == today)
        << "truth/greenp5.png and today's greenp5.png don't match.";
}

TEST_F(FiltersRegressionTest, ChannelsBlue) {
  ImageEditor *image_editor;
  PixelBuffer *buffer = new PixelBuffer("resources/truth/flower.png");
  image_editor = new ImageEditor(buffer);

  image_editor->ApplyChannelsFilter(1.0, 1.0, 0.5);

  image_editor->SaveToFile("resources/bluep5-test.png");
  PixelBuffer truth("resources/truth/bluep5.png");
  PixelBuffer saturday("resources/sat/bluep5-test-sat.png");

  std::ifstream ifile("resources/bluep5-test.png");

  ASSERT_TRUE(static_cast<bool>(ifile)) << "bluep5-test.png didn't save.";
  ifile.close();

  PixelBuffer today("resources/bluep5-test.png");

  EXPECT_TRUE(truth == saturday)
         << "truth/bluep5.png and Saturday bluep5.png don't match.";

  EXPECT_TRUE(truth == today)
        << "truth/bluep5.png and today's bluep5.png don't match.";
}

TEST_F(FiltersRegressionTest, Threshold) {
  ImageEditor *image_editor;
  PixelBuffer *buffer = new PixelBuffer("resources/truth/flower.png");
  image_editor = new ImageEditor(buffer);

  image_editor->ApplyThresholdFilter(0.5);

  image_editor->SaveToFile("resources/thresholdp5-test.png");
  PixelBuffer truth("resources/truth/thresholdp5.png");
  PixelBuffer saturday("resources/sat/thresholdp5-test-sat.png");

  std::ifstream ifile("resources/thresholdp5-test.png");

  ASSERT_TRUE(static_cast<bool>(ifile)) << "thresholdp5-test.png didn't save.";
  ifile.close();

  PixelBuffer today("resources/thresholdp5-test.png");

  EXPECT_TRUE(truth == saturday)
         << "truth/thresholdp5.png and Saturday thresholdp5.png don't match.";

  EXPECT_TRUE(truth == today)
        << "truth/thresholdp5.png and today's thresholdp5.png don't match.";
}

TEST_F(FiltersRegressionTest, Saturate) {
  ImageEditor *image_editor;
  PixelBuffer *buffer = new PixelBuffer("resources/truth/flower.png");
  image_editor = new ImageEditor(buffer);

  image_editor->ApplySaturateFilter(0.0);

  image_editor->SaveToFile("resources/saturate0-test.png");
  PixelBuffer truth("resources/truth/saturate0.png");
  PixelBuffer saturday("resources/sat/saturate0-test-sat.png");

  std::ifstream ifile("resources/saturate0-test.png");

  ASSERT_TRUE(static_cast<bool>(ifile)) << "saturate0-test.png didn't save.";
  ifile.close();

  PixelBuffer today("resources/saturate0-test.png");

  EXPECT_TRUE(truth == saturday)
         << "truth/saturate0.png and Saturday saturate0.png don't match.";

  EXPECT_TRUE(truth == today)
        << "truth/saturate0.png and today's saturate0.png don't match.";
}

TEST_F(FiltersRegressionTest, Quantize) {
  ImageEditor *image_editor;
  PixelBuffer *buffer = new PixelBuffer("resources/truth/flower.png");
  image_editor = new ImageEditor(buffer);

  image_editor->ApplyQuantizeFilter(2);

  image_editor->SaveToFile("resources/quan2-test.png");
  PixelBuffer truth("resources/truth/quan2.png");
  PixelBuffer saturday("resources/sat/quan2-test-sat.png");

  std::ifstream ifile("resources/quan2-test.png");

  ASSERT_TRUE(static_cast<bool>(ifile)) << "quan2-test.png didn't save.";
  ifile.close();

  PixelBuffer today("resources/quan2-test.png");

  EXPECT_TRUE(truth == saturday)
         << "truth/quan2.png and Saturday quan2.png don't match.";

  EXPECT_TRUE(truth == today)
        << "truth/quan2.png and today's quan2.png don't match.";
}

TEST_F(FiltersRegressionTest, Sharpen) {
  ImageEditor *image_editor;
  PixelBuffer *buffer = new PixelBuffer("resources/truth/flower.png");
  image_editor = new ImageEditor(buffer);

  image_editor->ApplySharpenFilter(5.0);

  image_editor->SaveToFile("resources/sharpen5-test.png");
  PixelBuffer truth("resources/truth/sharpen5.png");
  PixelBuffer saturday("resources/sat/sharpen5-test-sat.png");

  std::ifstream ifile("resources/sharpen5-test.png");

  ASSERT_TRUE(static_cast<bool>(ifile)) << "sharpen5-test.png didn't save.";
  ifile.close();

  PixelBuffer today("resources/sharpen5-test.png");

  EXPECT_TRUE(truth == saturday)
         << "truth/sharpen5.png and Saturday sharpen5.png don't match.";

  EXPECT_TRUE(truth == today)
        << "truth/sharpen5.png and today's sharpen5.png don't match.";
}
}  // namespace image_tools
