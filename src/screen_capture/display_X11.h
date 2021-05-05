/**
 *
 *  Copyright:  Copyright (c) 2020, ISSuh
 *
 */

#ifndef SRC_SCREEN_CAPTURE_DISPLAY_X11_H_
#define SRC_SCREEN_CAPTURE_DISPLAY_X11_H_

#include <X11/Xlib.h>
#include <map>

#include "base/geometry.h"

namespace rdv {

class DisplayHandle {
 public:
  DisplayHandle() : display_(XOpenDisplay(NULL)) {}
  explicit DisplayHandle(Display* display) : display_(display) {}
  ~DisplayHandle() { XCloseDisplay(display_); }

  Display* get() const { return display_; }

 private:
  Display* display_;
};

class Screen {
 public:
  Screen();
  Screen(DisplayHandle* display_handle, uint32_t id, Rect rect);
  ~Screen();

  DisplayHandle* GetDisplayHanel() const { return display_handle_; }
  uint32_t GetId() const { return id_; }
  Rect GetRect() const { return rect_; }
  Size GetSize() const { return rect_.size(); }
  Point GetPoint() const { return rect_.point(); }

 private:
  DisplayHandle* display_handle_;
  uint32_t id_;
  Rect rect_;
};

class X11Display {
 public:
  using ScreenMap = std::map<uint32_t, Screen>;

  X11Display();
  ~X11Display();

  DisplayHandle* GetDisplayHandle() { return &display_handle_; }
  const ScreenMap& GetScreenMap();

 private:
  bool UpdateScreen();

  DisplayHandle display_handle_;
  ScreenMap screen_map_;
};

}  // namespace rdv

#endif  // SRC_SCREEN_CAPTURE_DISPLAY_X11_H_
