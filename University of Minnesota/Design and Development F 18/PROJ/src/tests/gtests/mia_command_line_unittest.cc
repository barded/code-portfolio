// Copyright Deandra Bardell 2018

#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include "gtest/gtest.h"
#include "mia/command_line_processor.h"
#include "imagetools/pixel_buffer.h"


namespace image_tools {

class MiaCommandLineTest : public ::testing::Test {
  void SetUp() override {}
 protected:
  CommandLineProcessor cmd_processor;
};
TEST_F(MiaCommandLineTest, BadInputOutputFiles) {
  int arc = 3;
  char* args[3];

  std::string a1 = "./mia";
  std::string a2 = "resources/truth/flower.jpg";
  std::string a3 = "resources/testflowerjpg.png";

  args[0] = const_cast<char*>(a1.c_str());
  args[1] = const_cast<char*>(a2.c_str());
  args[2] = const_cast<char*>(a3.c_str());

  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia command line shouldn't accept .jpg files.";

  args[1] = NULL;
  args[2] = NULL;

  a2 = "resources/truth/flower.png";
  a3 = "resources/miabadinputstestflowerjpg.jpg";

  args[1] = const_cast<char*>(a2.c_str());
  args[2] = const_cast<char*>(a3.c_str());

  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia command line shouldn't output .jpg files.";

  args[1] = NULL;
  args[2] = NULL;

  a2 = "resources/truth/Flower.png";
  a3 = "resources/miabadinputstestflower.png";

  args[1] = const_cast<char*>(a2.c_str());
  args[2] = const_cast<char*>(a3.c_str());

  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
          << "Mia command line shouldn't acceess non existing files.";

  arc = 2;
  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
          << "Mia command line should have too few comamnds.";

  arc = 1;
  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
        << "Mia command line should have too few comamnds.";

  arc = 0;
  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
          << "Mia command line should have too few comamnds.";
}

TEST_F(MiaCommandLineTest, GoodInputOutputFiles) {
  int arc = 3;
  char* args[3];

  std::string a1 = "./mia";
  std::string a2 = "resources/truth/flower.png";
  std::string a3 = "resources/miagoodtestout.png";

  args[0] = const_cast<char*>(a1.c_str());
  args[1] = const_cast<char*>(a2.c_str());
  args[2] = const_cast<char*>(a3.c_str());

  ASSERT_EQ(1, cmd_processor.ProcessCommandLine(arc, args))
           << "Failed basic input output test.";

  std::ifstream ifile("resources/miagoodtestout.png");

  ASSERT_TRUE(static_cast<bool>(ifile)) << "miagoodtestout.png didn't save.";
  ifile.close();
  PixelBuffer truth("resources/truth/flower.png");
  PixelBuffer out("resources/miagoodtestout.png");

  EXPECT_TRUE(truth == out)
         << "flower.png and miagoodtestout.png don't match.";

  args[2] = NULL;
  args[2] = NULL;
  a2 = "resources/truth/flower.png";
  a3 = "resources/miagoodtestoutPNG.PNG";
  args[1] = const_cast<char*>(a2.c_str());
  args[2] = const_cast<char*>(a3.c_str());


  ASSERT_EQ(1, cmd_processor.ProcessCommandLine(arc, args))
           << "Failed basic input output .PNG test.";

  std::ifstream ifile2("resources/miagoodtestoutPNG.PNG");

  ASSERT_TRUE(static_cast<bool>(ifile2))
          << "miagoodtestoutPNG.PNG didn't save.";
  ifile2.close();

  PixelBuffer truth2("resources/truth/flower.png");
  PixelBuffer out2("resources/miagoodtestoutPNG.PNG");

  EXPECT_TRUE(truth2 == out2)
         << "flower.png and miagoodtestoutPNG.PNG don't match.";
}

TEST_F(MiaCommandLineTest, BlurFilterCommand) {
  int arc = 5;
  char* args[5];

  std::string a1 = "./mia";
  std::string a2 = "resources/truth/flower.png";
  std::string a3 = "-blur";
  std::string a4 = "5";
  std::string a5 = "resources/mia-blur5.png";

  args[0] = const_cast<char*>(a1.c_str());
  args[1] = const_cast<char*>(a2.c_str());
  args[2] = const_cast<char*>(a3.c_str());
  args[3] = const_cast<char*>(a4.c_str());
  args[4] = const_cast<char*>(a5.c_str());

  EXPECT_EQ(1, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia -blur 5 problem.";

  std::ifstream ifile("resources/mia-blur5.png");

  ASSERT_TRUE(static_cast<bool>(ifile)) << "mia-blur5.png didn't save.";
  ifile.close();

  PixelBuffer truth("resources/truth/blur5.png");
  PixelBuffer today("resources/mia-blur5.png");

  EXPECT_TRUE(truth == today)
          << "truth/blur5.png and mia-blur5.png don't match.";

  for (int i = -4; i <= 0; i+=2) {
    args[3] = NULL;
    a4 = std::to_string(i);
    args[3] = const_cast<char*>(a4.c_str());
    EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
             << "Mia -blur should fail for input " << i;
  }

  args[2] = NULL;
  a3 = "blur";
  args[2] = const_cast<char*>(a3.c_str());
  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia blur 0 should fail.";

  args[2] = NULL;
  args[3] = NULL;
  a3 = "-blur";
  a4 = "a";
  args[2] = const_cast<char*>(a3.c_str());
  args[3] = const_cast<char*>(a4.c_str());
  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
          << "Mia -blur a should fail.";

  arc = 4;
  char *args2[4];
  a3 = "-blur";
  a4 = "resources/mia-blur-fail.png";
  args2[0] = const_cast<char*>(a1.c_str());
  args2[1] = const_cast<char*>(a2.c_str());
  args2[2] = const_cast<char*>(a3.c_str());
  args2[3] = const_cast<char*>(a4.c_str());

  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args2))
           << "Mia -blur *.png should fail.";
}

