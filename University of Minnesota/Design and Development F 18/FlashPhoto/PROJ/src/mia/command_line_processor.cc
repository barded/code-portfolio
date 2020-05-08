// Copyright Deandra Bardell 2018

#include <fstream>
#include <stdexcept>
#include <cstring>
#include <cstdlib>
#include <iomanip>
#include <string>
#include <vector>

#include "mia/command_line_processor.h"
#include "imagetools/convolution_filter_motion_blur.h"
#include "imagetools/image_editor.h"
#include "mia/image_editor_commands.h"

namespace image_tools {

CommandLineProcessor::CommandLineProcessor() {}

CommandLineProcessor::~CommandLineProcessor() {
  int size = commands_.size();
  for (int i = 0; i < size; i++) {
    delete (commands_[i]);
  }
  commands_.clear();
}

bool CommandLineProcessor::FileExists(const char* fname) {
  std::ifstream ifile(fname);
  return static_cast<bool>(ifile);
}


int CommandLineProcessor::ProcessCommandLine(const int &argc, char *argv[]) {
  // first argument is program name
  int arg_count = argc;
  argv++, arg_count--;

  ImageEditor *image_editor = new ImageEditor();
  SaveCommand *save;


  // look for help
  for (int i = 0; i < arg_count; i++) {
    if (!strcmp(argv[i], "-help")) {
      PrintHelp();
      return 0;
    }
  }

  if (arg_count < 2) {
    std::cout << "Mia needs at least 2 inputs." << '\n';
    PrintHelp();
    return 0;
  }

  if (FileExists(argv[0])) {
    if (strstr(argv[0], ".png") != NULL || strstr(argv[0], ".PNG") != NULL) {
      std::string in_file = static_cast<std::string>(argv[0]);
      commands_.push_back(new LoadCommand(image_editor, in_file));
      argv++;
      arg_count--;
    } else {
      std::cout << argv[0] << " is not a .png." << '\n';
      return 0;
    }
  } else {
      std::cout << argv[0] << " not found." << '\n';
      return 0;
    }

  if (strstr(argv[arg_count-1], ".png") != NULL
      || strstr(argv[arg_count-1], ".PNG") != NULL) {
    std::string out = static_cast<std::string>(argv[arg_count-1]);
    save = new SaveCommand(image_editor, out);
    arg_count--;
  } else {
    std::cout << argv[arg_count-1] << " is not a .png." << '\n';
    return 0;
  }

  // parse inputs
  while (arg_count > 0) {
    if (!strcmp(argv[0], "-blur")) {
      if (arg_count < 2) {
        std::cout << "Too few arguments for -blur." << '\n';
        return 0;
      } else {
        float r;
        try {
          r = std::stof(argv[1]);
        }
        catch (const std::invalid_argument &ia) {
          std::cout << argv[1] << " isn't a valid -blur radius." << '\n';
          return 0;
        }

        if (r < 1) {
          std::cout << "-blur radius needs to be greater than 0." << '\n';
          return 0;
        }

        commands_.push_back(new BlurFilterCommand(image_editor, r));
        arg_count -= 2;
        argv += 2;
      }
    } else if (!strcmp(argv[0], "-edgedetect")) {
        commands_.push_back(new EdgeFilterCommand(image_editor));
        arg_count -= 1;
        argv += 1;
    } else if (!strcmp(argv[0], "-sharpen")) {
        if (arg_count < 2) {
          std::cout << "Too few arguments for -sharpen." << '\n';
          return 0;
        } else {
          float r;
          try {
            r = std::stof(argv[1]);
          }
          catch (const std::invalid_argument &ia) {
            std::cout << argv[1] << " isn't a valid -sharpen radius." << '\n';
            return 0;
          }

          if (r < 1) {
            std::cout << "-sharpen radius needs to be greater than 0." << '\n';
            return 0;
          }

          commands_.push_back(new SharpenFilterCommand(image_editor, r));
          arg_count -= 2;
          argv += 2;
        }
    } else if (!strcmp(argv[0], "-red")) {
        if (arg_count < 2) {
          std::cout << "Too few arguments for -red." << '\n';
          return 0;
        } else {
          float s;
          try {
            s = std::stof(argv[1]);
          }
          catch (const std::invalid_argument &ia) {
            std::cout << argv[1] << " isn't a valid -red scale factor." << '\n';
            return 0;
          }

          commands_.push_back(
                         new ChannelsFilterCommand(image_editor, s, 1.0, 1.0));
          arg_count -= 2;
          argv += 2;
        }
    } else if (!strcmp(argv[0], "-green")) {
        if (arg_count < 2) {
          std::cout << "Too few arguments for -green." << '\n';
          return 0;
        } else {
          float s;
          try {
            s = std::stof(argv[1]);
          }
          catch (const std::invalid_argument &ia) {
            std::cout << argv[1]
                      << " isn't a valid -green scale factor." << '\n';
            return 0;
          }

          commands_.push_back(
                         new ChannelsFilterCommand(image_editor, 1.0, s, 1.0));
          arg_count -= 2;
          argv += 2;
        }
    } else if (!strcmp(argv[0], "-blue")) {
        if (arg_count < 2) {
          std::cout << "Too few arguments for -blue." << '\n';
          return 0;
        } else {
          float s;
          try {
            s = std::stof(argv[1]);
          }
          catch (const std::invalid_argument &ia) {
            std::cout << argv[1]
                      << " isn't a valid -blue scale factor." << '\n';
            return 0;
          }

          commands_.push_back(
                         new ChannelsFilterCommand(image_editor, 1.0, 1.0, s));
          arg_count -= 2;
          argv += 2;
        }
    } else if (!strcmp(argv[0], "-quantize")) {
      if (arg_count < 2) {
        std::cout << "Too few arguments for -quantize." << '\n';
        return 0;
      } else {
        int n;
        try {
          n = std::stoi(argv[1]);
        }
        catch (const std::invalid_argument &ia) {
          std::cout << argv[1]
                    << " isn't a valid -quantize int number of bins." << '\n';
          return 0;
        }

        if (n < 2) {
          std::cout << "-quantize int number of bins"
                       " should be 2 or more." << '\n';
          return 0;
        }

        commands_.push_back(new QuantizeFilterCommand(image_editor, n));
        arg_count -= 2;
        argv += 2;
      }
    } else if (!strcmp(argv[0], "-saturate")) {
        if (arg_count < 2) {
          std::cout << "Too few arguments for -saturate." << '\n';
          return 0;
        } else {
          float s;
          try {
            s = std::stof(argv[1]);
          }
          catch (const std::invalid_argument &ia) {
            std::cout << argv[1]
                      << " isn't a valid -saturate scale factor." << '\n';
            return 0;
          }

          commands_.push_back(new SaturateFilterCommand(image_editor, s));
          arg_count -= 2;
          argv += 2;
        }
    } else if (!strcmp(argv[0], "-threshold")) {
        if (arg_count < 2) {
          std::cout << "Too few arguments for -threshold." << '\n';
          return 0;
        } else {
          float c;
          try {
            c = std::stof(argv[1]);
          }
          catch (const std::invalid_argument &ia) {
            std::cout << argv[1]
                      << " isn't a valid -threshold cutoff value." << '\n';
            return 0;
          }

          if (c < 0 || c > 1) {
            std::cout << "-threshold cutoff needs to be in range 0-1." << '\n';
            return 0;
          }
          commands_.push_back(new ThresholdFilterCommand(image_editor, c));
          arg_count -= 2;
          argv += 2;
        }
    } else if (!strcmp(argv[0], "-motionblur-n-s")) {
        if (arg_count < 2) {
          std::cout << "Too few arguments for -motionblur-n-s." << '\n';
          return 0;
        } else {
          float r;
          try {
            r = std::stof(argv[1]);
          }
          catch (const std::invalid_argument &ia) {
            std::cout << argv[1]
                      << " isn't a valid -motionblur-n-s radius." << '\n';
            return 0;
          }

          if (r < 0) {
            std::cout << "-motionblur-n-s radius needs to be positive." << '\n';
            return 0;
          }

          commands_.push_back(
            new MotionBlurFilterCommand(image_editor, r,
                                   ConvolutionFilterMotionBlur::BLUR_DIR_N_S));
          arg_count -= 2;
          argv += 2;
        }
    } else if (!strcmp(argv[0], "-motionblur-e-w")) {
        if (arg_count < 2) {
          std::cout << "Too few arguments for -motionblur-e-w." << '\n';
          return 0;
        } else {
          float r;
          try {
            r = std::stof(argv[1]);
          }
          catch (const std::invalid_argument &ia) {
            std::cout << argv[1]
                      << " isn't a valid -motionblur-e-w radius." << '\n';
            return 0;
          }

          if (r < 0) {
            std::cout << "-motionblur-e-w radius needs to be positive." << '\n';
            return 0;
          }

          commands_.push_back(
            new MotionBlurFilterCommand(image_editor, r,
                                   ConvolutionFilterMotionBlur::BLUR_DIR_E_W));
          arg_count -= 2;
          argv += 2;
        }
    } else if (!strcmp(argv[0], "-motionblur-ne-sw")) {
        if (arg_count < 2) {
          std::cout << "Too few arguments for -motionblur-ne-sw." << '\n';
          return 0;
        } else {
          float r;
          try {
            r = std::stof(argv[1]);
          }
          catch (const std::invalid_argument &ia) {
            std::cout << argv[1]
                      << " isn't a valid -motionblur-ne-sw radius." << '\n';
            return 0;
          }

          if (r < 0) {
            std::cout
                << "-motionblur-ne-sw radius needs to be positive." << '\n';
            return 0;
          }

          commands_.push_back(
            new MotionBlurFilterCommand(image_editor, r,
                                ConvolutionFilterMotionBlur::BLUR_DIR_NE_SW));
          arg_count -= 2;
          argv += 2;
        }
    } else if (!strcmp(argv[0], "-motionblur-nw-se")) {
        if (arg_count < 2) {
          std::cout << "Too few arguments for -motionblur-nw-se." << '\n';
          return 0;
        } else {
          float r;
          try {
            r = std::stof(argv[1]);
          }
          catch (const std::invalid_argument &ia) {
            std::cout << argv[1]
                      << " isn't a valid -motionblur-nw-se radius." << '\n';
            return 0;
          }

          if (r < 0) {
            std::cout
               << "-motionblur-nw-se radius needs to be positive." << '\n';
            return 0;
          }

          commands_.push_back(
            new MotionBlurFilterCommand(image_editor, r,
                                 ConvolutionFilterMotionBlur::BLUR_DIR_NW_SE));
          arg_count -= 2;
          argv += 2;
        }
    } else {
        std::cout << "Unknown command: " << argv[0] << '\n';
        PrintHelp();
        return 0;
      }
  }

    commands_.push_back(save);
    int size = commands_.size();

    for (int i = 0; i < size; i++) {
      ImageEditorCommand *cmd = commands_[i];
      (*cmd).Execute();
    }

  delete image_editor;

  return 1;
}


void CommandLineProcessor::PrintHelp() {
  int w = 22;

  std::cout <<
     "usage: mia infile.png [image processing commands ...] outfile.png\n\n";
  std::cout << std::left << std::setw(w) <<
     "infile.png:" << "input image file in png format\n";
  std::cout << std::left << std::setw(w) <<
     "outfile.png:" << "filename to use for saving the result\n\n";
  std::cout << "image processing comamnds:\n";
  std::cout << std::left << std::setw(w) <<
     "-blur r:" << "apply a gaussian blur of radius r\n";
  std::cout << std::left << std::setw(w) <<
     "-edgedetect:" << "apply an edge detection filter\n";
  std::cout << std::left << std::setw(w) <<
     "-sharpen r:" << "apply a sharpening filter of radius r\n";
  std::cout << std::left << std::setw(w) <<
     "-red s:" << "scale the red channel by factor s\n";
  std::cout << std::left << std::setw(w) <<
     "-green s:" << "scale the green channel by factor s\n";
  std::cout << std::left << std::setw(w) <<
     "-blue s:" << "scale the blue channel by factor s\n";
  std::cout << std::left << std::setw(w) <<
     "-quantize n:" << "quantize each color channel into n bins\n";
  std::cout << std::left << std::setw(w) <<
     "-saturate s:" << "saturate colors by scale factor s\n";
  std::cout << std::left << std::setw(w) <<
     "-threshold c:" << "threshold using cutoff value c\n";
  std::cout << std::left << std::setw(w) <<
     "-motionblur-n-s r:" << "north-south motion blur with kernel radius r\n";
  std::cout << std::left << std::setw(w) <<
     "-motionblur-e-w r:" << "east-west motion blur with kernel radius r\n";
  std::cout << std::left << std::setw(w) <<
     "-motionblur-ne-sw r:" << "ne-sw motion blur with kernel radius r\n";
  std::cout << std::left << std::setw(w) <<
     "-motionblur-nw-se r:" << "nw-se motion blur with kernel radius r\n";
}
}  // namespace image_tools
