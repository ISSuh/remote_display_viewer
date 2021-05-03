/**
 *
 *  Copyright:  Copyright (c) 2020, ISSuh
 *
 */

#ifndef SRC_SCREEN_CAPTURE_DISPLAY_X11_H_
#define SRC_SCREEN_CAPTURE_DISPLAY_X11_H_

#include <map>
#include <X11/Xlib.h>

#include "base/geometry.h"

namespace rdv {

class DisplayHandle {
 public:
  DisplayHandle() : display_(nullptr) {}
  ~DisplayHandle() {}

  Display* Get() const { return display_; }

 private:
  Display* display_;
};

class Screen {
 public:
  Screen();
  Screen(DisplayHandle* display_handle, uint32_t id, Size size, Point point);
  ~Screen();

  void SetDisplayHanel(DisplayHandle* display_handle) const { return display_handle_ = display_handle; }
  void SetId(uint32_t id) { id_ = id;}
  void SetSize(Size size) { size_ = size; }
  void SetPoint(Point point) { point_ = point; }

  DisplayHandle* GetDisplayHanel() const { return display_handle_; }
  uint32_t GetId() const { return id_; }
  Size GetSize() const { return size_; }
  Point GetPoint() const { return point_; }

 private:
  DisplayHandle* display_handle_;
  uint32_t id_;
  Size size_;
  Point point_;
};

class X11Display {
 public:
  X11Display() {}
  ~X11Display() {}

  using ScreenMap = std::map<uint8_t, Screen>;

  bool UpdateScreen();
  const ScreenMap& GetScreenMap() const { return screen_map_; }

 private:
  DisplayHandle display_handle_;
  ScreenMap screen_map_;
};

}  // namespace rdv

#endif  // SRC_SCREEN_CAPTURE_DISPLAY_X11_H_
