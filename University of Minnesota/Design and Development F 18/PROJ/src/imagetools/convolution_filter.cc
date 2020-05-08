// Copyright Deandra Bardell 2018

#include "imagetools/convolution_filter.h"

#include <cmath>

#include "imagetools/color_data.h"
#include "imagetools/pixel_buffer.h"
#include "imagetools/float_matrix.h"
#include "imagetools/image_tools_math.h"


namespace image_tools {

ConvolutionFilter::ConvolutionFilter() : kernel_(NULL) {}
ConvolutionFilter::~ConvolutionFilter() {
  if (kernel_ != NULL) {
    delete kernel_;
  }
}

void ConvolutionFilter::SetupFilter() {
  set_kernel(CreateKernel());
}

void ConvolutionFilter::CleanupFilter() {
  delete kernel_;
  kernel_ = NULL;
}


ColorData ConvolutionFilter::CalculateFilteredPixel(const PixelBuffer *buffer,
                                                    int x, int y) {
  FloatMatrix *curr_kernel = kernel();
  int k_width = curr_kernel->width();
  int k_height = curr_kernel->height();
  int b_width = buffer->width();
  int b_height = buffer-> height();
  int radius = static_cast<int>(std::floor((curr_kernel->height() - 1) / 2.0));

  ColorData pixel(0, 0, 0);

  // loop over the current kernel
  for (int j = 0; j < k_height; j++) {
    // the y value of the pixel in the buffer
    int pixel_y = ImageToolsMath::Clamp(y - radius + j, 0, b_height - 1);

    for (int i = 0; i < k_width; i++) {
      float weight = curr_kernel->value(i, j);

      // the x value of the pixel in the buffer
      int pixel_x = ImageToolsMath::Clamp(x - radius + i, 0, b_width - 1);

      ColorData current_pixel = buffer->pixel(pixel_x, pixel_y);

      pixel = pixel + current_pixel * weight;
    }
  }

  pixel.Clamp();
  return pixel;
}

}  /* namespace image_tools */
