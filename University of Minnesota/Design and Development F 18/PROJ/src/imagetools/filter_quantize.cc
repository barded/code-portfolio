// Copyright Deandra Bardell 2018

#include "imagetools/filter_quantize.h"

#include <math.h>

#include "imagetools/color_data.h"
#include "imagetools/pixel_buffer.h"

namespace image_tools {

FilterQuantize::FilterQuantize(): num_bins_(10) {}

FilterQuantize::FilterQuantize(int num_bins): num_bins_(num_bins) {}

FilterQuantize::~FilterQuantize() {}

ColorData FilterQuantize::CalculateFilteredPixel(const PixelBuffer *buffer,
                                                 int x, int y) {
  ColorData pixel = buffer->pixel(x, y);
  int num_steps = num_bins() - 1;

  if (num_steps > 0) {  //  prevent division by 0 and negatives
    //  static_cast used to ensure float division
    float red = round(pixel.red() * num_steps) / static_cast<float>(num_steps);
    float green = round(pixel.green() * num_steps)
                      / static_cast<float>(num_steps);
    float blue = round(pixel.blue() * num_steps)
                      / static_cast<float>(num_steps);

    ColorData new_pixel(red, green, blue);
    new_pixel.Clamp();   // clamp here as a precaution
    return new_pixel;
  } else {   // num_steps <= 0
    return ColorData(0, 0, 0);   // return black pixel
  }
}

}  /* namespace image_tools */
