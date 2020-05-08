// Copyright (c) 2015-2018 Daniel Keefe, TAs, & Regents of the University of
// Minnesota.

#include <math.h>
#include <iostream>
#include <fstream>
#include "gtest/gtest.h"
#include "imagetools/image_editor.h"
#include "imagetools/pixel_buffer.h"
#include "imagetools/color_data.h"

// tools
#include "imagetools/tool_blur.h"
#include "imagetools/tool_calligraphy_pen.h"
#include "imagetools/tool_chalk.h"
#include "imagetools/tool_eraser.h"
#include "imagetools/tool_highlighter.h"
#include "imagetools/tool_pen.h"
#include "imagetools/tool_spray_can.h"
#include "imagetools/tool_stamp.h"


namespace image_tools {

class ToolsRegressionTest : public ::testing::Test {
  void SetUp() override {}
};

TEST_F(ToolsRegressionTest, Pen) {
  ToolPen pen;
  ImageEditor *image_editor;
  PixelBuffer *buffer = new PixelBuffer(500, 500, ColorData(1, 1, 1));
  image_editor = new ImageEditor(buffer);
  image_editor->StartStroke(pen.name(), ColorData(.9, .25, .4), 5, 200, 200);

  int x = 200, y;

  for (y = 200; y < 400; y++) {
    image_editor->AddToStroke(x, y);
  }

  for (x = 200; x < 420; x++) {
    image_editor->AddToStroke(x, y);
  }

  for (int i = 0; i < 150; i++) {
    x--;
    y--;
    image_editor->AddToStroke(x, y);
  }
  image_editor->EndStroke(x, y);

  image_editor->SaveToFile("resources/pen-test.png");
  PixelBuffer truth("resources/truth/pen.png");
  PixelBuffer saturday("resources/sat/pen-test-sat.png");

  std::ifstream ifile("resources/pen-test.png");

  ASSERT_TRUE(static_cast<bool>(ifile)) << "pen-test.png didn't save.";
  ifile.close();

  PixelBuffer today("resources/pen-test.png");

  EXPECT_TRUE(truth == saturday)
         << "truth/pen.png and Saturday pen.png don't match.";

  EXPECT_TRUE(truth == today)
        << "truth/pen.png and today's pen.png don't match.";
}


TEST_F(ToolsRegressionTest, Chalk) {
  ToolChalk chalk;
  chalk.testing();  //  sets seed_ to 1
  ImageEditor *image_editor;
  PixelBuffer *buffer = new PixelBuffer(500, 500, ColorData(1, 1, 1));
  image_editor = new ImageEditor(buffer);
  image_editor->StartStroke(chalk.name(), ColorData(.9, .25, .4), 5, 200, 200);

  int x = 200, y;

  for (y = 200; y < 400; y++) {
    image_editor->AddToStroke(x, y);
  }

  for (x = 200; x < 420; x++) {
    image_editor->AddToStroke(x, y);
  }

  for (int i = 0; i < 150; i++) {
    x--;
    y--;
    image_editor->AddToStroke(x, y);
  }
  image_editor->EndStroke(x, y);

  image_editor->SaveToFile("resources/chalk-test.png");
  PixelBuffer truth("resources/truth/chalk.png");
  PixelBuffer saturday("resources/sat/chalk-test-sat.png");

  std::ifstream ifile("resources/chalk-test.png");

  ASSERT_TRUE(static_cast<bool>(ifile)) << "chalk-test.png didn't save.";
  ifile.close();

  PixelBuffer today("resources/chalk-test.png");

  EXPECT_TRUE(truth == saturday)
         << "truth/chalk.png and Saturday chalk.png don't match.";

  EXPECT_TRUE(truth == today)
        << "truth/chalk.png and today's chalk.png don't match.";
}


TEST_F(ToolsRegressionTest, Highlighter) {
  ToolHighlighter highlighter;
  ImageEditor *image_editor;
  PixelBuffer *buffer = new PixelBuffer(500, 500, ColorData(1, 1, 1));
  image_editor = new ImageEditor(buffer);
  image_editor->StartStroke(highlighter.name(),
                            ColorData(.9, .25, .4), 5, 200, 200);

  int x = 200, y;

  for (y = 200; y < 400; y++) {
    image_editor->AddToStroke(x, y);
  }

  for (x = 200; x < 420; x++) {
    image_editor->AddToStroke(x, y);
  }

  for (int i = 0; i < 150; i++) {
    x--;
    y--;
    image_editor->AddToStroke(x, y);
  }
  image_editor->EndStroke(x, y);

  image_editor->SaveToFile("resources/highlighter-test.png");
  PixelBuffer truth("resources/truth/highlighter.png");
  PixelBuffer saturday("resources/sat/highlighter-test-sat.png");

  std::ifstream ifile("resources/highlighter-test.png");

  ASSERT_TRUE(static_cast<bool>(ifile)) << "highlighter-test.png didn't save.";
  ifile.close();

  PixelBuffer today("resources/highlighter-test.png");

  EXPECT_TRUE(truth == saturday)
         << "truth/highlighter.png and Saturday highlighter.png don't match.";

  EXPECT_TRUE(truth == today)
        << "truth/highlighter.png and today's highlighter.png don't match.";
}


TEST_F(ToolsRegressionTest, CalligraphyPen) {
  ToolCalligraphyPen calligraphy_pen;
  ImageEditor *image_editor;
  PixelBuffer *buffer = new PixelBuffer(500, 500, ColorData(1, 1, 1));
  image_editor = new ImageEditor(buffer);
  image_editor->StartStroke(calligraphy_pen.name(),
                            ColorData(.9, .25, .4), 5, 200, 200);

  int x = 200, y;

  for (y = 200; y < 400; y++) {
    image_editor->AddToStroke(x, y);
  }

  for (x = 200; x < 420; x++) {
    image_editor->AddToStroke(x, y);
  }

  for (int i = 0; i < 150; i++) {
    x--;
    y--;
    image_editor->AddToStroke(x, y);
  }
  image_editor->EndStroke(x, y);

  image_editor->SaveToFile("resources/calligraphy_pen-test.png");
  PixelBuffer truth("resources/truth/calligraphy_pen.png");
  PixelBuffer saturday("resources/sat/calligraphy_pen-test-sat.png");

  std::ifstream ifile("resources/calligraphy_pen-test.png");

  ASSERT_TRUE(static_cast<bool>(ifile))
    << "calligraphy_pen-test.png didn't save.";
  ifile.close();

  PixelBuffer today("resources/calligraphy_pen-test.png");

  EXPECT_TRUE(truth == saturday)
    << "truth/calligraphy_pen.png and "
        "Saturday calligraphy_pen.png don't match.";

  EXPECT_TRUE(truth == today)
    << "truth/calligraphy_pen.png and today's calligraphy_pen.png don't match.";
}


TEST_F(ToolsRegressionTest, SprayCan) {
  ToolSprayCan spray_can;
  ImageEditor *image_editor;
  PixelBuffer *buffer = new PixelBuffer(500, 500, ColorData(1, 1, 1));
  image_editor = new ImageEditor(buffer);
  image_editor->StartStroke(spray_can.name(),
                            ColorData(.9, .25, .4), 5, 200, 200);

  int x = 200, y;

  for (y = 200; y < 400; y++) {
    image_editor->AddToStroke(x, y);
  }

  for (x = 200; x < 420; x++) {
    image_editor->AddToStroke(x, y);
  }

  for (int i = 0; i < 150; i++) {
    x--;
    y--;
    image_editor->AddToStroke(x, y);
  }
  image_editor->EndStroke(x, y);

  image_editor->SaveToFile("resources/spray_can-test.png");
  PixelBuffer truth("resources/truth/spray_can.png");
  PixelBuffer saturday("resources/sat/spray_can-test-sat.png");

  std::ifstream ifile("resources/spray_can-test.png");

  ASSERT_TRUE(static_cast<bool>(ifile)) << "spray_can-test.png didn't save.";
  ifile.close();

  PixelBuffer today("resources/spray_can-test.png");

  EXPECT_TRUE(truth == saturday)
         << "truth/spray_can.png and Saturday spray_can.png don't match.";

  EXPECT_TRUE(truth == today)
        << "truth/spray_can.png and today's spray_can.png don't match.";
}


TEST_F(ToolsRegressionTest, Stamp) {
  ToolStamp stamp;
  ImageEditor *image_editor;
  PixelBuffer *buffer = new PixelBuffer(500, 500, ColorData(1, 1, 1));
  image_editor = new ImageEditor(buffer);
  image_editor->StartStroke(stamp.name(), ColorData(.9, .25, .4), 5, 200, 200);

  int x = 200, y;

  for (y = 200; y < 400; y++) {
    image_editor->AddToStroke(x, y);
  }

  for (x = 200; x < 420; x++) {
    image_editor->AddToStroke(x, y);
  }

  for (int i = 0; i < 150; i++) {
    x--;
    y--;
    image_editor->AddToStroke(x, y);
  }
  image_editor->EndStroke(x, y);

  image_editor->StartStroke(stamp.name(), ColorData(.9, .25, .4), 5, 250, 270);
  image_editor->EndStroke(250, 270);

  image_editor->SaveToFile("resources/stamp-test.png");
  PixelBuffer truth("resources/truth/stamp.png");
  PixelBuffer saturday("resources/sat/stamp-test-sat.png");

  std::ifstream ifile("resources/stamp-test.png");

  ASSERT_TRUE(static_cast<bool>(ifile)) << "stamp-test.png didn't save.";
  ifile.close();

  PixelBuffer today("resources/stamp-test.png");

  EXPECT_TRUE(truth == saturday)
         << "truth/stamp.png and Saturday stamp.png don't match.";

  EXPECT_TRUE(truth == today)
        << "truth/stamp.png and today's stamp.png don't match.";
}


TEST_F(ToolsRegressionTest, Blur) {
  ToolBlur blur;
  ToolPen pen;
  ImageEditor *image_editor;
  PixelBuffer *buffer = new PixelBuffer(500, 500, ColorData(1, 1, 1));
  image_editor = new ImageEditor(buffer);
  image_editor->StartStroke(pen.name(), ColorData(.9, .25, .4), 3, 200, 200);

  int x = 200, y;

  for (y = 200; y < 400; y++) {
    image_editor->AddToStroke(x, y);
  }

  for (x = 200; x < 420; x++) {
    image_editor->AddToStroke(x, y);
  }

  for (int i = 0; i < 150; i++) {
    x--;
    y--;
    image_editor->AddToStroke(x, y);
  }
  image_editor->EndStroke(x, y);

  image_editor->StartStroke(blur.name(), ColorData(.9, .25, .4), 5, 200, 200);

  x = 200;

  for (y = 200; y < 400; y++) {
    image_editor->AddToStroke(x, y);
  }

  for (x = 200; x < 420; x++) {
    image_editor->AddToStroke(x, y);
  }

  for (int i = 0; i < 150; i++) {
    x--;
    y--;
    image_editor->AddToStroke(x, y);
  }
  image_editor->EndStroke(x, y);

  image_editor->SaveToFile("resources/blur-test.png");
  PixelBuffer truth("resources/truth/blur.png");
  PixelBuffer saturday("resources/sat/blur-test-sat.png");

  std::ifstream ifile("resources/blur-test.png");

  ASSERT_TRUE(static_cast<bool>(ifile)) << "blur-test.png didn't save.";
  ifile.close();

  PixelBuffer today("resources/blur-test.png");

  EXPECT_TRUE(truth == saturday)
         << "truth/blur.png and Saturday blur.png don't match.";

  EXPECT_TRUE(truth == today)
        << "truth/blur.png and today's blur.png don't match.";
}


TEST_F(ToolsRegressionTest, Eraser) {
  ToolEraser eraser;
  ToolPen pen;
  ImageEditor *image_editor;
  PixelBuffer *buffer = new PixelBuffer(500, 500, ColorData(1, 1, 1));
  image_editor = new ImageEditor(buffer);
  image_editor->StartStroke(pen.name(), ColorData(.9, .25, .4), 7, 200, 200);

  int x = 200, y;

  for (y = 200; y < 400; y++) {
    image_editor->AddToStroke(x, y);
  }

  for (x = 200; x < 420; x++) {
    image_editor->AddToStroke(x, y);
  }

  for (int i = 0; i < 150; i++) {
    x--;
    y--;
    image_editor->AddToStroke(x, y);
  }
  image_editor->EndStroke(x, y);

  image_editor->StartStroke(eraser.name(), ColorData(.9, .25, .4), 5, 200, 200);

  x = 200;

  for (y = 200; y < 400; y++) {
    image_editor->AddToStroke(x, y);
  }

  for (x = 200; x < 420; x++) {
    image_editor->AddToStroke(x, y);
  }

  for (int i = 0; i < 150; i++) {
    x--;
    y--;
    image_editor->AddToStroke(x, y);
  }
  image_editor->EndStroke(x, y);

  image_editor->SaveToFile("resources/eraser-test.png");
  PixelBuffer truth("resources/truth/eraser.png");
  PixelBuffer saturday("resources/sat/eraser-test-sat.png");

  std::ifstream ifile("resources/eraser-test.png");

  ASSERT_TRUE(static_cast<bool>(ifile)) << "eraser-test.png didn't save.";
  ifile.close();

  PixelBuffer today("resources/eraser-test.png");

  EXPECT_TRUE(truth == saturday)
        << "truth/eraser.png and Saturday eraser.png don't match.";

  EXPECT_TRUE(truth == today)
       << "truth/eraser.png and today's eraser.png don't match.";
}
}  // namespace image_tools
