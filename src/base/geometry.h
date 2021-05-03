/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRC_BASE_GEOMETRY_H_
#define SRC_BASE_GEOMETRY_H_

#include <iostream>
#include <string>

namespace rdv {

class Size {
 public:
  Size() : width_(0), height_(0) {}
  Size(int32_t width, int32_t height)
    : width_(width < 0 ? 0 : width), height_(height < 0 ? 0 : height) {}
  ~Size() = default;

  int32_t width() const { return width_; }
  int32_t height() const { return height_; }

  void SetWidth(int32_t width) { width_ = width < 0 ? 0 : width; }
  void SetHeight(int32_t height) { height_ = height < 0 ? 0 : height; }

  void SetSize(int32_t width, int32_t height) {
    SetWidth(width);
    SetHeight(height);
  }

  std::string ToString() {
    return std::to_string(width_) + "x" + std::to_string(height);
  }

 private:
  int32_t width_;
  int32_t height_;
};

class Point {
 public:
  Point() : x_(0), y_(0) {}
  Point(int32_t x, int32_t y)
    : x_(x < 0 ? 0 : x), y_(y < 0 ? 0 : y) {}
  ~Point() = default;

  int32_t x() const { return x_; }
  int32_t y() const { return y_; }

  void SetWidth(int32_t x) { x_ = x < 0 ? 0 : x; }
  void Sety(int32_t y) { y_ = y < 0 ? 0 : y; }

  void SetPosition(int32_t x, int32_t y) {
    SetWidth(x);
    Sety(y);
  }

  std::string ToString() {
    return std::to_string(x_) + "," + std::to_string(y_);
  }

 private:
  int32_t x_;
  int32_t y_;
};

class Rect {
 public:
 private:
  Position pos_;
  Size size_;
}

}  // namespace rdv

#endif  // SRC_BASE_GEOMETRY_H_
