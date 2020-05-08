// Copyright 3081W 2018

/**
@file tool_pen.h
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

#ifndef IMAGETOOLS_TOOL_PEN_H_
#define IMAGETOOLS_TOOL_PEN_H_

#include <string>
#include "imagetools/tool.h"

namespace image_tools {

/** @brief The simplest of tools, this has a mask that is a completely opaque
 circle. */
class ToolPen : public Tool {
 public:
  /** @brief Sets stamp overlap in superclass to 0.9. */
  ToolPen();
  virtual ~ToolPen();

  static const std::string name() { return "Pen"; }

  /** @brief Calls CreateConstantMask. */
  FloatMatrix* CreateMask(float radius) override;
};
}  // namespace image_tools

#endif  // IMAGETOOLS_TOOL_PEN_H_