TEST_F(MiaCommandLineTest, EdgeFilterCommand) {
  int arc = 4;
  char* args[4];

  std::string a1 = "./mia";
  std::string a2 = "resources/truth/flower.png";
  std::string a3 = "-edgedetect";
  std::string a4 = "resources/mia-edge.png";

  args[0] = const_cast<char*>(a1.c_str());
  args[1] = const_cast<char*>(a2.c_str());
  args[2] = const_cast<char*>(a3.c_str());
  args[3] = const_cast<char*>(a4.c_str());

  EXPECT_EQ(1, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia -edgedetect problem.";

  std::ifstream ifile("resources/mia-edge.png");

  ASSERT_TRUE(static_cast<bool>(ifile)) << "mia-edge.png didn't save.";
  ifile.close();

  PixelBuffer truth("resources/truth/edge.png");
  PixelBuffer today("resources/mia-edge.png");

  EXPECT_TRUE(truth == today)
          << "truth/edge.png and mia-edge.png don't match.";

  args[2] = NULL;
  a3 = "-EdgeDetect";
  args[2] = const_cast<char*>(a3.c_str());
  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia -EdgeDetect should fail.";
}

TEST_F(MiaCommandLineTest, SharpenFilterCommand) {
  int arc = 5;
  char* args[5];

  std::string a1 = "./mia";
  std::string a2 = "resources/truth/flower.png";
  std::string a3 = "-sharpen";
  std::string a4 = "5";
  std::string a5 = "resources/mia-sharpen5.png";

  args[0] = const_cast<char*>(a1.c_str());
  args[1] = const_cast<char*>(a2.c_str());
  args[2] = const_cast<char*>(a3.c_str());
  args[3] = const_cast<char*>(a4.c_str());
  args[4] = const_cast<char*>(a5.c_str());

  EXPECT_EQ(1, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia -sharpen 5 problem.";

  std::ifstream ifile("resources/mia-sharpen5.png");

  ASSERT_TRUE(static_cast<bool>(ifile)) << "mia-sharpen5.png didn't save.";
  ifile.close();

  PixelBuffer truth("resources/truth/sharpen5.png");
  PixelBuffer today("resources/mia-sharpen5.png");

  EXPECT_TRUE(truth == today)
          << "truth/sharpen5.png and mia-sharpen5.png don't match.";

  for (int i = -4; i <= 0; i+=2) {
    args[3] = NULL;
    a4 = std::to_string(i);
    args[3] = const_cast<char*>(a4.c_str());
    EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
             << "Mia -sharpen should fail for input " << i;
  }

  args[2] = NULL;
  a3 = "-sharpe";
  args[2] = const_cast<char*>(a3.c_str());
  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia -sharpe 0 should fail.";

  args[2] = NULL;
  args[3] = NULL;
  a3 = "-sharpen";
  a4 = "b";
  args[2] = const_cast<char*>(a3.c_str());
  args[3] = const_cast<char*>(a4.c_str());
  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
         << "Mia -sharpen should fail.";

  arc = 4;
  char *args2[4];
  a3 = "-sharpen";
  a4 = "resources/mia-sharpen-fail.png";
  args2[0] = const_cast<char*>(a1.c_str());
  args2[1] = const_cast<char*>(a2.c_str());
  args2[2] = const_cast<char*>(a3.c_str());
  args2[3] = const_cast<char*>(a4.c_str());

  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args2))
           << "Mia -sharpen *.png should fail.";
}

TEST_F(MiaCommandLineTest, ChannelsFilterCommandRed) {
  int arc = 5;
  char* args[5];

  std::string a1 = "./mia";
  std::string a2 = "resources/truth/flower.png";
  std::string a3 = "-red";
  std::string a4 = ".5";
  std::string a5 = "resources/mia-redp5.png";

  args[0] = const_cast<char*>(a1.c_str());
  args[1] = const_cast<char*>(a2.c_str());
  args[2] = const_cast<char*>(a3.c_str());
  args[3] = const_cast<char*>(a4.c_str());
  args[4] = const_cast<char*>(a5.c_str());

  EXPECT_EQ(1, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia -red .5 problem.";

  std::ifstream ifile("resources/mia-redp5.png");

  ASSERT_TRUE(static_cast<bool>(ifile)) << "mia-redp5.png didn't save.";
  ifile.close();

  PixelBuffer truth("resources/truth/redp5.png");
  PixelBuffer today("resources/mia-redp5.png");

  EXPECT_TRUE(truth == today)
          << "truth/redp5.png and mia-redp5.png don't match.";

  args[4] = NULL;
  a5 = "resources/mia-red-loop.png";
  args[4] = const_cast<char*>(a5.c_str());

  for (float i = -1.2; i < 4.0; i+=4.0) {
    args[3] = NULL;
    a4 = std::to_string(i);
    args[3] = const_cast<char*>(a4.c_str());
    EXPECT_EQ(1, cmd_processor.ProcessCommandLine(arc, args))
             << "Mia -red should pass for input " << i;
  }

  args[3] = NULL;
  args[4] = NULL;
  a4 = std::to_string(0);
  a5 = "resources/mia-red0.png";
  args[3] = const_cast<char*>(a4.c_str());
  args[4] = const_cast<char*>(a5.c_str());
  EXPECT_EQ(1, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia -red 0 should pass.";

  std::ifstream ifile2("resources/mia-red0.png");

  ASSERT_TRUE(static_cast<bool>(ifile2)) << "mia-red0.png didn't save.";
  ifile2.close();

  PixelBuffer truth2("resources/truth/mia-red0-truth.png");
  PixelBuffer out("resources/mia-red0.png");

  EXPECT_TRUE(truth2 == out)
         << "truth2 and mia-red0.png don't match.";
  args[2] = NULL;
  a3 = "-RED";
  args[2] = const_cast<char*>(a3.c_str());
  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia -RED 0 should fail.";

  args[2] = NULL;
  args[3] = NULL;
  a3 = "-red";
  a4 = "c";
  args[2] = const_cast<char*>(a3.c_str());
  args[3] = const_cast<char*>(a4.c_str());
  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
         << "Mia -red c should fail.";


  arc = 4;
  char *args2[4];
  a3 = "-red";
  a4 = "resources/mia-red-fail.png";
  args2[0] = const_cast<char*>(a1.c_str());
  args2[1] = const_cast<char*>(a2.c_str());
  args2[2] = const_cast<char*>(a3.c_str());
  args2[3] = const_cast<char*>(a4.c_str());

  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args2))
           << "Mia -red *.png should fail.";
}

