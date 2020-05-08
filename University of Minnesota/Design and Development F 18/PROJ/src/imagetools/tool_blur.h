// Copyright 3081W 2018

/**
@file tool_blur.h
This file is part of the CSCI-3081W Project Support Code, which was developed
at the University of Minnesota.

This code is to be used for student coursework.  It is not an open source
project.
@copyright Copyright (c) 2015-2018 Daniel Keefe, TAs, & Regents of the
University of Minnesota.

Original Author(s) of this File:
  Seth Johnson, 4/4/2015, University of Minnesota

Author(s) of Significant Updates/Modifications to the File:
  ...
*/

#ifndef IMAGETOOLS_TOOL_BLUR_H_
#define IMAGETOOLS_TOOL_BLUR_H_

#include <stdio.h>
#include <string>
#include "imagetools/tool.h"
#include "imagetools/convolution_filter_blur.h"


namespace image_tools {

/** @brief This tool serves as a mobile version of the blur filter, functions
  much like the spray can, except with blurring neighboring pixels rather than
  coloring them (linear falloff). */
class ToolBlur : public Tool {
 public:
  /** @brief Sets stamp overlap in superclass to 0.333. Calls the SetupFilter
    function from ConvolutionFilterBlurblur. */
  ToolBlur();

  /** @brief Calls CleanupFilter function from ConvolutionFilterBlur. */
  virtual ~ToolBlur();

  static const std::string name() { return "Blur"; }

  /** @brief Calls CreateLinearFalloffMask. */
  FloatMatrix* CreateMask(float radius) override;

  /** @brief Calls CalculateFilteredPixel from ConvolutionFilterBlur. */
  ColorData LookupPaintColor(int x, int y) override;

 private:
  /* Copy assignment/construction is disallowed */
  ToolBlur(const ToolBlur& rhs) = delete;
  ToolBlur& operator=(const ToolBlur& rhs) = delete;

  ConvolutionFilterBlur blur_;
};

}  // namespace image_tools

#endif  // IMAGETOOLS_TOOL_BLUR_H_
