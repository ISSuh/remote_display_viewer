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

  bool IsEmpty() { return (width_ == 0 || height_ == 0); }

  std::string ToString() {
    return std::to_string(width_) + "x" + std::to_string(height_);
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

  void Setx(int32_t x) { x_ = x < 0 ? 0 : x; }
  void Sety(int32_t y) { y_ = y < 0 ? 0 : y; }

  void SetPoint(int32_t x, int32_t y) {
    Setx(x);
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
  Rect() : size_(Size(0, 0)), pos_(Point(0, 0)) {}
  Rect(Size size, Point pos) : size_(size), pos_(pos) {}
  Rect(int32_t witdh, int32_t height, int32_t x, int32_t y)
    : size_(Size(witdh, height)), pos_(Point(x, y)) {}
  ~Rect() {}

  Size size() const { return size_; }
  Point point() const { return pos_; }

  int32_t width() const { return size_.width(); }
  int32_t height() const { return size_.height(); }
  int32_t x() const { return pos_.x(); }
  int32_t y() const { return pos_.y(); }

  void SetSize(Size size) { size_ = size; }
  void SetSize(int32_t width, int32_t height) { size_.SetSize(width, height) ; }

  void SettPoint(Point pos) { pos_ = pos; }
  void SettPoint(int32_t x, int32_t y) { pos_.SetPoint(x, y); }

  bool IsEmpty() { return size_.IsEmpty(); }

  std::string ToString() {
    return size_.ToString() + "/" + pos_.ToString();
  }

 private:
  Size size_;
  Point pos_;
};

}  // namespace rdv

#endif  // SRC_BASE_GEOMETRY_H_
