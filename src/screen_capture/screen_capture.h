/**
 *
 *  Copyright:  Copyright (c) 2020, ISSuh
 *
 */

#ifndef SRC_SCREEN_CAPTURE_SCREEN_CAPTURE_H_
#define SRC_SCREEN_CAPTURE_SCREEN_CAPTURE_H_

#include <X11/Xlib.h>
#include <X11/extensions/XShm.h>

#include <atomic>
#include <chrono>

#include "base/geometry.h"
#include "screen_capture/display.h"

namespace rdv {

class ScreemImage {
 public:
  const uint8_t PLANE = 4;

  ScreemImage();
  ~ScreemImage();

  bool CreateImage(DisplayHandle* display_hanle, Size size);
  void DestroyImage(DisplayHandle* display_hanle);

  XImage* GetImage() { return ximage_; }
  int32_t GetImageSize() {
    return ximage_->width * ximage_->width;
  }

 private:
  bool CreateShm(Size size);

  XImage* ximage_;
  XShmSegmentInfo shm_info_;
  Size size_;
};

class ScreenCapture {
 public:
  ScreenCapture();
  ~ScreenCapture();

  X11Display::ScreenMap GetScreenMap() { return display_.GetScreenMap(); }
  bool SetScreen(int32_t screen_id);
  int32_t GetScreenWidth(int32_t screen_id);
  int32_t GetScreenHeight(int32_t screen_id);

  void Capture() { Capture(Point(0, 0)); }
  void Capture(Point pos);

  // void Run();

  ScreemImage image_;
  X11Display display_;
};

}  // namespace rdv

#endif  // SRC_SCREEN_CAPTURE_SCREEN_CAPTURE_H_
