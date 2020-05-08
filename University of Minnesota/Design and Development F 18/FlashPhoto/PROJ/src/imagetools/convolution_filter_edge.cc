// Copyright Deandra Bardell 2018

#include "imagetools/convolution_filter_edge.h"

#include "imagetools/color_data.h"
#include "imagetools/pixel_buffer.h"
#include "imagetools/float_matrix.h"
#include "imagetools/image_tools_math.h"


namespace image_tools {

ConvolutionFilterEdge::ConvolutionFilterEdge() {}
ConvolutionFilterEdge::~ConvolutionFilterEdge() {}

FloatMatrix* ConvolutionFilterEdge::CreateKernel() {
  FloatMatrix *kernel = new FloatMatrix(3, 3);

  kernel->Scale(-1.0);
  kernel->set_value(1, 1, 8.0);  // middle of kernel

  return kernel;
}


}  /* namespace image_tools */
