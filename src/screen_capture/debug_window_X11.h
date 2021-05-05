/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRC_SCREEN_CAPTURE_DEBUG_WINDOW_X11_H_
#define SRC_SCREEN_CAPTURE_DEBUG_WINDOW_X11_H_

#include <iostream>

#include "screen_capture/display_X11.h"

namespace rdv {

class WindowHandle {
 public:
  WindowHandle() : handle_(0) {}
  ~WindowHandle() {}

  Window& get() { return handle_; }

 private:
  Window handle_;
};

class DebugWindow {
 public:
  DebugWindow();
  ~DebugWindow();

  void CreateWindow(DisplayHandle* display_handle, Size size);
  void DestroyWindow(DisplayHandle* display_handle);
  WindowHandle GetWindowHandle() const { return window_; }

 private:
  WindowHandle window_;
  uint32_t width_;
  uint32_t height_;
};

}  // namespace rdv

#endif  // SRC_SCREEN_CAPTURE_DEBUG_WINDOW_X11_H_
