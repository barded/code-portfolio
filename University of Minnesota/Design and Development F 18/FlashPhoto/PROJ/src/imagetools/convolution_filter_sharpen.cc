// Copyright Deandra Bardell 2018

#include "imagetools/convolution_filter_sharpen.h"

#include <cmath>

#include "imagetools/color_data.h"
#include "imagetools/pixel_buffer.h"
#include "imagetools/float_matrix.h"
#include "imagetools/image_tools_math.h"


namespace image_tools {

ConvolutionFilterSharpen::ConvolutionFilterSharpen() {
  size_ = static_cast<int>(round(2.0 * 5.0 + 1));
}

ConvolutionFilterSharpen::ConvolutionFilterSharpen(float radius) {
  size_ = static_cast<int>(round(2.0 * radius + 1));
}

ConvolutionFilterSharpen::~ConvolutionFilterSharpen() {}

void ConvolutionFilterSharpen::set_size(float radius) {
  size_ = static_cast<int>(round(2.0 * radius + 1));
}


FloatMatrix* ConvolutionFilterSharpen::CreateKernel() {
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
  kernel->Scale(-1.0);

  // add 2 to the middle element of the kernel
  kernel->set_value(middle, middle, kernel->value(middle, middle) + 2.0);

  return kernel;
}


}  /* namespace image_tools */