TEST_F(MiaCommandLineTest, ChannelsFilterCommandGreen) {
  int arc = 5;
  char* args[5];

  std::string a1 = "./mia";
  std::string a2 = "resources/truth/flower.png";
  std::string a3 = "-green";
  std::string a4 = ".5";
  std::string a5 = "resources/mia-greenp5.png";

  args[0] = const_cast<char*>(a1.c_str());
  args[1] = const_cast<char*>(a2.c_str());
  args[2] = const_cast<char*>(a3.c_str());
  args[3] = const_cast<char*>(a4.c_str());
  args[4] = const_cast<char*>(a5.c_str());

  EXPECT_EQ(1, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia -green .5 problem.";

  std::ifstream ifile("resources/mia-greenp5.png");

  ASSERT_TRUE(static_cast<bool>(ifile)) << "mia-greenp5.png didn't save.";
  ifile.close();

  PixelBuffer truth("resources/truth/greenp5.png");
  PixelBuffer today("resources/mia-greenp5.png");

  EXPECT_TRUE(truth == today)
          << "truth/greenp5.png and mia-greenp5.png don't match.";

  args[4] = NULL;
  a5 = "resources/mia-green-loop.png";
  args[4] = const_cast<char*>(a5.c_str());

  for (float i = -1.7; i < 4.0; i+=4.0) {
    args[3] = NULL;
    a4 = std::to_string(i);
    args[3] = const_cast<char*>(a4.c_str());
    EXPECT_EQ(1, cmd_processor.ProcessCommandLine(arc, args))
             << "Mia -green should pass for input " << i;
  }

  args[3] = NULL;
  args[4] = NULL;
  a4 = std::to_string(0);
  a5 = "resources/mia-green0.png";
  args[3] = const_cast<char*>(a4.c_str());
  args[4] = const_cast<char*>(a5.c_str());
  EXPECT_EQ(1, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia -green 0 should pass.";

  std::ifstream ifile2("resources/mia-green0.png");

  ASSERT_TRUE(static_cast<bool>(ifile2)) << "mia-green0.png didn't save.";
  ifile2.close();

  PixelBuffer truth2("resources/truth/mia-green0-truth.png");
  PixelBuffer out("resources/mia-green0.png");

  EXPECT_TRUE(truth2 == out)
         << "truth2 and mia-green0.png don't match.";

  args[2] = NULL;
  a3 = "-gr";
  args[2] = const_cast<char*>(a3.c_str());
  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia -gr 0 should fail.";


  args[2] = NULL;
  args[3] = NULL;
  a3 = "-green";
  a4 = "d";
  args[2] = const_cast<char*>(a3.c_str());
  args[3] = const_cast<char*>(a4.c_str());
  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
         << "Mia -green d should fail.";

  arc = 4;
  char *args2[4];
  a3 = "-green";
  a4 = "resources/mia-green-fail.png";
  args2[0] = const_cast<char*>(a1.c_str());
  args2[1] = const_cast<char*>(a2.c_str());
  args2[2] = const_cast<char*>(a3.c_str());
  args2[3] = const_cast<char*>(a4.c_str());

  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args2))
           << "Mia -green *.png should fail.";
}

TEST_F(MiaCommandLineTest, ChannelsFilterCommandBlue) {
  int arc = 5;
  char* args[5];

  std::string a1 = "./mia";
  std::string a2 = "resources/truth/flower.png";
  std::string a3 = "-blue";
  std::string a4 = ".5";
  std::string a5 = "resources/mia-bluep5.png";

  args[0] = const_cast<char*>(a1.c_str());
  args[1] = const_cast<char*>(a2.c_str());
  args[2] = const_cast<char*>(a3.c_str());
  args[3] = const_cast<char*>(a4.c_str());
  args[4] = const_cast<char*>(a5.c_str());

  EXPECT_EQ(1, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia -blue .5 problem.";

  std::ifstream ifile("resources/mia-bluep5.png");

  ASSERT_TRUE(static_cast<bool>(ifile)) << "mia-bluep5.png didn't save.";
  ifile.close();

  PixelBuffer truth("resources/truth/bluep5.png");
  PixelBuffer today("resources/mia-bluep5.png");

  EXPECT_TRUE(truth == today)
          << "truth/bluep5.png and mia-bluep5.png don't match.";

  args[4] = NULL;
  a5 = "resources/mia-blue-loop.png";
  args[4] = const_cast<char*>(a5.c_str());

  for (float i = -1.2; i < 4.0; i+=4.0) {
    args[3] = NULL;
    a4 = std::to_string(i);
    args[3] = const_cast<char*>(a4.c_str());
    EXPECT_EQ(1, cmd_processor.ProcessCommandLine(arc, args))
             << "Mia -blue should pass for input " << i;
  }

  args[3] = NULL;
  args[4] = NULL;
  a4 = std::to_string(0);
  a5 = "resources/mia-blue0.png";
  args[3] = const_cast<char*>(a4.c_str());
  args[4] = const_cast<char*>(a5.c_str());
  EXPECT_EQ(1, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia -blue 0 should pass.";

  std::ifstream ifile2("resources/mia-blue0.png");

  ASSERT_TRUE(static_cast<bool>(ifile2)) << "mia-blue0.png didn't save.";
  ifile2.close();

  PixelBuffer truth2("resources/truth/mia-blue0-truth.png");
  PixelBuffer out("resources/mia-blue0.png");

  EXPECT_TRUE(truth2 == out)
         << "truth2 and mia-blue0.png don't match.";

  args[2] = NULL;
  a3 = "-bleu";
  args[2] = const_cast<char*>(a3.c_str());
  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia -bleu 0 should fail.";

  args[2] = NULL;
  args[3] = NULL;
  a3 = "-blue";
  a4 = "e";
  args[2] = const_cast<char*>(a3.c_str());
  args[3] = const_cast<char*>(a4.c_str());
  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
         << "Mia -blue e should fail.";

  arc = 4;
  char *args2[4];
  a3 = "-blue";
  a4 = "resources/mia-blue-fail.png";
  args2[0] = const_cast<char*>(a1.c_str());
  args2[1] = const_cast<char*>(a2.c_str());
  args2[2] = const_cast<char*>(a3.c_str());
  args2[3] = const_cast<char*>(a4.c_str());

  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args2))
           << "Mia -blue *.png should fail.";
}



