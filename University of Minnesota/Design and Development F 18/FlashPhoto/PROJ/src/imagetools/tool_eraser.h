// Copyright 3081W 2018

/**
@file tool_eraser.h
This file is part of the CSCI-3081W Project Support Code, which was developed
at the University of Minnesota.

This code is to be used for student coursework.  It is not an open source
project.
@copyright Copyright (c) 2015-2018 Daniel Keefe, TAs, & Regents of the
University of Minnesota.

Original Author(s) of this File:
  Seth Johnson, 2/15/15, University of Minnesota

Author(s) of Significant Updates/Modifications to the File:
  ...
*/

#ifndef IMAGETOOLS_TOOL_ERASER_H_
#define IMAGETOOLS_TOOL_ERASER_H_

#include <string>
#include "imagetools/color_data.h"
#include "imagetools/float_matrix.h"
#include "imagetools/tool.h"

namespace image_tools {

/** @brief This tool simulates an Eraser.  It overrides the Tool's StartStroke
 method to change the paint_color to be the canvas's background color. */
class ToolEraser : public Tool {
 public:
  /** @brief Sets stamp overlap in superclass to 0.9. */
  ToolEraser();
  virtual ~ToolEraser();

  static const std::string name() { return "Eraser"; }

  /** @brief Calls CreateConstantMask. */
  FloatMatrix *CreateMask(float radius) override;

  /** @brief The buffer's background color is used as the paint color for
   this tool. */
  void StartStroke(PixelBuffer *buffer, int x, int y,
                   const ColorData &paint_color, float radius) override;
};

}  // namespace image_tools

#endif  // IMAGETOOLS_TOOL_ERASER_H_
