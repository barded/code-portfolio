// Copyright Deandra Bardell 2018


#include "imagetools/filter_channels.h"

#include "imagetools/color_data.h"
#include "imagetools/pixel_buffer.h"

namespace image_tools {

FilterChannels::FilterChannels()
    : red_factor_(1.0), green_factor_(1.0), blue_factor_(1.0) {}

FilterChannels::FilterChannels(float red, float green, float blue)
    : red_factor_(red), green_factor_(green), blue_factor_(blue) {}

FilterChannels::~FilterChannels() {}


ColorData FilterChannels::CalculateFilteredPixel(const PixelBuffer *buffer,
                                                 int x, int y) {
  ColorData pixel = buffer->pixel(x, y);
  float red = pixel.red() * red_factor();
  float green = pixel.green() * green_factor();
  float blue = pixel.blue() * blue_factor();

  ColorData new_pixel(red, green, blue);
  new_pixel.Clamp();

  return new_pixel;
}

}  /* namespace image_tools */