TEST_F(MiaCommandLineTest, QuantizeFilterCommand) {
  int arc = 5;
  char* args[5];

  std::string a1 = "./mia";
  std::string a2 = "resources/truth/flower.png";
  std::string a3 = "-quantize";
  std::string a4 = "2";
  std::string a5 = "resources/mia-quan2.png";

  args[0] = const_cast<char*>(a1.c_str());
  args[1] = const_cast<char*>(a2.c_str());
  args[2] = const_cast<char*>(a3.c_str());
  args[3] = const_cast<char*>(a4.c_str());
  args[4] = const_cast<char*>(a5.c_str());

  EXPECT_EQ(1, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia -quantize 5 problem.";

  std::ifstream ifile("resources/mia-quan2.png");

  ASSERT_TRUE(static_cast<bool>(ifile)) << "mia-quan2.png didn't save.";
  ifile.close();

  PixelBuffer truth("resources/truth/quan2.png");
  PixelBuffer today("resources/mia-quan2.png");

  EXPECT_TRUE(truth == today)
          << "truth/quan2.png and mia-quan2.png don't match.";

  for (int i = -1; i < 2; i++) {
    args[3] = NULL;
    a4 = std::to_string(i);
    args[3] = const_cast<char*>(a4.c_str());
    EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
             << "Mia -quantize should fail for input " << i;
  }

  args[2] = NULL;
  a3 = "quan";
  args[2] = const_cast<char*>(a3.c_str());
  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia quan 1 should fail.";

  args[2] = NULL;
  args[3] = NULL;
  a3 = "-quantize";
  a4 = "fkk";
  args[2] = const_cast<char*>(a3.c_str());
  args[3] = const_cast<char*>(a4.c_str());
  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
         << "Mia -quantize fkk should fail.";

  arc = 4;
  char *args2[4];
  a3 = "-quantize";
  a4 = "resources/mia-quan-fail.png";
  args2[0] = const_cast<char*>(a1.c_str());
  args2[1] = const_cast<char*>(a2.c_str());
  args2[2] = const_cast<char*>(a3.c_str());
  args2[3] = const_cast<char*>(a4.c_str());

  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args2))
           << "Mia -quantize *.png should fail.";
}

TEST_F(MiaCommandLineTest, SaturateFilterCommand) {
  int arc = 5;
  char* args[5];

  std::string a1 = "./mia";
  std::string a2 = "resources/truth/flower.png";
  std::string a3 = "-saturate";
  std::string a4 = "0";
  std::string a5 = "resources/mia-saturate0.png";

  args[0] = const_cast<char*>(a1.c_str());
  args[1] = const_cast<char*>(a2.c_str());
  args[2] = const_cast<char*>(a3.c_str());
  args[3] = const_cast<char*>(a4.c_str());
  args[4] = const_cast<char*>(a5.c_str());

  EXPECT_EQ(1, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia -saturate 0 problem.";

  std::ifstream ifile("resources/mia-saturate0.png");

  ASSERT_TRUE(static_cast<bool>(ifile)) << "mia-saturate0.png didn't save.";
  ifile.close();

  PixelBuffer truth("resources/truth/saturate0.png");
  PixelBuffer today("resources/mia-saturate0.png");

  EXPECT_TRUE(truth == today)
          << "truth/saturate0.png and mia-saturate0.png don't match.";

  args[4] = NULL;
  a5 = "resources/mia-saturate-loop.png";
  args[4] = const_cast<char*>(a5.c_str());

  for (float i = -1.34; i < 4.0; i+=4.07) {
    args[3] = NULL;
    a4 = std::to_string(i);
    args[3] = const_cast<char*>(a4.c_str());
    EXPECT_EQ(1, cmd_processor.ProcessCommandLine(arc, args))
             << "Mia -saturate should pass for input " << i;
  }

  std::ifstream ifile2("resources/mia-saturate-loop.png");

  ASSERT_TRUE(static_cast<bool>(ifile2))
            << "mia-saturate-loop.png didn't save.";
  ifile2.close();

  PixelBuffer truth2("resources/truth/mia-saturate-loop-truth.png");
  PixelBuffer out("resources/mia-saturate-loop.png");

  EXPECT_TRUE(truth2 == out)
         << "truth2 and mia-saturate-loop.png don't match.";

  args[2] = NULL;
  a3 = "-Satirate";
  args[2] = const_cast<char*>(a3.c_str());
  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia -Satirate should fail.";

  args[2] = NULL;
  args[3] = NULL;
  a3 = "-saturate";
  a4 = "ghgh";
  args[2] = const_cast<char*>(a3.c_str());
  args[3] = const_cast<char*>(a4.c_str());
  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
         << "Mia -saturate ghgh should fail.";

  arc = 4;
  char *args2[4];
  a3 = "-saturate";
  a4 = "resources/mia-saturate-fail.png";
  args2[0] = const_cast<char*>(a1.c_str());
  args2[1] = const_cast<char*>(a2.c_str());
  args2[2] = const_cast<char*>(a3.c_str());
  args2[3] = const_cast<char*>(a4.c_str());

  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args2))
           << "Mia -saturate *.png should fail.";
}

