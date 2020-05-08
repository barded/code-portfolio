// Copyright 3081W 2018

/**
  @file convolution_filter_edge.h
  @copyright Copyright Deandra Bardell 2018 / 3081W Staff
*/

#ifndef IMAGETOOLS_CONVOLUTION_FILTER_EDGE_H_
#define IMAGETOOLS_CONVOLUTION_FILTER_EDGE_H_

#include "imagetools/convolution_filter.h"
#include "imagetools/color_data.h"
#include "imagetools/pixel_buffer.h"
#include "imagetools/float_matrix.h"

namespace image_tools {

/** @brief Implements an edge filter using a 3x3 edge kernel. */
class ConvolutionFilterEdge : public ConvolutionFilter {
 public:
  /** @brief No member variables to initialize. */
  ConvolutionFilterEdge();
  ~ConvolutionFilterEdge();

  /** @brief Generates a kernel that averages the neighboring pixels. */
  FloatMatrix* CreateKernel() override;
};

}  /* namespace image_tools */

#endif  // IMAGETOOLS_CONVOLUTION_FILTER_EDGE_H_
