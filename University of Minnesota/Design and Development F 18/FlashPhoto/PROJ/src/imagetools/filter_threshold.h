// Copyright 3081W 2018

/**
  @file filter_threshold.h
  @copyright Copyright Deandra Bardell 2018 / 3081W Staff
*/

#ifndef IMAGETOOLS_FILTER_THRESHOLD_H_
#define IMAGETOOLS_FILTER_THRESHOLD_H_

#include "imagetools/filter.h"
#include "imagetools/color_data.h"
#include "imagetools/pixel_buffer.h"

namespace image_tools {

/** @brief A threshold filter that converts each pixel of the image to pure
 black
 or white.  First, each pixel is converted to a grayscale value, then the result
 is compared to the cutoff value.  If greater than the cutoff, then the pixel
 is set to white, and if less it is set to black. */
class FilterThreshold : public Filter {
 public:
  /** @brief Sets cutoff_ to 0.5. */
  FilterThreshold();

  /** @brief Sets cutoff_ to input argument. */
  explicit FilterThreshold(float cutoff);
  ~FilterThreshold();

  /** @brief Calculates the filtered version of the pixel at (x, y). */
  ColorData CalculateFilteredPixel(const PixelBuffer *buffer,
                                   int x, int y) override;

  inline float cutoff() const { return cutoff_; }

  inline void set_cutoff(float value) { cutoff_ = value; }

 private:
  float cutoff_;
};

}  /* namespace image_tools */

#endif  // IMAGETOOLS_FILTER_THRESHOLD_H_