TEST_F(MiaCommandLineTest, ThresholdFilterCommand) {
  int arc = 5;
  char* args[5];

  std::string a1 = "./mia";
  std::string a2 = "resources/truth/flower.png";
  std::string a3 = "-threshold";
  std::string a4 = ".5";
  std::string a5 = "resources/mia-thresholdp5.png";

  args[0] = const_cast<char*>(a1.c_str());
  args[1] = const_cast<char*>(a2.c_str());
  args[2] = const_cast<char*>(a3.c_str());
  args[3] = const_cast<char*>(a4.c_str());
  args[4] = const_cast<char*>(a5.c_str());

  EXPECT_EQ(1, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia -threshold .5 problem.";

  std::ifstream ifile("resources/mia-thresholdp5.png");

  ASSERT_TRUE(static_cast<bool>(ifile)) << "mia-thresholdp5.png didn't save.";
  ifile.close();

  PixelBuffer truth("resources/truth/thresholdp5.png");
  PixelBuffer today("resources/mia-thresholdp5.png");

  EXPECT_TRUE(truth == today)
          << "truth/thresholdp5.png and mia-thresholdp5.png don't match.";

  for (float i = -3.3; i < 4.0; i+=6.0) {
    args[3] = NULL;
    a4 = std::to_string(i);
    args[3] = const_cast<char*>(a4.c_str());
    EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
             << "Mia -threshold should fail for input " << i;
  }

  args[3] = NULL;
  args[4] = NULL;
  a4 = std::to_string(0);
  a5 = "resources/mia-threshold0.png";
  args[3] = const_cast<char*>(a4.c_str());
  args[4] = const_cast<char*>(a5.c_str());
  EXPECT_EQ(1, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia -threshold 0 should pass.";

  std::ifstream ifile2("resources/mia-threshold0.png");

  ASSERT_TRUE(static_cast<bool>(ifile2)) << "mia-threshold0.png didn't save.";
  ifile2.close();

  PixelBuffer truth2("resources/truth/mia-threshold0-truth.png");
  PixelBuffer out("resources/mia-threshold0.png");

  EXPECT_TRUE(truth2 == out)
         << "truth2 and mia-threshold0.png don't match.";

  args[2] = NULL;
  a3 = "-threshol";
  args[2] = const_cast<char*>(a3.c_str());
  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia -threshol  should fail.";

  args[2] = NULL;
  args[3] = NULL;
  a3 = "-threshold";
  a4 = "Jhk";
  args[2] = const_cast<char*>(a3.c_str());
  args[3] = const_cast<char*>(a4.c_str());
  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
         << "Mia -threshold Jhk should fail.";

  arc = 4;
  char *args2[4];
  a3 = "-threshold";
  a4 = "resources/mia-threshold-fail.png";
  args2[0] = const_cast<char*>(a1.c_str());
  args2[1] = const_cast<char*>(a2.c_str());
  args2[2] = const_cast<char*>(a3.c_str());
  args2[3] = const_cast<char*>(a4.c_str());

  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args2))
           << "Mia -threshold *.png should fail.";
}


TEST_F(MiaCommandLineTest, MotionBlurFilterCommandNS) {
  int arc = 5;
  char* args[5];

  std::string a1 = "./mia";
  std::string a2 = "resources/truth/flower.png";
  std::string a3 = "-motionblur-n-s";
  std::string a4 = "5";
  std::string a5 = "resources/mia-mblurns5.png";

  args[0] = const_cast<char*>(a1.c_str());
  args[1] = const_cast<char*>(a2.c_str());
  args[2] = const_cast<char*>(a3.c_str());
  args[3] = const_cast<char*>(a4.c_str());
  args[4] = const_cast<char*>(a5.c_str());

  EXPECT_EQ(1, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia -motionblur-n-s 5 problem.";

  std::ifstream ifile("resources/mia-mblurns5.png");

  ASSERT_TRUE(static_cast<bool>(ifile)) << "mia-mblurns5.png didn't save.";
  ifile.close();

  PixelBuffer truth("resources/truth/mblurns5.png");
  PixelBuffer today("resources/mia-mblurns5.png");

  EXPECT_TRUE(truth == today)
          << "truth/mblurns5.png and mia-mblurns5.png don't match.";

  for (int i = -4; i < 0; i+=2) {
    args[3] = NULL;
    a4 = std::to_string(i);
    args[3] = const_cast<char*>(a4.c_str());
    EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
             << "Mia -motionblur-n-s should fail for input " << i;
  }

  args[3] = NULL;
  args[4] = NULL;
  a4 = std::to_string(0);
  a5 = "resources/mia-mblurns0.png";
  args[3] = const_cast<char*>(a4.c_str());
  args[4] = const_cast<char*>(a5.c_str());
  EXPECT_EQ(1, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia -motionblur-n-s 0 should pass.";

  std::ifstream ifile2("resources/mia-mblurns0.png");

  ASSERT_TRUE(static_cast<bool>(ifile2)) << "mia-mblurns0.png didn't save.";
  ifile2.close();

  PixelBuffer truth2("resources/truth/mia-mblurns0-truth.png");
  PixelBuffer out("resources/mia-mblurns0.png");

  EXPECT_TRUE(truth2 == out)
         << "truth2 and mia-mblurns0.png don't match.";

  args[2] = NULL;
  a3 = "mblurns";
  args[2] = const_cast<char*>(a3.c_str());
  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia mblurns 0 should fail.";

  args[2] = NULL;
  args[3] = NULL;
  a3 = "-motionblur-n-s";
  a4 = "i";
  args[2] = const_cast<char*>(a3.c_str());
  args[3] = const_cast<char*>(a4.c_str());
  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
        << "Mia -motionblur-n-s i should fail.";


  arc = 4;
  char *args2[4];
  a3 = "-mblurns";
  a4 = "resources/mia-mblurns-fail.png";
  args2[0] = const_cast<char*>(a1.c_str());
  args2[1] = const_cast<char*>(a2.c_str());
  args2[2] = const_cast<char*>(a3.c_str());
  args2[3] = const_cast<char*>(a4.c_str());

  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args2))
           << "Mia -motionblur-n-s *.png should fail.";
}


