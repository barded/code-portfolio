// Copyright 3081W 2018

/**
  @file filter_saturate.h
  @copyright Copyright Deandra Bardell 2018 / 3081W Staff
*/


#ifndef IMAGETOOLS_FILTER_SATURATE_H_
#define IMAGETOOLS_FILTER_SATURATE_H_

#include "imagetools/filter.h"
#include "imagetools/color_data.h"
#include "imagetools/pixel_buffer.h"

namespace image_tools {

/** @brief Adjusts the saturation or colorfulness of an image. A scale factor of
 1.0 will have no effect.  A scale factor of greater than 1.0 will "turn up" the
 saturation of the image, making it more colorful, and a factor less than 1.0
 will "turn down" the saturation.  A value of 0.0 will convert the image to
 grayscale. */
class FilterSaturate : public Filter {
 public:
  /** @brief Sets factor_ to 1.0. */
  FilterSaturate();

  /** @brief Sets factor_ to input argument. */
  explicit FilterSaturate(float factor);
  ~FilterSaturate();

  /** @brief Calculates the filtered version of the pixel at (x, y). */
  ColorData CalculateFilteredPixel(const PixelBuffer *buffer,
                                   int x, int y) override;

  inline float factor() const { return factor_; }

  inline void set_factor(float value) { factor_ = value; }

 private:
  float factor_;
};

}  /* namespace image_tools */

#endif  // IMAGETOOLS_FILTER_SATURATE_H_
