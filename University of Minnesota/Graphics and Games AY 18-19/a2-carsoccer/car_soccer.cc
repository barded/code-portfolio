/** CSci-4611 Assignment 2:  Car Soccer
 */

#include "car_soccer.h"
#include "config.h"


CarSoccer::CarSoccer(): GraphicsApp(1024, 768, "Car Soccer"), time_(0.0) {
  // Define a search path for finding data files (images and shaders)
  searchPath_.push_back(".");
  searchPath_.push_back("./data");
  searchPath_.push_back(DATA_DIR_INSTALL);
  searchPath_.push_back(DATA_DIR_BUILD);

  // on program start
  car_.set_rotate(-M_PI / 2.0);
  Vector3 s = car_.size();
  car_.set_position(Point3(0, s[1] / 2, 45));
  car2_.set_rotate(M_PI / 2.0);
  car2_.set_position(Point3(0, s[1] / 2, -45));
  int min = -20;
  int max = 20;
  float r1 = rand() % (max - min + 1) + min;
  float r2 = rand() % (max - min + 1) + min;
  float r3 = rand() % (max - min + 1) + min;

  Vector3 v = Vector3(r1, r2, r3);
  ball_.set_velocity(v);

}

CarSoccer::~CarSoccer() {}

Vector2 CarSoccer::joystick_direction() {
  Vector2 dir;
  if (IsKeyDown(GLFW_KEY_LEFT))
    car_.set_rotate(car_.angle() - .05);

  if (IsKeyDown(GLFW_KEY_RIGHT))
    car_.set_rotate(car_.angle() + .05);

  if (IsKeyDown(GLFW_KEY_UP)) {
    if (car_.speed() < 0) {
      car_.set_speed(0);
    } else
      car_.set_speed(car_.speed() + 2);
  }
  if (IsKeyDown(GLFW_KEY_DOWN)) {
    if (car_.speed() > 0) {
      car_.set_speed(0);
    } else
      car_.set_speed(car_.speed() - 2);
  }

  if (IsKeyDown(GLFW_KEY_A))
    car2_.set_rotate(car2_.angle() - .05);

  if (IsKeyDown(GLFW_KEY_D))
    car2_.set_rotate(car2_.angle() + .05);

  if (IsKeyDown(GLFW_KEY_W)) {
    if (car2_.speed() < 0) {
      car2_.set_speed(0);
    } else
      car2_.set_speed(car2_.speed() + 2);
  }
  if (IsKeyDown(GLFW_KEY_S)) {
    if (car2_.speed() > 0) {
      car2_.set_speed(0);
    } else
      car2_.set_speed(car2_.speed() - 2);
  }

  return dir;
}

void CarSoccer::launch_ball() {
  ball_.Reset();
  int min = -200;
  int max = 200;
  float r1 = rand() % (max - min + 1) + min;
  float r2 = rand() % (max - min + 1) + min;
  float r3 = rand() % (max - min + 1) + min;

  Vector3 v = Vector3(r1, r2, r3);
  // Vector3 v=Vector3(0,0,40);

  ball_.set_velocity(v);
}

void CarSoccer::OnSpecialKeyDown(int key, int scancode, int modifiers) {
  if (key == GLFW_KEY_SPACE) {
    // Here's where you could call some form of launch_ball();
    launch_ball();

  }

  if (key == GLFW_KEY_ENTER) {
    ball_.set_velocity(Vector3(0, 0, 0));
    Point3 p = ball_.position();
    p[1] = ball_.radius();
    ball_.set_position(p);
  }
}