TEST_F(MiaCommandLineTest, MotionBlurFilterCommandEW) {
  int arc = 5;
  char* args[5];

  std::string a1 = "./mia";
  std::string a2 = "resources/truth/flower.png";
  std::string a3 = "-motionblur-e-w";
  std::string a4 = "5";
  std::string a5 = "resources/mia-mblurew5.png";

  args[0] = const_cast<char*>(a1.c_str());
  args[1] = const_cast<char*>(a2.c_str());
  args[2] = const_cast<char*>(a3.c_str());
  args[3] = const_cast<char*>(a4.c_str());
  args[4] = const_cast<char*>(a5.c_str());

  EXPECT_EQ(1, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia -motionblur-e-w 5 problem.";

  std::ifstream ifile("resources/mia-mblurew5.png");

  ASSERT_TRUE(static_cast<bool>(ifile)) << "mia-mblurew5.png didn't save.";
  ifile.close();

  PixelBuffer truth("resources/truth/mblurew5.png");
  PixelBuffer today("resources/mia-mblurew5.png");

  EXPECT_TRUE(truth == today)
          << "truth/mblurew5.png and mia-mblurew5.png don't match.";

  for (int i = -4; i < 0; i+=2) {
    args[3] = NULL;
    a4 = std::to_string(i);
    args[3] = const_cast<char*>(a4.c_str());
    EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
             << "Mia -motionblur-e-w should fail for input " << i;
  }

  args[3] = NULL;
  args[4] = NULL;
  a4 = std::to_string(0);
  a5 = "resources/mia-mblurew0.png";
  args[3] = const_cast<char*>(a4.c_str());
  args[4] = const_cast<char*>(a5.c_str());
  EXPECT_EQ(1, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia -motionblur-e-w 0 should pass.";

  std::ifstream ifile2("resources/mia-mblurew0.png");

  ASSERT_TRUE(static_cast<bool>(ifile2)) << "mia-mblurew0.png didn't save.";
  ifile2.close();

  PixelBuffer truth2("resources/truth/mia-mblurew0-truth.png");
  PixelBuffer out("resources/mia-mblurew0.png");

  EXPECT_TRUE(truth2 == out)
         << "truth2 and mia-mblurew0.png don't match.";

  args[2] = NULL;
  a3 = "mblurew";
  args[2] = const_cast<char*>(a3.c_str());
  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia mblurew 0 should fail.";

  args[2] = NULL;
  args[3] = NULL;
  a3 = "-motionblur-e-w";
  a4 = "L";
  args[2] = const_cast<char*>(a3.c_str());
  args[3] = const_cast<char*>(a4.c_str());
  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
         << "Mia -motionblur-e-w L should fail.";

  arc = 4;
  char *args2[4];
  a3 = "-mblurew";
  a4 = "resources/mia-mblurew-fail.png";
  args2[0] = const_cast<char*>(a1.c_str());
  args2[1] = const_cast<char*>(a2.c_str());
  args2[2] = const_cast<char*>(a3.c_str());
  args2[3] = const_cast<char*>(a4.c_str());

  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args2))
           << "Mia -motionblur-e-w *.png should fail.";
}


TEST_F(MiaCommandLineTest, MotionBlurFilterCommandNWSE) {
  int arc = 5;
  char* args[5];

  std::string a1 = "./mia";
  std::string a2 = "resources/truth/flower.png";
  std::string a3 = "-motionblur-nw-se";
  std::string a4 = "5";
  std::string a5 = "resources/mia-mblurnwse5.png";

  args[0] = const_cast<char*>(a1.c_str());
  args[1] = const_cast<char*>(a2.c_str());
  args[2] = const_cast<char*>(a3.c_str());
  args[3] = const_cast<char*>(a4.c_str());
  args[4] = const_cast<char*>(a5.c_str());

  EXPECT_EQ(1, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia -motionblur-nw-se 5 problem.";

  std::ifstream ifile("resources/mia-mblurnwse5.png");

  ASSERT_TRUE(static_cast<bool>(ifile)) << "mia-mblurnwse5.png didn't save.";
  ifile.close();

  PixelBuffer truth("resources/truth/mblurnwse5.png");
  PixelBuffer today("resources/mia-mblurnwse5.png");

  EXPECT_TRUE(truth == today)
          << "truth/mblurnwse5.png and mia-mblurnwse5.png don't match.";

  for (int i = -4; i < 0; i+=2) {
    args[3] = NULL;
    a4 = std::to_string(i);
    args[3] = const_cast<char*>(a4.c_str());
    EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
             << "Mia -motionblur-nw-se should fail for input " << i;
  }

  args[3] = NULL;
  args[4] = NULL;
  a4 = std::to_string(0);
  a5 = "resources/mia-mblurnwse0.png";
  args[3] = const_cast<char*>(a4.c_str());
  args[4] = const_cast<char*>(a5.c_str());
  EXPECT_EQ(1, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia -motionblur-nw-se 0 should pass.";

  std::ifstream ifile2("resources/mia-mblurnwse0.png");

  ASSERT_TRUE(static_cast<bool>(ifile2)) << "mia-mblurnwse0.png didn't save.";
  ifile2.close();

  PixelBuffer truth2("resources/truth/mia-mblurnwse0-truth.png");
  PixelBuffer out("resources/mia-mblurnwse0.png");

  EXPECT_TRUE(truth2 == out)
         << "truth2 and mia-mblurnwse0.png don't match.";

  args[2] = NULL;
  a3 = "mblurnwse";
  args[2] = const_cast<char*>(a3.c_str());
  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia mblurnwse 0 should fail.";

  args[2] = NULL;
  args[3] = NULL;
  a3 = "-motionblur-nw-se";
  a4 = "mn";
  args[2] = const_cast<char*>(a3.c_str());
  args[3] = const_cast<char*>(a4.c_str());
  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
         << "Mia -motionblur-nw-se mn should fail.";

  arc = 4;
  char *args2[4];
  a3 = "-mblurnwse";
  a4 = "resources/mia-mblurnwse-fail.png";
  args2[0] = const_cast<char*>(a1.c_str());
  args2[1] = const_cast<char*>(a2.c_str());
  args2[2] = const_cast<char*>(a3.c_str());
  args2[3] = const_cast<char*>(a4.c_str());

  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args2))
           << "Mia -motionblur-nw-se *.png should fail.";
}


