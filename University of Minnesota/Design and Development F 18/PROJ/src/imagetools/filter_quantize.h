// Copyright 3081W 2018

/**
  @file filter_quantize.h
  @copyright Copyright Deandra Bardell 2018 / 3081W Staff
*/


#ifndef IMAGETOOLS_FILTER_QUANTIZE_H_
#define IMAGETOOLS_FILTER_QUANTIZE_H_

#include "imagetools/filter.h"
#include "imagetools/color_data.h"
#include "imagetools/pixel_buffer.h"

namespace image_tools {

/** @brief Implements a quantization filter that bins colors to produce
 something like a graphic novel visual effect.
 Each channel of each pixel is characterized as filling into one of n predefined
 bins, turning the continuous range of 0.0 to 1.0 for each channel into a set of
 n discrete values.*/
class FilterQuantize : public Filter {
 public:
  /** @brief Sets num_bins to 10. */
  FilterQuantize();

  /** @brief Sets num_bins to input argument. */
  explicit FilterQuantize(int num_bins);
  ~FilterQuantize();

  /** @brief Calculates the filtered version of the pixel at (x, y). */
  ColorData CalculateFilteredPixel(const PixelBuffer *buffer,
                                   int x, int y) override;

  inline int num_bins() const { return num_bins_; }

  inline void set_num_bins(int n) { num_bins_ = n; }


 private:
  int num_bins_;
};

}  /* namespace image_tools */

#endif  // IMAGETOOLS_FILTER_QUANTIZE_H_
