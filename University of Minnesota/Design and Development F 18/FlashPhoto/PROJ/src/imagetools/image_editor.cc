/**
This file is part of the CSCI-3081W Project Support Code, which was developed
at the University of Minnesota.

This code is to be used for student coursework.  It is not an open source
project.
Copyright (c) 2015-2018 Daniel Keefe, TAs, & Regents of the University of
Minnesota.

Original Author(s) of this File:
  Daniel Keefe, 2018, University of Minnesota

Author(s) of Significant Updates/Modifications to the File:
  ...
*/

#include "imagetools/image_editor.h"

#include <deque>
#include <string>

#include "imagetools/pixel_buffer.h"

// filters
#include "imagetools/convolution_filter_blur.h"
#include "imagetools/convolution_filter_edge.h"
#include "imagetools/convolution_filter_motion_blur.h"
#include "imagetools/convolution_filter_sharpen.h"
#include "imagetools/filter_channels.h"
#include "imagetools/filter_quantize.h"
#include "imagetools/filter_saturate.h"
#include "imagetools/filter_threshold.h"

// tools
#include "imagetools/tool_blur.h"
#include "imagetools/tool_calligraphy_pen.h"
#include "imagetools/tool_chalk.h"
#include "imagetools/tool_eraser.h"
#include "imagetools/tool_highlighter.h"
#include "imagetools/tool_pen.h"
#include "imagetools/tool_spray_can.h"
#include "imagetools/tool_stamp.h"


namespace image_tools {

ImageEditor::ImageEditor() : current_buffer_(NULL), max_undos_(30) {}

ImageEditor::ImageEditor(PixelBuffer *buffer) {
  max_undos_ = 20;
  current_buffer_ = buffer;
}

ImageEditor::~ImageEditor() {
  if (current_buffer_ != NULL) {
    delete current_buffer_;
  }
}

void ImageEditor::LoadFromFile(const std::string &filename) {
  if (current_buffer_ != NULL) {
    SaveStateForPossibleUndo();
    current_buffer_->LoadFromFile(filename);
  } else {
    current_buffer_ = new PixelBuffer(filename);
  }
}

void ImageEditor::SaveToFile(const std::string &filename) {
  current_buffer_->SaveToFile(filename);
}

Tool *ImageEditor::GetToolByName(const std::string &name) {
  if (name == t_blur_.name()) {
    return &t_blur_;
  } else if (name == t_calligraphy_pen_.name()) {
    return &t_calligraphy_pen_;
  } else if (name == t_chalk_.name()) {
    return &t_chalk_;
  } else if (name == t_eraser_.name()) {
    return &t_eraser_;
  } else if (name == t_highlighter_.name()) {
    return &t_highlighter_;
  } else if (name == t_pen_.name()) {
    return &t_pen_;
  } else if (name == t_spray_can_.name()) {
    return &t_spray_can_;
  } else if (name == t_stamp_.name()) {
    return &t_stamp_;
  } else {
    return NULL;
  }
}

void ImageEditor::StartStroke(const std::string &tool_name,
                              const ColorData &color, float radius, int x,
                              int y) {
  current_tool_ = GetToolByName(tool_name);
  tool_color_ = color;
  tool_radius_ = radius;
  if ((current_tool_) && (current_buffer_)) {
    SaveStateForPossibleUndo();
    current_tool_->StartStroke(current_buffer_, x, y, tool_color_,
                               tool_radius_);
  }
}

void ImageEditor::AddToStroke(int x, int y) {
  if ((current_tool_) && (current_buffer_)) {
    current_tool_->AddToStroke(x, y);
  }
}

void ImageEditor::EndStroke(int x, int y) {
  if ((current_tool_) && (current_buffer_)) {
    current_tool_->EndStroke(x, y);
  }
}

void ImageEditor::ApplyBlurFilter(float radius) {
  if (current_buffer_) {
    SaveStateForPossibleUndo();
    f_blur_.set_size(radius);
    f_blur_.ApplyToBuffer(current_buffer_);
  }
}

void ImageEditor::ApplyMotionBlurFilter(float rad,
                                  ConvolutionFilterMotionBlur::BlurDir dir) {
  if (current_buffer_) {
    SaveStateForPossibleUndo();
    f_motion_blur_.set_size(rad);
    f_motion_blur_.set_dir(dir);
    f_motion_blur_.ApplyToBuffer(current_buffer_);
  }
}

void ImageEditor::ApplySharpenFilter(float rad) {
  if (current_buffer_) {
    SaveStateForPossibleUndo();
    f_sharpen_.set_size(rad);
    f_sharpen_.ApplyToBuffer(current_buffer_);
  }
}

void ImageEditor::ApplyEdgeDetectFilter() {
  if (current_buffer_) {
    SaveStateForPossibleUndo();
    f_edge_.ApplyToBuffer(current_buffer_);
  }
}

void ImageEditor::ApplyThresholdFilter(float value) {
  if (current_buffer_) {
    SaveStateForPossibleUndo();
    f_threshold_.set_cutoff(value);
    f_threshold_.ApplyToBuffer(current_buffer_);
  }
}

void ImageEditor::ApplySaturateFilter(float scale) {
  if (current_buffer_) {
    SaveStateForPossibleUndo();
    f_saturate_.set_factor(scale);
    f_saturate_.ApplyToBuffer(current_buffer_);
  }
}

void ImageEditor::ApplyChannelsFilter(float red, float green, float blue) {
  if (current_buffer_) {
    SaveStateForPossibleUndo();
    f_channels_.set_red_factor(red);
    f_channels_.set_green_factor(green);
    f_channels_.set_blue_factor(blue);
    f_channels_.ApplyToBuffer(current_buffer_);
  }
}

void ImageEditor::ApplyQuantizeFilter(int num) {
  if (current_buffer_) {
    SaveStateForPossibleUndo();
    f_quantize_.set_num_bins(num);
    f_quantize_.ApplyToBuffer(current_buffer_);
  }
}

bool ImageEditor::can_undo() { return saved_states_.size(); }

bool ImageEditor::can_redo() { return undone_states_.size(); }

void ImageEditor::Undo() {
  if (can_undo()) {
    // save state for a possilbe redo
    undone_states_.push_front(current_buffer_);

    // make the top state on the undo stack the current one
    current_buffer_ = saved_states_.front();
    saved_states_.pop_front();
  }
}

void ImageEditor::Redo() {
  if (can_redo()) {
    // save state for a possible undo
    saved_states_.push_front(current_buffer_);

    // make the top state on the redo stack the current one
    current_buffer_ = undone_states_.front();
    undone_states_.pop_front();
  }
}

void ImageEditor::SaveStateForPossibleUndo() {
  PixelBuffer *buffer_copy = new PixelBuffer(*current_buffer_);
  saved_states_.push_front(buffer_copy);

  // remove the oldest undos if we've over our limit
  while (saved_states_.size() > max_undos_) {
    delete saved_states_.back();
    saved_states_.pop_back();
  }

  // committing a new state invalidates the states saved in the redo stack,
  // so, we simply clear out this stack.
  while (!undone_states_.empty()) {
    delete undone_states_.back();
    undone_states_.pop_back();
  }
}


PixelBuffer *ImageEditor::pixel_buffer() { return current_buffer_; }

void ImageEditor::set_pixel_buffer(PixelBuffer *buffer) {
  current_buffer_ = buffer;
}

}  // namespace image_tools
