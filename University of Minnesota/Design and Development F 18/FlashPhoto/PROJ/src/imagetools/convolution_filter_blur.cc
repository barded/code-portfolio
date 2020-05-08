// Copyright Deandra Bardell 2018

#include "imagetools/convolution_filter_blur.h"

#include <cmath>

#include "imagetools/color_data.h"
#include "imagetools/pixel_buffer.h"
#include "imagetools/float_matrix.h"
#include "imagetools/image_tools_math.h"


namespace image_tools {

ConvolutionFilterBlur::ConvolutionFilterBlur() {
  size_ = static_cast<int>(round(2.0 * 5 + 1));
}

ConvolutionFilterBlur::ConvolutionFilterBlur(float radius) {
  size_ = static_cast<int>(round(2.0 * radius + 1));
}

void ConvolutionFilterBlur::set_size(float radius) {
  size_ = static_cast<int>(round(2.0 * radius + 1));
}

ConvolutionFilterBlur::~ConvolutionFilterBlur() {}


FloatMatrix* ConvolutionFilterBlur::CreateKernel() {
  FloatMatrix *kernel = new FloatMatrix(size(), size());
  // middle index of kernel
  int middle = static_cast<int>(std::floor(size() / 2.0));
  int sigma = static_cast<int>(round((size()-1) / 2.0));  // for Gaussian

  for (int y = 0; y < kernel->height(); y++) {
    for (int x = 0; x < kernel->width(); x++) {
      //  distance from center of kernel
      float distance = sqrt((x-middle) * (x-middle) + (y-middle) * (y-middle));
      kernel->set_value(x, y, ImageToolsMath::Gaussian(distance, sigma));
    }
  }

  kernel->Normalize();

  return kernel;
}


}  /* namespace image_tools */
