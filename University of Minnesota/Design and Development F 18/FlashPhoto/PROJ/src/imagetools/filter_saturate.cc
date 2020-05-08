// Copyright Deandra Bardell 2018


#include "imagetools/filter_saturate.h"

#include "imagetools/image_tools_math.h"
#include "imagetools/color_data.h"
#include "imagetools/pixel_buffer.h"

namespace image_tools {

FilterSaturate::FilterSaturate(): factor_(1.0) {}

FilterSaturate::FilterSaturate(float factor): factor_(factor) {}

FilterSaturate::~FilterSaturate() {}

ColorData FilterSaturate::CalculateFilteredPixel(const PixelBuffer *buffer,
                                                 int x, int y) {
  ColorData pixel = buffer->pixel(x, y);
  float luminance_value = pixel.Luminance();
  ColorData grayscale(luminance_value, luminance_value, luminance_value);

  ColorData lerp_value = ImageToolsMath::Lerp(grayscale, pixel, factor());

  lerp_value.Clamp();

  return lerp_value;
}

}  /* namespace image_tools */
