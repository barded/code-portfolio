// Copyright 3081W 2018

/**
@file tool_stamp.h
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

#ifndef IMAGETOOLS_TOOL_STAMP_H_
#define IMAGETOOLS_TOOL_STAMP_H_

#include <string>
#include "imagetools/tool.h"

namespace image_tools {

/** @brief Stamps a single X marks the spot. */
class ToolStamp : public Tool {
 public:
  /** @brief Sets stamp overlap in superclass to 0.9. */
  ToolStamp();
  virtual ~ToolStamp();

  static const std::string name() { return "Stamp"; }

  /** @brief The stamp tool overrides AddToStroke to make it do nothing.  This
   way, each time the stamp tool is used it produces just a single mark on the
   canvas at the first location where it is applied. */
  void AddToStroke(int x, int y) override;

  /** @brief Calls CreateBullseyeMask with line width 0.2 * radius. */
  FloatMatrix* CreateMask(float radius) override;
};

}  // namespace image_tools

#endif  // IMAGETOOLS_TOOL_STAMP_H_