TEST_F(MiaCommandLineTest, MotionBlurFilterCommandNESW) {
  int arc = 5;
  char* args[5];

  std::string a1 = "./mia";
  std::string a2 = "resources/truth/flower.png";
  std::string a3 = "-motionblur-ne-sw";
  std::string a4 = "5";
  std::string a5 = "resources/mia-mblurnesw5.png";

  args[0] = const_cast<char*>(a1.c_str());
  args[1] = const_cast<char*>(a2.c_str());
  args[2] = const_cast<char*>(a3.c_str());
  args[3] = const_cast<char*>(a4.c_str());
  args[4] = const_cast<char*>(a5.c_str());

  EXPECT_EQ(1, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia -motionblur-ne-sw 5 problem.";

  std::ifstream ifile("resources/mia-mblurnesw5.png");

  ASSERT_TRUE(static_cast<bool>(ifile)) << "mia-mblurnesw5.png didn't save.";
  ifile.close();

  PixelBuffer truth("resources/truth/mblurnesw5.png");
  PixelBuffer today("resources/mia-mblurnesw5.png");

  EXPECT_TRUE(truth == today)
          << "truth/mblurnesw5.png and mia-mblurnesw5.png don't match.";

  for (int i = -4; i < 0; i+=2) {
    args[3] = NULL;
    a4 = std::to_string(i);
    args[3] = const_cast<char*>(a4.c_str());
    EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
             << "Mia -motionblur-ne-sw should fail for input " << i;
  }

  args[3] = NULL;
  args[4] = NULL;
  a4 = std::to_string(0);
  a5 = "resources/mia-mblurnesw0.png";
  args[3] = const_cast<char*>(a4.c_str());
  args[4] = const_cast<char*>(a5.c_str());
  EXPECT_EQ(1, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia -motionblur-ne-sw 0 should pass.";

  std::ifstream ifile2("resources/mia-mblurnesw0.png");

  ASSERT_TRUE(static_cast<bool>(ifile2)) << "mia-mblurnesw0.png didn't save.";
  ifile2.close();

  PixelBuffer truth2("resources/truth/mia-mblurnesw0-truth.png");
  PixelBuffer out("resources/mia-mblurnesw0.png");

  EXPECT_TRUE(truth2 == out)
         << "truth2 and mia-mblurnesw0.png don't match.";


  args[2] = NULL;
  a3 = "mblurnesw";
  args[2] = const_cast<char*>(a3.c_str());
  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia mblurnesw 0 should fail.";

  args[2] = NULL;
  args[3] = NULL;
  a3 = "-motionblur-ne-sw";
  a4 = "x";
  args[2] = const_cast<char*>(a3.c_str());
  args[3] = const_cast<char*>(a4.c_str());
  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
         << "Mia -motionblur-ne-sw x should fail.";

  arc = 4;
  char *args2[4];
  a3 = "-mblurnesw";
  a4 = "resources/mia-mblurnesw-fail.png";
  args2[0] = const_cast<char*>(a1.c_str());
  args2[1] = const_cast<char*>(a2.c_str());
  args2[2] = const_cast<char*>(a3.c_str());
  args2[3] = const_cast<char*>(a4.c_str());

  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args2))
           << "Mia -motionblur-ne-sw *.png should fail.";
}

