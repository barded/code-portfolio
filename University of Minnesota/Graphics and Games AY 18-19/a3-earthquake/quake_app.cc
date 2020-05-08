/** CSci-4611 Assignment 3:  Earthquake
 */

#include "quake_app.h"
#include "config.h"
#include "earth.h"
#include <iostream>
#include <sstream>

// Number of seconds in 1 year (approx.)
const int PLAYBACK_WINDOW = 12 * 28 * 24 * 60 * 60;

using namespace std;

QuakeApp::QuakeApp(): GraphicsApp(1280, 720, "Earthquake"),
  playback_scale_(15000000.0), debug_mode_(false), flat_(true), t(0), trans_(false) {
    // Define a search path for finding data files (images and earthquake db)
    search_path_.push_back(".");
    search_path_.push_back("./data");
    search_path_.push_back(DATA_DIR_INSTALL);
    search_path_.push_back(DATA_DIR_BUILD);

    quake_db_ = EarthquakeDatabase(Platform::FindFile("earthquakes.txt", search_path_));
    current_time_ = quake_db_.earthquake(quake_db_.min_index()).date().ToSeconds();

    c2 = Color(1, 0, 0);
    c1 = Color(1, .93, 0);

  }

QuakeApp::~QuakeApp() {}

void QuakeApp::InitNanoGUI() {
  // Setup the GUI window
  nanogui::Window * window = new nanogui::Window(screen(), "Earthquake Controls");
  window - > setPosition(Eigen::Vector2i(10, 10));
  window - > setSize(Eigen::Vector2i(400, 200));
  window - > setLayout(new nanogui::GroupLayout());

  date_label_ = new nanogui::Label(window, "Current Date: MM/DD/YYYY", "sans-bold");

  globe_btn_ = new nanogui::Button(window, "Globe");
  globe_btn_ - > setCallback(std::bind( &QuakeApp::OnGlobeBtnPressed, this));
  globe_btn_ - > setTooltip("Toggle between map and globe.");

  new nanogui::Label(window, "Playback Speed", "sans-bold");

  nanogui::Widget * panel = new nanogui::Widget(window);
  panel - > setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
    nanogui::Alignment::Middle, 0, 20));

  nanogui::Slider * slider = new nanogui::Slider(panel);
  slider - > setValue(0.5 f);
  slider - > setFixedWidth(120);

  speed_box_ = new nanogui::TextBox(panel);
  speed_box_ - > setFixedSize(Eigen::Vector2i(60, 25));
  speed_box_ - > setValue("50");
  speed_box_ - > setUnits("%");
  slider - > setCallback(std::bind( &QuakeApp::OnSliderUpdate, this, std::placeholders::_1));
  speed_box_ - > setFixedSize(Eigen::Vector2i(60, 25));
  speed_box_ - > setFontSize(20);
  speed_box_ - > setAlignment(nanogui::TextBox::Alignment::Right);

  nanogui::Button * debug_btn = new nanogui::Button(window, "Toggle Debug Mode");
  debug_btn - > setCallback(std::bind( &QuakeApp::OnDebugBtnPressed, this));
  debug_btn - > setTooltip("Toggle displaying mesh triangles and normals (can be slow)");

  screen() - > performLayout();
}

void QuakeApp::OnLeftMouseDrag(const Point2 &pos,
  const Vector2 &delta) {
  // Optional: In our demo, we adjust the tilt of the globe here when the
  // mouse is dragged up/down on the screen.
}

void QuakeApp::OnGlobeBtnPressed() {
  // TODO: This is where you can switch between flat earth mode and globe mode

  flat_ = !flat_;
  trans_ = true;

}

void QuakeApp::OnDebugBtnPressed() {
  debug_mode_ = !debug_mode_;
}

void QuakeApp::OnSliderUpdate(float value) {
  speed_box_ - > setValue(std::to_string((int)(value * 100)));
  playback_scale_ = 30000000.0 * value;
}

