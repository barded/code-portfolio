// Copyright Deandra Bardell 2018

#include "imagetools/convolution_filter_motion_blur.h"

#include <cmath>

#include "imagetools/color_data.h"
#include "imagetools/pixel_buffer.h"
#include "imagetools/float_matrix.h"
#include "imagetools/image_tools_math.h"


namespace image_tools {

ConvolutionFilterMotionBlur::ConvolutionFilterMotionBlur()
    : dir_(BLUR_DIR_N_S) {
  size_ = static_cast<int>(round(2.0 * 5.0 + 1));
}

ConvolutionFilterMotionBlur::ConvolutionFilterMotionBlur(float radius,
                                                        BlurDir dir)
    : dir_(dir) {
  size_ = static_cast<int>(round(2.0 * radius + 1));
}

ConvolutionFilterMotionBlur::~ConvolutionFilterMotionBlur() {}

void ConvolutionFilterMotionBlur::set_size(float radius) {
  size_ = static_cast<int>(round(2.0 * radius + 1));
}

FloatMatrix* ConvolutionFilterMotionBlur::CreateKernel() {
  FloatMatrix *kernel = new FloatMatrix(size(), size());

  for (int y = 0; y < kernel->height(); y++) {
    for (int x = 0; x < kernel->width(); x++) {
      int i = x - kernel->width() / 2;
      int j = y - kernel->height() / 2;

      float intensity = 0.0;
      switch (dir()) {
        case BLUR_DIR_N_S:
          intensity = (i == 0) ? 1 : 0;
          break;
        case BLUR_DIR_E_W:
          intensity = (j == 0) ? 1 : 0;
          break;
        case BLUR_DIR_NE_SW:
          intensity = (j == i) ? 1 : 0;
          break;
        case BLUR_DIR_NW_SE:
          intensity = (j == -i) ? 1 : 0;
          break;
        default:
          break;
      }
      kernel->set_value(x, y, intensity);
    }
  }

  kernel->Normalize();
  return kernel;
}


}  /* namespace image_tools */
