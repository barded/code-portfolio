// Copyright 3081W 2018

/**
  @file filter_channels.h
  @copyright Copyright Deandra Bardell 2018 / 3081W Staff
*/

#ifndef IMAGETOOLS_FILTER_CHANNELS_H_
#define IMAGETOOLS_FILTER_CHANNELS_H_

#include "imagetools/filter.h"
#include "imagetools/color_data.h"
#include "imagetools/pixel_buffer.h"

namespace image_tools {

/** @brief Filter one or more of the RGB channels, scaling each by the specified
 amount.  Scale factors of 1.0 have no affect.  Less than 1.0 "turns down" the
 intensity of the corresponding channel, and greater than 1.0 "turns up" the
 intensity.
 */
class FilterChannels : public Filter {
 public:
  /** @brief Sets red_factor_, green_factor_, blue_factor_ to 1.0. */
  FilterChannels();

  /** @brief Sets red_factor_, green_factor_, blue_factor_ to
    input arguments. */
  FilterChannels(float red, float green, float blue);
  ~FilterChannels();

  /** @brief Calculates the filtered version of the pixel at (x, y). */
  ColorData CalculateFilteredPixel(const PixelBuffer *buffer,
                                   int x, int y) override;

  inline float red_factor() const { return red_factor_; }
  inline float green_factor() const { return green_factor_; }
  inline float blue_factor() const { return blue_factor_; }

  inline void set_red_factor(float value) { red_factor_ = value; }
  inline void set_green_factor(float value) { green_factor_ = value; }
  inline void set_blue_factor(float value) { blue_factor_ = value; }



 private:
  float red_factor_;
  float green_factor_;
  float blue_factor_;
};

}  /* namespace image_tools */

#endif  // IMAGETOOLS_FILTER_CHANNELS_H_