void QuakeApp::UpdateSimulation(double dt) {
  // Advance the current time and loop back to the start if time is past the last earthquake
  current_time_ += playback_scale_ * dt;
  if (current_time_ > quake_db_.earthquake(quake_db_.max_index()).date().ToSeconds()) {
    current_time_ = quake_db_.earthquake(quake_db_.min_index()).date().ToSeconds();
  }
  if (current_time_ < quake_db_.earthquake(quake_db_.min_index()).date().ToSeconds()) {
    current_time_ = quake_db_.earthquake(quake_db_.max_index()).date().ToSeconds();
  }

  double next_time_ = current_time_ + playback_scale_ * dt * 2.0;
  if (next_time_ > quake_db_.earthquake(quake_db_.max_index()).date().ToSeconds()) {
    next_time_ = quake_db_.earthquake(quake_db_.min_index()).date().ToSeconds();
  }
  if (next_time_ < quake_db_.earthquake(quake_db_.min_index()).date().ToSeconds()) {
    next_time_ = quake_db_.earthquake(quake_db_.max_index()).date().ToSeconds();
  }

  Date d(current_time_);
  Date d2(next_time_);

  stringstream s;
  s << "Current date: " << d.month() <<
    "/" << d.day() <<
    "/" << d.year();
  date_label_ - > setCaption(s.str());

  // TODO: Any animation, morphing, rotation of the earth, or other things that should
  // be updated once each frame would go here.

  float rfactor = M_PI / 20.0f;
  float mspeed = 1.2;

  if (flat_) {
    for (int i = 0; i < factive.size(); i++) {
      factive[i].magnitude_ -= dt * mspeed;
      if (factive[i].magnitude_ < 0) {
        factive.erase(factive.begin() + i);
        i--;
      }
      factive[i].color_ = c1.Lerp(c2, factive[i].magnitude_ / 9.6);
      factive[i].radius_ = factive[i].magnitude_ / 9.6 * rfactor;
    }

  } else {
    if (!trans_) {
      if (earth_.rotate >= 2 * M_PI) {
        earth_.rotate -= 2 * M_PI;
      }
      earth_.rotate += dt / 2.0;
    }

    for (int i = 0; i < ractive.size(); i++) {
      ractive[i].magnitude_ -= dt * mspeed;
      if (ractive[i].magnitude_ < 0) {
        ractive.erase(ractive.begin() + i);
        i--;
      }
      ractive[i].color_ = c1.Lerp(c2, ractive[i].magnitude_ / 9.6);
      ractive[i].radius_ = ractive[i].magnitude_ / 9.6 * rfactor;
    }
  }

  int begin = quake_db_.FindMostRecentQuake(d);
  int end = quake_db_.FindMostRecentQuake(d2);

  for (int j = begin; j <= end; j++) {
    Earthquake e = quake_db_.earthquake(j);

    Color c = c1.Lerp(c2, e.magnitude() / 9.6);
    if (flat_) {
      Dots
      dot = Dots(earth_.LatLongToPlane(e.latitude(), e.longitude()), e.magnitude(),
        c, e.magnitude() / 9.0 * rfactor);
      factive.push_back(dot);
    } else {
      Dots
      dot = Dots(earth_.LatLongToSphere(e.latitude(), e.longitude()), e.magnitude(),
        c, e.magnitude() / 9.0 * rfactor);
      ractive.push_back(dot);
    }
  }
  if (t > 1 || t < 0) {
    trans_ = false;

    if (flat_) {
      t = .01;
      earth_.earth_mesh_.SetNormals(earth_.fnormals);
      earth_.earth_mesh_.SetVertices(earth_.fvertices);
      earth_.earth_mesh_.UpdateGPUMemory();
      earth_.rotate = 0;
    } else {
      t = .99;
      earth_.earth_mesh_.SetNormals(earth_.rnormals);
      earth_.earth_mesh_.SetVertices(earth_.rvertices);
      earth_.earth_mesh_.UpdateGPUMemory();

    }

  }

  if (trans_) {
    if (flat_) {
      t -= dt / 4.0;
      earth_.rotate /= 1.1;
      for (int i = 0; i < earth_.tvertices.size(); i++) {
        earth_.tvertices[i] = earth_.fvertices[i].Lerp(earth_.rvertices[i], t);
        earth_.tnormals[i] = earth_.fnormals[i].Lerp(earth_.rnormals[i], t);

      }

      earth_.earth_mesh_.SetNormals(earth_.tnormals);
      earth_.earth_mesh_.SetVertices(earth_.tvertices);
      earth_.earth_mesh_.UpdateGPUMemory();
    } else {
      earth_.rotate += dt / 2.0;

      t += dt / 4.0;
      for (int i = 0; i < earth_.tvertices.size(); i++) {
        earth_.tvertices[i] = earth_.fvertices[i].Lerp(earth_.rvertices[i], t);
        earth_.tnormals[i] = earth_.fnormals[i].Lerp(earth_.rnormals[i], t);

      }
      earth_.earth_mesh_.SetNormals(earth_.tnormals);
      earth_.earth_mesh_.SetVertices(earth_.tvertices);
      earth_.earth_mesh_.UpdateGPUMemory();
    }
  }

}

void QuakeApp::InitOpenGL() {
  // Set up the camera in a good position to see the entire earth in either mode
  proj_matrix_ = Matrix4::Perspective(60, aspect_ratio(), 0.1, 50);
  view_matrix_ = Matrix4::LookAt(Point3(0, 0, 3.5), Point3(0, 0, 0), Vector3(0, 1, 0));
  glClearColor(0.0, 0.0, 0.0, 1);

  // Initialize the earth object
  earth_.Init(search_path_);

  // Initialize the texture used for the background image
  stars_tex_.InitFromFile(Platform::FindFile("iss006e40544.png", search_path_));
}

void QuakeApp::DrawUsingOpenGL() {
  quick_shapes_.DrawFullscreenTexture(Color(1, 1, 1), stars_tex_);

  // You can leave this as the identity matrix and we will have a fine view of
  // the earth.  If you want to add any rotation or other animation of the
  // earth, the model_matrix is where you would apply that.
  Matrix4 model_matrix;

  // Draw the earth
  earth_.Draw(model_matrix, view_matrix_, proj_matrix_);
  if (debug_mode_) {
    earth_.DrawDebugInfo(model_matrix, view_matrix_, proj_matrix_);
  }

  // TODO: You'll also need to draw the earthquakes.  It's up to you exactly
  // how you wish to do that.
  if (flat_) {
    for (int i = 0; i < factive.size(); i++) {
      float r = factive[i].radius_;

      Matrix4 Mdot =
        Matrix4::Translation(factive[i].pos_ - Point3(0, 0, 0)) *
        Matrix4::Scale(Vector3(r, r, r)) *
        Matrix4::RotationY(earth_.rotate);

      quick_shapes_.DrawSphere(model_matrix * Mdot, view_matrix_, proj_matrix_, factive[i].color_);
    }
  } else {
    for (int i = 0; i < ractive.size(); i++) {
      float r = ractive[i].radius_;

      Matrix4 Mdot =
        Matrix4::RotationY(earth_.rotate) *
        Matrix4::Translation(ractive[i].pos_ - Point3(0, 0, 0)) *
        Matrix4::Scale(Vector3(r, r, r));
      quick_shapes_.DrawSphere(model_matrix * Mdot, view_matrix_, proj_matrix_, ractive[i].color_);
    }
  }

}
