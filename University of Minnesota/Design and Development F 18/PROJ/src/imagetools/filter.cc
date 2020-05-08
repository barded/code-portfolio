// Copyright Deandra Bardell 2018


#include "imagetools/filter.h"

#include "imagetools/color_data.h"
#include "imagetools/pixel_buffer.h"

namespace image_tools {

Filter::Filter() {}

Filter::~Filter() {}

void Filter::ApplyToBuffer(PixelBuffer *buffer) {
  PixelBuffer *source = buffer;
  PixelBuffer *dest = buffer;

  if (!can_calculate_in_place()) {
    // create a temporary source buffer
    source = new PixelBuffer(*buffer);
  }

  SetupFilter();

  for (int i = 0; i < source->width(); i++) {
    for (int j = 0; j < source->height(); j++) {
      ColorData p = CalculateFilteredPixel(source, i, j);
      p.Clamp();
      dest->set_pixel(i, j, p);
    }
  }

  CleanupFilter();

  if (!can_calculate_in_place()) {
    delete source;
  }
}

void Filter::SetupFilter() {}

void Filter::CleanupFilter() {}

}  /* namespace image_tools */