void CarSoccer::UpdateSimulation(double timeStep) {
  // Here's where you shound do your "simulation", updating the positions of the
  // car and ball as needed and checking for collisions.  Filling this routine
  // in is the main part of the assignment.
  Vector2 dir = joystick_direction(); //  for keyboard input

  time_ += timeStep;

  Point3 ball_p = ball_.position() + ball_.velocity() * timeStep;
  ball_.set_position(ball_p);

  Vector3 vnew = ball_.velocity() + gravity * timeStep;
  ball_.set_velocity(vnew);

  if (fabs(vnew[0]) < .1 && fabs(vnew[2]) < .1 && fabs(vnew[1]) <= fabs(gravity[1]) * timeStep && ball_p[1] <= ball_.radius() + ball_.radius() * .5) {
    vnew[1] = 0;
    ball_p[1] = ball_.radius();
    ball_.set_velocity(vnew);
    ball_.set_position(ball_p);

  }

  car_.set_speed(car_.speed() * car_.drag());
  Vector3 carv = Vector3(cos(car_.angle()), 0, sin(car_.angle())) * car_.speed();
  car_.set_velocity(carv);

  Point3 car_p = car_.position() + car_.velocity() * timeStep;
  car_.set_position(car_p);

  car_p = car_.position();

  Vector3 d = ball_p - car_p;

  car2_.set_speed(car2_.speed() * car2_.drag());
  Vector3 car2v = Vector3(cos(car2_.angle()), 0, sin(car2_.angle())) * car2_.speed();
  car2_.set_velocity(car2v);

  Point3 car2_p = car2_.position() + car2_.velocity() * timeStep;
  car2_.set_position(car2_p);

  car2_p = car2_.position();

  Vector3 d2 = ball_p - car2_p;

  if (d.Length() < ball_.radius() + car_.collision_radius()) { //   hits car
    float r_car = car_.collision_radius();
    float r_ball = ball_.radius();
    float dist = r_car + r_ball - d.Length();
    Vector3 n = d.ToUnit();

    Point3 newball_p = ball_.position() + dist * n;
    Vector3 rel = ball_.velocity() - car_.velocity();
    Vector3 reflect = (rel - 2.0 * rel.Dot(n) * n) * bouncedamp;
    ball_.set_position(newball_p);
    Vector3 vball = reflect + car_.velocity();
    ball_.set_velocity(vball);
    ball_p = ball_.position();

  } else if (d2.Length() < ball_.radius() + car2_.collision_radius()) { //   hits car2
    float r_car2 = car2_.collision_radius();
    float r_ball = ball_.radius();
    float dist = r_car2 + r_ball - d2.Length();
    Vector3 n = d2.ToUnit();

    Point3 newball_p = ball_.position() + dist * n;
    Vector3 rel = ball_.velocity() - car2_.velocity();
    Vector3 reflect = (rel - 2.0 * rel.Dot(n) * n) * bouncedamp;
    ball_.set_position(newball_p);
    Vector3 vball = reflect + car2_.velocity();
    ball_.set_velocity(vball);
    ball_p = ball_.position();
  } else {

    if (car_p[0] > 40 - car_.collision_radius()) { //   hits right
      car_p[0] = 40 - car_.collision_radius();
      car_.set_position(car_p);
      car_.set_speed(0);
    }

    if (car_p[0] < -40 + car_.collision_radius()) { //   hits left
      car_p[0] = -40 + car_.collision_radius();
      car_.set_position(car_p);
      car_.set_speed(0);
    }

    if (car_p[2] > 50 - car_.collision_radius()) { //   hits close
      car_p[2] = 50 - car_.collision_radius();
      car_.set_position(car_p);
      car_.set_speed(0);
    }

    if (car_p[2] < -50 + car_.collision_radius()) { //   hits far
      car_p[2] = -50 + car_.collision_radius();
      car_.set_position(car_p);
      car_.set_speed(0);
    }

    if (car2_p[0] > 40 - car2_.collision_radius()) { //   hits right
      car2_p[0] = 40 - car2_.collision_radius();
      car2_.set_position(car2_p);
      car2_.set_speed(0);
    }

    if (car2_p[0] < -40 + car2_.collision_radius()) { //   hits left
      car2_p[0] = -40 + car2_.collision_radius();
      car2_.set_position(car2_p);
      car2_.set_speed(0);
    }

    if (car2_p[2] > 50 - car2_.collision_radius()) { //   hits close
      car2_p[2] = 50 - car2_.collision_radius();
      car2_.set_position(car2_p);
      car2_.set_speed(0);
    }

    if (car2_p[2] < -50 + car2_.collision_radius()) { //   hits far
      car2_p[2] = -50 + car2_.collision_radius();
      car2_.set_position(car2_p);
      car2_.set_speed(0);
    }

    // ball collisions
    //                    GOAL
    if (ball_p[0] > -10 && ball_p[0] < 10 && ball_p[1] >= ball_.radius() - 2 && ball_p[1] < 10 && (ball_p[2] < -50 + ball_.radius() || ball_p[2] > 50 - ball_.radius())) {
      launch_ball();
      car_.Reset();
      car2_.Reset();
      car_.set_rotate(-M_PI / 2.0);
      Vector3 s = car_.size();
      car_.set_position(Point3(0, s[1] / 2, 45));
      car2_.set_rotate(M_PI / 2.0);
      car2_.set_position(Point3(0, s[1] / 2, -45));

    } else {

      if (ball_p[1] < ball_.radius()) { //   hits floor
        Vector3 n = Vector3(0, 1, 0);
        ball_p[1] = ball_.radius();
        Vector3 reflect = (ball_.velocity() - 2.0 * ball_.velocity().Dot(n) * n) * bouncedamp;
        ball_.set_position(ball_p);
        ball_.set_velocity(reflect);
        ball_p = ball_.position();
      }

      if (ball_p[1] > 35 - ball_.radius()) { //   hits ceil
        Vector3 n = Vector3(0, -1, 0);
        ball_p[1] = 35 - ball_.radius();
        Vector3 reflect = (ball_.velocity() - 2.0 * ball_.velocity().Dot(n) * n) * bouncedamp;
        ball_.set_position(ball_p);
        ball_.set_velocity(reflect);
        ball_p = ball_.position();
      }

      if (ball_p[0] > 40 - ball_.radius()) { //   hits right
        Vector3 n = Vector3(-1, 0, 0);
        ball_p[0] = 40 - ball_.radius();
        Vector3 reflect = (ball_.velocity() - 2.0 * ball_.velocity().Dot(n) * n) * bouncedamp;
        ball_.set_position(ball_p);
        ball_.set_velocity(reflect);
        ball_p = ball_.position();
      }

      if (ball_p[0] < -40 + ball_.radius()) { //   hits left
        Vector3 n = Vector3(1, 0, 0);
        ball_p[0] = -40 + ball_.radius();
        Vector3 reflect = (ball_.velocity() - 2.0 * ball_.velocity().Dot(n) * n) * bouncedamp;
        ball_.set_position(ball_p);
        ball_.set_velocity(reflect);
        ball_p = ball_.position();
      }

      if (ball_p[2] > 50 - ball_.radius()) { //   hits close
        Vector3 n = Vector3(0, 0, -1);
        ball_p[2] = 50 - ball_.radius();
        Vector3 reflect = (ball_.velocity() - 2.0 * ball_.velocity().Dot(n) * n) * bouncedamp;
        ball_.set_position(ball_p);
        ball_.set_velocity(reflect);
        ball_p = ball_.position();
      }

      if (ball_p[2] < -50 + ball_.radius()) { //   hits far
        Vector3 n = Vector3(0, 0, 1);
        ball_p[2] = -50 + ball_.radius();
        Vector3 reflect = (ball_.velocity() - 2.0 * ball_.velocity().Dot(n) * n) * bouncedamp;
        ball_.set_position(ball_p);
        ball_.set_velocity(reflect);
        ball_p = ball_.position();
      }

    }
  }

}

