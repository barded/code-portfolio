// Copyright 3081W 2018

/**
  @file convolution_filter.h
  @copyright Copyright Deandra Bardell 2018 / 3081W Staff
*/


#ifndef IMAGETOOLS_CONVOLUTION_FILTER_H_
#define IMAGETOOLS_CONVOLUTION_FILTER_H_

#include "imagetools/filter.h"
#include "imagetools/color_data.h"
#include "imagetools/pixel_buffer.h"
#include "imagetools/float_matrix.h"

namespace image_tools {

/** @brief The base class for filters that use convolution.  These filters
 convolve a kernel with the image by first making a temp copy of the image
 buffer.
 Then, they loop through each pixel of this copy, centering the kernel on that
 pixel and computing a new color by looking at the original colors of not just
 the current pixel but also its neighbors.  The output is written back to the
 original pixel buffer. */
class ConvolutionFilter : public Filter {
 public:
  /** @brief Sets kernel_ to NULL. */
  ConvolutionFilter();
  ~ConvolutionFilter();

  /** @brief For ConvolutionFilters, this method is used to call the
   CreateKernel() factory method before the process of looping through each
   pixel of the image is started. */
  void SetupFilter() override;

  /** @brief For ConvolutionFilters, this method is used to delete the kernel
    created by SetupFilter(). */
  void CleanupFilter() override;

  /** @brief Factory method: This method defers the job of creating the proper
   convolution kernel to subclasses.  The convolution algorithm is the same
   for all convolution filters, but the specific kernel used changes. */
  virtual FloatMatrix* CreateKernel() = 0;

  /** @brief ConvolutionFilters cannot be calculated "in place".  They require
   creating a temporary copy of the input buffer so that they can look up the
   colors of the original pixel values even after some have been modified. */
  bool can_calculate_in_place() const override { return false; }

  /** @brief For ConvolutionFilters, this function performs the convolution with
   the kernel centered at (x,y) and returns the resulting color.
   @param[in] buffer The buffer to filter
   @param[in] x X position of the pixel to filter
   @param[in] y Y position of the pixel to filter
   @return The filtered version of the color at pixel (x,y)
   */
  ColorData CalculateFilteredPixel(const PixelBuffer *buffer,
                                   int x, int y) override;

  inline FloatMatrix *kernel() { return kernel_; }
  inline void set_kernel(FloatMatrix *matrix) { kernel_ = matrix; }

 private:
  /* Copy assignment/construction is disallowed */
  ConvolutionFilter(const ConvolutionFilter &rhs) = delete;
  ConvolutionFilter &operator=(const ConvolutionFilter &rhs) = delete;

  FloatMatrix *kernel_;
};

}  /* namespace image_tools */

#endif  // IMAGETOOLS_CONVOLUTION_FILTER_H_