TEST_F(MiaCommandLineTest, CombinedOutput1) {
  int arc = 11;
  char* args[11];

  std::string a1 = "./mia";
  std::string a2 = "resources/truth/flower.png";
  std::string a3 = "-motionblur-n-s";
  std::string a4 = "3";
  std::string a5 = "-blue";
  std::string a6 = "2";
  std::string a7 = "-quantize";
  std::string a8 = "4.3";
  std::string a9 = "-help";
  std::string a10 = "4";
  std::string a11 = "resources/mia-combinedoutput1.png";

  args[0] = const_cast<char*>(a1.c_str());
  args[1] = const_cast<char*>(a2.c_str());
  args[2] = const_cast<char*>(a3.c_str());
  args[3] = const_cast<char*>(a4.c_str());
  args[4] = const_cast<char*>(a5.c_str());
  args[5] = const_cast<char*>(a6.c_str());
  args[6] = const_cast<char*>(a7.c_str());
  args[7] = const_cast<char*>(a8.c_str());
  args[8] = const_cast<char*>(a9.c_str());
  args[9] = const_cast<char*>(a10.c_str());
  args[10] = const_cast<char*>(a11.c_str());

  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia -help should end the program.";

  args[8] = NULL;
  a9 = "-sharpen";
  args[8] = const_cast<char*>(a9.c_str());

  EXPECT_EQ(1, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia combinedoutput1 should pass.";

  std::ifstream ifile("resources/mia-combinedoutput1.png");

  ASSERT_TRUE(static_cast<bool>(ifile))
            << "mia-combinedoutput1.png didn't save.";
  ifile.close();

  PixelBuffer truth("resources/truth/mia-combinedoutput1-truth.png");
  PixelBuffer today("resources/mia-combinedoutput1.png");

  EXPECT_TRUE(truth == today)
          << "truth and mia-combinedoutput1.png don't match.";

  args[7] = NULL;
  a8 = "hm";
  args[7] = const_cast<char*>(a8.c_str());
  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia -quantize hm should fail.";
}

TEST_F(MiaCommandLineTest, CombinedOutput2) {
  int arc = 11;
  char* args[11];

  std::string a1 = "./mia";
  std::string a2 = "resources/truth/flower.png";
  std::string a3 = "-motionblur-n-s";
  std::string a4 = "3.1";
  std::string a5 = "-blue";
  std::string a6 = "2";
  std::string a7 = "-sharpen";
  std::string a8 = "3.85";
  std::string a9 = "-q";
  std::string a10 = "4";
  std::string a11 = "resources/mia-combinedoutput2.png";

  args[0] = const_cast<char*>(a1.c_str());
  args[1] = const_cast<char*>(a2.c_str());
  args[2] = const_cast<char*>(a3.c_str());
  args[3] = const_cast<char*>(a4.c_str());
  args[4] = const_cast<char*>(a5.c_str());
  args[5] = const_cast<char*>(a6.c_str());
  args[6] = const_cast<char*>(a7.c_str());
  args[7] = const_cast<char*>(a8.c_str());
  args[8] = const_cast<char*>(a9.c_str());
  args[9] = const_cast<char*>(a10.c_str());
  args[10] = const_cast<char*>(a11.c_str());

  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia -q should fail.";

  args[8] = NULL;
  a9 = "-quantize";
  args[8] = const_cast<char*>(a9.c_str());

  EXPECT_EQ(1, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia combinedoutput2 should pass.";

  std::ifstream ifile("resources/mia-combinedoutput2.png");

  ASSERT_TRUE(static_cast<bool>(ifile))
            << "mia-combinedoutput2.png didn't save.";
  ifile.close();

  PixelBuffer truth("resources/truth/mia-combinedoutput2-truth.png");
  PixelBuffer today("resources/mia-combinedoutput2.png");
  PixelBuffer output1("resources/mia-combinedoutput1.png");


  EXPECT_TRUE(truth == today)
          << "truth and mia-combinedoutput2.png don't match.";


  EXPECT_FALSE(output1 == today)
          << "mia-combinedoutput1.png and mia-combinedoutput2.png match.";

  args[5] = NULL;
  a6 = "-Blue";
  args[5] = const_cast<char*>(a6.c_str());
  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia -Blue should fail.";
}

TEST_F(MiaCommandLineTest, CombinedOutput3) {
  int arc = 11;
  char* args[11];

  std::string a1 = "./mia";
  std::string a2 = "resources/truth/flower.png";
  std::string a3 = "-blur";
  std::string a4 = "4.05";
  std::string a5 = "-saturate";
  std::string a6 = ".7";
  std::string a7 = "threshold";
  std::string a8 = ".6";
  std::string a9 = "-blur";
  std::string a10 = "2.1";
  std::string a11 = "resources/mia-combinedoutput3.png";

  args[0] = const_cast<char*>(a1.c_str());
  args[1] = const_cast<char*>(a2.c_str());
  args[2] = const_cast<char*>(a3.c_str());
  args[3] = const_cast<char*>(a4.c_str());
  args[4] = const_cast<char*>(a5.c_str());
  args[5] = const_cast<char*>(a6.c_str());
  args[6] = const_cast<char*>(a7.c_str());
  args[7] = const_cast<char*>(a8.c_str());
  args[8] = const_cast<char*>(a9.c_str());
  args[9] = const_cast<char*>(a10.c_str());
  args[10] = const_cast<char*>(a11.c_str());

  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia threshold should fail.";

  args[6] = NULL;
  a7 = "-threshold";
  args[6] = const_cast<char*>(a7.c_str());

  EXPECT_EQ(1, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia combinedoutput3 should pass.";

  std::ifstream ifile("resources/mia-combinedoutput3.png");

  ASSERT_TRUE(static_cast<bool>(ifile))
            << "mia-combinedoutput3.png didn't save.";
  ifile.close();

  PixelBuffer truth("resources/truth/mia-combinedoutput3-truth.png");
  PixelBuffer today("resources/mia-combinedoutput3.png");

  EXPECT_TRUE(truth == today)
          << "truth and mia-combinedoutput3.png don't match.";

  args[1] = NULL;
  a2 = "flower.png";
  args[1] = const_cast<char*>(a2.c_str());
  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia file input should fail.";
}

TEST_F(MiaCommandLineTest, CombinedOutput4) {
  int arc = 24;
  char* args[24];

  std::string a1 = "./mia";
  std::string a2 = "resources/truth/flower.png";
  std::string a3 = "-motionblur-n-s";
  std::string a4 = "3";
  std::string a5 = "-blue";
  std::string a6 = "2";
  std::string a7 = "-quantize";
  std::string a8 = "4";
  std::string a9 = "-sharpen";
  std::string a10 = "2.95";
  std::string a11 = "-edgedetect";
  std::string a12 = "-motionblur-e-w";
  std::string a13 = "1.15";
  std::string a14 = "-saturate";
  std::string a15 = "3.5";
  std::string a16 = "-blur";
  std::string a17 = "1";
  std::string a18 = "-red";
  std::string a19 = "0";
  std::string a20 = "-motionblur-ne-sw";
  std::string a21 = "2";
  std::string a22 = "-motionblur-nw-se";
  std::string a23 = "1";
  std::string a24 = "resources/mia-combinedoutput4.png";

  args[0] = const_cast<char*>(a1.c_str());
  args[1] = const_cast<char*>(a2.c_str());
  args[2] = const_cast<char*>(a3.c_str());
  args[3] = const_cast<char*>(a4.c_str());
  args[4] = const_cast<char*>(a5.c_str());
  args[5] = const_cast<char*>(a6.c_str());
  args[6] = const_cast<char*>(a7.c_str());
  args[7] = const_cast<char*>(a8.c_str());
  args[8] = const_cast<char*>(a9.c_str());
  args[9] = const_cast<char*>(a10.c_str());
  args[10] = const_cast<char*>(a11.c_str());
  args[11] = const_cast<char*>(a12.c_str());
  args[12] = const_cast<char*>(a13.c_str());
  args[13] = const_cast<char*>(a14.c_str());
  args[14] = const_cast<char*>(a15.c_str());
  args[15] = const_cast<char*>(a16.c_str());
  args[16] = const_cast<char*>(a17.c_str());
  args[17] = const_cast<char*>(a18.c_str());
  args[18] = const_cast<char*>(a19.c_str());
  args[19] = const_cast<char*>(a20.c_str());
  args[20] = const_cast<char*>(a21.c_str());
  args[21] = const_cast<char*>(a22.c_str());
  args[22] = const_cast<char*>(a23.c_str());
  args[23] = const_cast<char*>(a24.c_str());

  EXPECT_EQ(1, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia combinedoutput4 should pass.";

  std::ifstream ifile("resources/mia-combinedoutput4.png");

  ASSERT_TRUE(static_cast<bool>(ifile))
            << "mia-combinedoutput4.png didn't save.";
  ifile.close();

  PixelBuffer truth("resources/truth/mia-combinedoutput4-truth.png");
  PixelBuffer today("resources/mia-combinedoutput4.png");

  EXPECT_TRUE(truth == today)
          << "truth and mia-combinedoutput4.png don't match.";

  args[20] = NULL;
  a21 = "-help";
  args[20] = const_cast<char*>(a21.c_str());
  EXPECT_EQ(0, cmd_processor.ProcessCommandLine(arc, args))
           << "Mia -help should end the program.";
}
}  //  namespace image_tools
