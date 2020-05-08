// Copyright 3081W 2018

/**
  @file convolution_filter_motion_blur.h
  @copyright Copyright Deandra Bardell 2018 / 3081W Staff
*/


#ifndef IMAGETOOLS_CONVOLUTION_FILTER_MOTION_BLUR_H_
#define IMAGETOOLS_CONVOLUTION_FILTER_MOTION_BLUR_H_

#include "imagetools/convolution_filter.h"

#include "imagetools/color_data.h"
#include "imagetools/pixel_buffer.h"
#include "imagetools/float_matrix.h"

namespace image_tools {


/** @brief Implements a motion blurring filter.  This is different that a
 blurring filter, which blurs in all directions. A motion blur filter only blurs
 in one direction. */
class ConvolutionFilterMotionBlur : public ConvolutionFilter {
 public:
  /** @brief Four possible directions of blur are supported. */
  enum BlurDir { BLUR_DIR_N_S, BLUR_DIR_E_W, BLUR_DIR_NE_SW, BLUR_DIR_NW_SE };

  /** @brief Sets kernel radius to 5.0 and direction to BLUR_DIR_N_S. */
  ConvolutionFilterMotionBlur();

  /** @brief Sets kernel radius and direction input arguments. */
  ConvolutionFilterMotionBlur(float radius, BlurDir dir);
  ~ConvolutionFilterMotionBlur();

  /** @brief Generates a kernel that averages the neighboring pixels. */
  FloatMatrix* CreateKernel() override;

  /** @brief The size of the kernel (i.e. width/height) is stored and
      is equal to rounded value 2 * radius + 1. */
  inline int size() { return size_; }
  inline BlurDir dir() { return dir_; }

  void set_size(float radius);
  inline void set_dir(BlurDir dir) { dir_ = dir; }


 private:
  int size_;
  BlurDir dir_;
};

}  /* namespace image_tools */

#endif  // IMAGETOOLS_CONVOLUTION_FILTER_MOTION_BLUR_H_
