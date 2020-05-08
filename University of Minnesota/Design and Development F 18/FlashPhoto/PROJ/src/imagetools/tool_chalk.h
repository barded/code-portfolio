// Copyright 3081W 2018

/**
@file tool_chalk.h
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

#ifndef IMAGETOOLS_TOOL_CHALK_H_
#define IMAGETOOLS_TOOL_CHALK_H_

#include <string>
#include "imagetools/tool.h"

namespace image_tools {

/** @brief This tool simulates chalk drawing on a rough paper.  The class
 overrides Tool's default color blending function to insert random noise as
 the tool is moved around. */
class ToolChalk : public Tool {
 public:
  /** @brief Constructor does nothing. */
  ToolChalk();

  virtual ~ToolChalk();

  static const std::string name() { return "Chalk"; }

  /** @brief Calls CreateConstantMask. */
  FloatMatrix *CreateMask(float radius) override;

  /** @brief For the chalk tool, the mask is a constant 1.0 inside the radius
   of the tool and 0.0 outside.  We only use the mask to get this circular
   shape. To get the appearance of chalk, we insert random holes in the mask
   each time we apply it to the canvas.  This is done probabalistically so that
   about 40% of the mask is opaqaue and the other 60% is transparent but
   the transparent parts show up at random places within the mask. */
  ColorData CombinePaintAndCanvasColor(const ColorData &paint_color,
                                       const ColorData &canvas_color,
                                       float mask_intensity) override;

  /** @brief Used for unit testing to ensure a constant mask when testing
   this tool. Sets the seed to 1. */
  void testing() { seed_ = 1; }

 private:
  static unsigned int seed_;
};
}  // namespace image_tools

#endif  // IMAGETOOLS_TOOL_CHALK_H_
