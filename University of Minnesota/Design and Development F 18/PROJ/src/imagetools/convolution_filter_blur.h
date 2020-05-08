// Copyright 3081W 2018

/**
  @file convolution_filter_blur.h
  @copyright Copyright Deandra Bardell 2018 / 3081W Staff
*/

#ifndef IMAGETOOLS_CONVOLUTION_FILTER_BLUR_H_
#define IMAGETOOLS_CONVOLUTION_FILTER_BLUR_H_

#include "imagetools/convolution_filter.h"
#include "imagetools/color_data.h"
#include "imagetools/pixel_buffer.h"
#include "imagetools/float_matrix.h"

namespace image_tools {

/** @brief Implements a blurring filter using a Gaussian kernel (i.e. blurs
  equally in all directions). */
class ConvolutionFilterBlur : public ConvolutionFilter {
 public:
  /** @brief Sets kernel radius to 5.0 */
  ConvolutionFilterBlur();

  /** @brief Radius should be > 1 in order to blur */
  explicit ConvolutionFilterBlur(float radius);

  ~ConvolutionFilterBlur();

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

#endif  // IMAGETOOLS_CONVOLUTION_FILTER_BLUR_H_
