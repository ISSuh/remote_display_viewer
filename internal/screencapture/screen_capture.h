/**
 *
 *  Copyright:  Copyright (c) 2020, ISSuh
 *
 */

#ifndef SRC_SCREEN_CAPTURE_SCREEN_CAPTURE_H_
#define SRC_SCREEN_CAPTURE_SCREEN_CAPTURE_H_


#include <vector>

#include "geometry.h"

namespace rdv {

class Screens {
 public:
  Screens();
  ~Screens();

  void* handle() { return screen_handle_; }
  void* GetRootWindowHandle();

  uint8_t NumberOfScreen();
  Rect ScreenRect(uint8_t screen_id);

 private:
  void* screen_handle_;
};

class ScreenCapture {
 public:
  ScreenCapture();
  ~ScreenCapture();

  static const int IMAGE_PLANE = 3;

  uint8_t NumberOfScreen() { return screens_.NumberOfScreen(); }
  Rect ScreenRect(uint8_t screen_id) { return screens_.ScreenRect(screen_id); }

  void Capture(uint8_t screen_id, uint8_t* image);

 private:
  // void DrawMousePonter(const Rect& screen_rect, void* screenshot);

  Screens screens_;
};

}  // namespace rdv

#endif  // SRC_SCREEN_CAPTURE_SCREEN_CAPTURE_H_
