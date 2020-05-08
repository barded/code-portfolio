// Copyright 3081W 2018

/**
 @file mask_factory.h
 This file is part of the CSCI-3081W Project Support Code, which was developed
 at the University of Minnesota.

 This code is to be used for student coursework.  It is not an open source
 project.
 @copyright  Copyright (c) 2015-2018 Daniel Keefe, TAs, & Regents of the
 University of Minnesota.

 Original Author(s) of this File:
 Daniel Keefe, 2018, University of Minnesota

 Author(s) of Significant Updates/Modifications to the File:
 ...
 */

#ifndef IMAGETOOLS_MASK_FACTORY_H_
#define IMAGETOOLS_MASK_FACTORY_H_

#include "imagetools/float_matrix.h"

namespace image_tools {

/** @brief This factory is used to create masks used by image editing tools.
 Some of the masks are used by more than one tool. */
class MaskFactory {
 public:
  /** @brief Creates an opaque square mask with height 2*radius+1.
     Creates a circular brush shape. */
  static FloatMatrix* CreateConstantMask(float radius);

  /** @brief Creates an opaque mask with height 2*radius+1, tilted
    by angle_in_deg, and the desired width-height ratio.
    Creates a circular brush shape. */
  static FloatMatrix* CreateOvalMask(float radius, float angle_in_deg,
                                     float ratio);

  /** @brief Creates a square mask with height 2*radius+1, and opaqueness
   falls off linearly in all directions. Creates a circular brush shape. */
  static FloatMatrix* CreateLinearFalloffMask(float radius);

  /** @brief Creates an opaque mask with height 2*radius+1. The pattern is
   a bullsey used for the stamp tool. Creates a circular brush shape. */
  static FloatMatrix* CreateBullseyeMask(float radius, float linewidth);
};
}  // namespace image_tools

#endif  // IMAGETOOLS_MASK_FACTORY_H_
