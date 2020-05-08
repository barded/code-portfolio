// Copyright Deandra Bardell 2018


#include "imagetools/filter_threshold.h"

#include "imagetools/image_tools_math.h"
#include "imagetools/color_data.h"
#include "imagetools/pixel_buffer.h"

namespace image_tools {

FilterThreshold::FilterThreshold(): cutoff_(0.5) {}

FilterThreshold::FilterThreshold(float cutoff): cutoff_(cutoff) {}

FilterThreshold::~FilterThreshold() {}

ColorData FilterThreshold::CalculateFilteredPixel(const PixelBuffer *buffer,
                                                  int x, int y) {
  ColorData pixel = buffer->pixel(x, y);
  float luminance_value = pixel.Luminance();

  ColorData black(0, 0, 0), white(1, 1, 1);

  if (luminance_value < cutoff()) {
    return black;
  } else {
    return white;
  }
}

} /* namespace image_tools */
