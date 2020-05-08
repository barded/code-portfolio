// Copyright 3081W 2018

/**
  @file convolution_filter_sharpen.h
  @copyright Copyright Deandra Bardell 2018 / 3081W Staff
*/

#ifndef IMAGETOOLS_CONVOLUTION_FILTER_SHARPEN_H_
#define IMAGETOOLS_CONVOLUTION_FILTER_SHARPEN_H_

#include "imagetools/convolution_filter.h"
#include "imagetools/color_data.h"
#include "imagetools/pixel_buffer.h"
#include "imagetools/float_matrix.h"

namespace image_tools {

/** @brief Implements a sharpening filter using a corresponding sharpening
    kernel. */
class ConvolutionFilterSharpen : public ConvolutionFilter {
 public:
  /** @brief Sets kernel radius to 5.0. */
  ConvolutionFilterSharpen();

  /** @brief Sets kernel radius to input argument. */
  explicit ConvolutionFilterSharpen(float radius);
  ~ConvolutionFilterSharpen();

  /** @brief Generates a kernel that averages the neighboring pixels. */
  FloatMatrix* CreateKernel() override;

  /** @brief The size of the kernel (i.e. width/height) is stored and
      is equal to rounded value 2 * radius + 1. */
  inline int size() { return size_; }

  void set_size(float radius);

 private:
  int size_;
};

}  /* namespace image_tools */

#endif  // IMAGETOOLS_CONVOLUTION_FILTER_SHARPEN_H_
