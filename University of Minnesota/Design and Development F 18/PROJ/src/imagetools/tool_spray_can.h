// Copyright 3081W 2018

/**
@file tool_spray_can.h
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

#ifndef IMAGETOOLS_TOOL_SPRAY_CAN_H_
#define IMAGETOOLS_TOOL_SPRAY_CAN_H_

#include <string>
#include "imagetools/tool.h"

namespace image_tools {

/** @brief This tool has a linear falloff so that it looks like the paint has
 been sprayed onto the canvas.  It also has the interesting property that it
 continues to paint even when held stationary so paint continues to accumulate
 on the canvas the longer you hold it still. */
class ToolSprayCan : public Tool {
 public:
  /** @brief Sets stamp overlap in superclass to 0.75. */
  ToolSprayCan();
  virtual ~ToolSprayCan();

  static const std::string name() { return "Spray Can"; }

  /** @brief Calls CreateLinearFalloffMask then scales all elements by 0.1. */
  FloatMatrix* CreateMask(float radius) override;

  /** @brief Overrides the default value with true. */
  bool applies_paint_when_stationary() override { return true; }
};
} /* namespace image_tools */

#endif  // IMAGETOOLS_TOOL_SPRAY_CAN_H_
