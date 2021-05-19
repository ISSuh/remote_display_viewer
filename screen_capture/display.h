/**
 *
 *  Copyright:  Copyright (c) 2020, ISSuh
 *
 */

#ifndef SRC_SCREEN_CAPTURE_DISPLAY_H_
#define SRC_SCREEN_CAPTURE_DISPLAY_H_

#include <X11/Xlib.h>
#include <map>

#include "geometry.h"

namespace rdv {

class DisplayHandle {
 public:
  DisplayHandle() : display_(XOpenDisplay(nullptr)) {}
  explicit DisplayHandle(Display* display) : display_(display) {}
  ~DisplayHandle() {
    if (display_) {
      XCloseDisplay(display_);
    }
  }

  Display* get() const { return display_; }

 private:
  Display* display_;
};

class ScreenInfo {
 public:
  ScreenInfo();
  ScreenInfo(DisplayHandle* display_handle, uint32_t id, Rect rect);
  ~ScreenInfo();

  ScreenInfo(const ScreenInfo& info) {
    display_handle_ = info.display_handle_;
    id_ = info.id_;
    rect_ = info.rect_;
  }

  DisplayHandle* GetDisplayHanel() const { return display_handle_; }
  uint32_t GetId() const { return id_; }
  Rect GetRect() const { return rect_; }

  ScreenInfo& operator=(const ScreenInfo& info) {
    if (&info == this) {
      return *this;
    }

    display_handle_ = info.display_handle_;
    id_ = info.id_;
    rect_ = info.rect_;
  }

 private:
  DisplayHandle* display_handle_;
  uint32_t id_;
  Rect rect_;
};

class X11Display {
 public:
  using ScreenMap = std::map<uint32_t, ScreenInfo>;

  X11Display();
  ~X11Display();

  DisplayHandle* GetDisplayHandle() { return &display_handle_; }
  ScreenMap GetScreenMap();

 private:
  bool UpdateScreen();

  DisplayHandle display_handle_;
  ScreenMap screen_map_;
};

}  // namespace rdv

#endif  // SRC_SCREEN_CAPTURE_DISPLAY_H_
