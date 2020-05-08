/** CSci-4611 Assignment 2:  Car Soccer
 */

#ifndef CAR_H_
#define CAR_H_

#include <mingfx.h>

/// Small data structure for a car
class Car {
public:

    /// The constructor sets the static properties of the car, like its size,
    /// and then calls Reset() to reset the position, velocity, and any other
    /// dynamic variables that change during game play.
    Car(): size_(3, 2, 4), collision_radius_(2.5), angle_(0), speed_(0), drag_(.99) {
      Reset();
    }

    /// Nothing special needed in the constructor
    virtual~Car() {}

    /// Resets all the dynamic variables, so if you call this after a goal, the
    /// car will go back to its starting position.
    void Reset() {
      velocity_ = Vector3(0, 0, 0);
      speed_ = 0;
    }

    float collision_radius() {
      return collision_radius_;
    }
    float angle() {
      return angle_;
    }
    float speed() {
      return speed_;
    }
    float drag() {
      return drag_;
    }

    Vector3 size() {
      return size_;
    }
    Vector3 velocity() {
      return velocity_;
    }

    Point3 position() {
      return position_;
    }
    void set_position(const Point3 & p) {
      position_ = p;
    }
    void set_velocity(const Vector3 & v) {
      velocity_ = v;
    }
    void set_speed(const float & s) {
      speed_ = s;
    }
    void set_rotate(const float & r) {
      angle_ = r;
    }

private:
    // You will probably need to store some additional data here, e.g., speed.

    Vector3 size_;
    float collision_radius_;
    float angle_;
    float speed_;
    float drag_;
    Point3 position_;
    Vector3 velocity_;
};

#endif
