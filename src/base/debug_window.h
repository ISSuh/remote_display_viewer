/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRC_BASE_DEBUG_WINDOW_H_
#define SRC_BASE_DEBUG_WINDOW_H_

#include <iostream>
#include <X11/Xlib.h>

namespace rdv {

class WindowHandle {
 public:
  WindowHandle() : handle_(0) {}
  ~WindowHandle() {}

  Window& getHandle() { return handle_; }

 private:
  Window handle_;
};

class DebugWindow {
 public:
  DebugWindow() : width_(0), height_(0) {}
  ~DebugWindow() {}

  void CreateWindow(Display* display, int width, int height) {

  }

  WindowHandle GetWindow() {}

 private:
  WindowHandle window_;
  uint32_t width_;
  uint32_t height_;
};

}  // namespcae rdv

#endif  // SRC_BASE_DEBUG_WINDOW_H_
