/**
This file is part of the CSCI-3081W Project Support Code, which was developed
at the University of Minnesota.

This code is to be used for student coursework.  It is not an open source
project.
Copyright (c) 2015-2018 Daniel Keefe, TAs, & Regents of the University of
Minnesota.

Original Author(s) of this File:
  Seth Johnson, 2/15/15, University of Minnesota

Author(s) of Significant Updates/Modifications to the File:
  ...
*/

#include "imagetools/tool_chalk.h"
#include <cmath>
#include <cstdlib>
#include "imagetools/mask_factory.h"

namespace image_tools {

#ifndef WIN32
unsigned int ToolChalk::seed_ = time(NULL);
#endif

ToolChalk::ToolChalk() {}

ToolChalk::~ToolChalk() {}

FloatMatrix *ToolChalk::CreateMask(float radius) {
  return MaskFactory::CreateConstantMask(radius);
}

ColorData ToolChalk::CombinePaintAndCanvasColor(const ColorData &paint_color,
                                                const ColorData &canvas_color,
                                                float mask_intensity) {
  // random number between 0.0 and 1.0
#ifdef WIN32
  float r = static_cast<float>(rand_r()) / static_cast<float>(RAND_MAX);
#else
  float r = static_cast<float>(rand_r(&seed_)) / static_cast<float>(RAND_MAX);
#endif
  if (r >= 0.4) {  // 60% of the time, make the mask_intensity = 0
    mask_intensity = 0.0;
  }
  return Tool::CombinePaintAndCanvasColor(paint_color, canvas_color,
                                          mask_intensity);
}

}  // namespace image_tools