void CarSoccer::InitOpenGL() {
  // Set up the camera in a good position to see the entire field
  projMatrix_ = Matrix4::Perspective(60, aspect_ratio(), 1, 1000);
  modelMatrix_ = Matrix4::LookAt(Point3(0, 60, 70), Point3(0, 0, 10), Vector3(0, 1, 0));

  // Set a background color for the screen
  glClearColor(0.8, 0.8, 0.8, 1);

  // Load some image files we'll use
  fieldTex_.InitFromFile(Platform::FindFile("pitch.png", searchPath_));
  crowdTex_.InitFromFile(Platform::FindFile("crowd.png", searchPath_));
}

void CarSoccer::DrawUsingOpenGL() {
  // Draw the crowd as a fullscreen background image
  quickShapes_.DrawFullscreenTexture(Color(1, 1, 1), crowdTex_);

  // Draw the field with the field texture on it.
  Color col(16.0 / 255.0, 46.0 / 255.0, 9.0 / 255.0);
  Matrix4 M = Matrix4::Translation(Vector3(0, -0.201, 0)) * Matrix4::Scale(Vector3(50, 1, 60));
  quickShapes_.DrawSquare(modelMatrix_ * M, viewMatrix_, projMatrix_, col);
  M = Matrix4::Translation(Vector3(0, -0.2, 0)) * Matrix4::Scale(Vector3(40, 1, 50));
  quickShapes_.DrawSquare(modelMatrix_ * M, viewMatrix_, projMatrix_, Color(1, 1, 1), fieldTex_);

  // Draw the car
  Color carcol(0.8, 0.2, 0.2);
  Matrix4 Mcar =
    Matrix4::Translation(car_.position() - Point3(0, 0, 0)) *
    Matrix4::Scale(car_.size()) *
    Matrix4::Scale(Vector3(0.5, 0.5, 0.5)) *
    Matrix4::RotationY(-car_.angle());
  quickShapes_.DrawCube(modelMatrix_ * Mcar, viewMatrix_, projMatrix_, carcol);

  Color car2col(0, 0, 0.6);
  Matrix4 Mcar2 =
    Matrix4::Translation(car2_.position() - Point3(0, 0, 0)) *
    Matrix4::Scale(car2_.size()) *
    Matrix4::Scale(Vector3(0.5, 0.5, 0.5)) *
    Matrix4::RotationY(-car2_.angle());
  quickShapes_.DrawCube(modelMatrix_ * Mcar2, viewMatrix_, projMatrix_, car2col);

  // Draw the ball
  Color ballcol(1, 1, 1);
  Matrix4 Mball =
    Matrix4::Translation(ball_.position() - Point3(0, 0, 0)) *
    Matrix4::Scale(Vector3(ball_.radius(), ball_.radius(), ball_.radius()));
  quickShapes_.DrawSphere(modelMatrix_ * Mball, viewMatrix_, projMatrix_, ballcol);

  // Draw the ball's shadow -- this is a bit of a hack, scaling Y by zero
  // flattens the sphere into a pancake, which we then draw just a bit
  // above the ground plane.
  Color shadowcol(0.2, 0.4, 0.15);
  Matrix4 Mshadow =
    Matrix4::Translation(Vector3(ball_.position()[0], -0.1, ball_.position()[2])) *
    Matrix4::Scale(Vector3(ball_.radius(), 0, ball_.radius())) *
    Matrix4::RotationX(90);
  quickShapes_.DrawSphere(modelMatrix_ * Mshadow, viewMatrix_, projMatrix_, shadowcol);

  Vector3 dir = 8 * car_.velocity();
  // quickShapes_.DrawArrow(modelMatrix_, viewMatrix_, projMatrix_,Color(1,1,0),car_.position(),dir,.1);
  // quickShapes_.DrawArrow(modelMatrix_, viewMatrix_, projMatrix_,Color(1,1,0),ball_.position(),ball_.velocity(),.2);

  Vector3 carv = Vector3(cos(car_.angle()), 0, sin(car_.angle()));
  quickShapes_.DrawArrow(modelMatrix_, viewMatrix_, projMatrix_, Color(1, 1, 0), car_.position(), carv * 3.0, .2);

  Vector3 car2v = Vector3(cos(car2_.angle()), 0, sin(car2_.angle()));
  quickShapes_.DrawArrow(modelMatrix_, viewMatrix_, projMatrix_, Color(1, 1, 0), car2_.position(), car2v * 3.0, .2);

  // You should add drawing the goals and the boundary of the playing area
  // using quickShapes_.DrawLines()

  Matrix4 m_boundary;
  std::vector < Point3 > boundary;
  boundary.push_back(Point3(-40., 0, 50.));
  boundary.push_back(Point3(-40., 0, -50.));
  boundary.push_back(Point3(40., 0, -50.));
  boundary.push_back(Point3(40., 0, 50));
  boundary.push_back(Point3(-40., 0, 50.));
  boundary.push_back(Point3(-40., 35., 50.));
  boundary.push_back(Point3(-40., 35., -50.));
  boundary.push_back(Point3(-40., 0, -50.));
  boundary.push_back(Point3(-40., 35., -50.));
  boundary.push_back(Point3(40., 35., -50.));
  boundary.push_back(Point3(40., 0, -50.));
  boundary.push_back(Point3(40., 35., -50.));
  boundary.push_back(Point3(40., 35., 50.));
  boundary.push_back(Point3(40., 0, 50));
  boundary.push_back(Point3(40., 35., 50.));
  boundary.push_back(Point3(-40., 35., 50.));
  quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, Color(.5, 0, 0, .3), boundary, QuickShapes::LinesType::LINE_LOOP, 0.2);

  Matrix4 m_neargoal;
  std::vector < Point3 > neargoal;
  neargoal.push_back(Point3(-10, 0, 50));
  neargoal.push_back(Point3(-10, 10, 50));
  neargoal.push_back(Point3(10, 10, 50));
  neargoal.push_back(Point3(10, 0, 50));
  neargoal.push_back(Point3(-10, 0, 50));
  neargoal.push_back(Point3(-10, 2, 50));
  neargoal.push_back(Point3(10, 2, 50));
  neargoal.push_back(Point3(10, 4, 50));
  neargoal.push_back(Point3(-10, 4, 50));
  neargoal.push_back(Point3(-10, 6, 50));
  neargoal.push_back(Point3(10, 6, 50));
  neargoal.push_back(Point3(10, 8, 50));
  neargoal.push_back(Point3(-10, 8, 50));
  neargoal.push_back(Point3(-10, 10, 50));
  neargoal.push_back(Point3(-8, 10, 50));
  neargoal.push_back(Point3(-8, 0, 50));
  neargoal.push_back(Point3(-6, 0, 50));
  neargoal.push_back(Point3(-6, 10, 50));
  neargoal.push_back(Point3(-4, 10, 50));
  neargoal.push_back(Point3(-4, 0, 50));
  neargoal.push_back(Point3(-2, 0, 50));
  neargoal.push_back(Point3(-2, 10, 50));
  neargoal.push_back(Point3(0, 10, 50));
  neargoal.push_back(Point3(0, 0, 50));
  neargoal.push_back(Point3(2, 0, 50));
  neargoal.push_back(Point3(2, 10, 50));
  neargoal.push_back(Point3(4, 10, 50));
  neargoal.push_back(Point3(4, 0, 50));
  neargoal.push_back(Point3(6, 0, 50));
  neargoal.push_back(Point3(6, 10, 50));
  neargoal.push_back(Point3(8, 10, 50));
  neargoal.push_back(Point3(8, 0, 50));
  quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, Color(1, 1, 1, 1), neargoal, QuickShapes::LinesType::LINE_LOOP, 0.1);

  Matrix4 m_fargoal;
  std::vector < Point3 > fargoal;
  fargoal.push_back(Point3(-10, 0, -50));
  fargoal.push_back(Point3(-10, 10, -50));
  fargoal.push_back(Point3(10, 10, -50));
  fargoal.push_back(Point3(10, 0, -50));
  fargoal.push_back(Point3(-10, 0, -50));
  fargoal.push_back(Point3(-10, 2, -50));
  fargoal.push_back(Point3(10, 2, -50));
  fargoal.push_back(Point3(10, 4, -50));
  fargoal.push_back(Point3(-10, 4, -50));
  fargoal.push_back(Point3(-10, 6, -50));
  fargoal.push_back(Point3(10, 6, -50));
  fargoal.push_back(Point3(10, 8, -50));
  fargoal.push_back(Point3(-10, 8, -50));
  fargoal.push_back(Point3(-10, 10, -50));
  fargoal.push_back(Point3(-8, 10, -50));
  fargoal.push_back(Point3(-8, 0, -50));
  fargoal.push_back(Point3(-6, 0, -50));
  fargoal.push_back(Point3(-6, 10, -50));
  fargoal.push_back(Point3(-4, 10, -50));
  fargoal.push_back(Point3(-4, 0, -50));
  fargoal.push_back(Point3(-2, 0, -50));
  fargoal.push_back(Point3(-2, 10, -50));
  fargoal.push_back(Point3(0, 10, -50));
  fargoal.push_back(Point3(0, 0, -50));
  fargoal.push_back(Point3(2, 0, -50));
  fargoal.push_back(Point3(2, 10, -50));
  fargoal.push_back(Point3(4, 10, -50));
  fargoal.push_back(Point3(4, 0, -50));
  fargoal.push_back(Point3(6, 0, -50));
  fargoal.push_back(Point3(6, 10, -50));
  fargoal.push_back(Point3(8, 10, -50));
  fargoal.push_back(Point3(8, 0, -50));
  quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, Color(1, 1, 1, 1), fargoal, QuickShapes::LinesType::LINE_LOOP, 0.15);

}
