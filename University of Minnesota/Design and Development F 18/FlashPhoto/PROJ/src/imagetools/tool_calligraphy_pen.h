// Copyright 3081W 2018

/**
@file tool_calligraphy_pen.h
This file is part of the CSCI-3081W Project Support Code, which was developed
at the University of Minnesota.

This code is to be used for student coursework.  It is not an open source
project.
@copyright Copyright (c) 2015-2018 Daniel Keefe, TAs, & Regents of the
University of Minnesota.

Original Author(s) of this File:
  Seth Johnson, 2/15/2015, University of Minnesota

Author(s) of Significant Updates/Modifications to the File:
  ...
*/

#ifndef IMAGETOOLS_TOOL_CALLIGRAPHY_PEN_H_
#define IMAGETOOLS_TOOL_CALLIGRAPHY_PEN_H_

#include <cmath>
#include <string>
#include "imagetools/float_matrix.h"
#include "imagetools/tool.h"

namespace image_tools {

/** @brief This tool simulates a Calligraphy Pen, meaning it paints with a
 different thickness depending on which direction it is moved.  It has a oval
 mask tilted at an angle of 70 degrees counter-clockwise from the x-axis and an
 elongation ratio of 0.333. */
class ToolCalligraphyPen : public Tool {
 public:
  /** @brief Sets stamp overlap in superclass to 0.9. */
  ToolCalligraphyPen();
  virtual ~ToolCalligraphyPen();

  static const std::string name() { return "Calligraphy Pen"; }

  /** @brief Calls CreateOvalMask tilted at angle of 70 and ratio of 0.333. */
  FloatMatrix* CreateMask(float radius) override;
};

}  // namespace image_tools

#endif  // IMAGETOOLS_TOOL_CALLIGRAPHY_PEN_H_
