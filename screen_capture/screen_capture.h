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

#include "geometry.h"
#include "display.h"

namespace rdv {

class ScreemImage {
 public:
  ScreemImage();
  ~ScreemImage();

  bool CreateImage(DisplayHandle* display_hanle, Rect rect);
  void DestroyImage(DisplayHandle* display_hanle);

  XImage* GetXImage() { return ximage_; }

  char* GetImageBuffer() { return ximage_->data; }
  int32_t GetImageBufferSize() const {
    return rect_.width() * rect_.height() * image_plane;
  }

  const Rect& GetRect() const { return rect_; }
  int32_t GetImagePlane() const { return image_plane; }

 private:
  bool CreateShm();

  XImage* ximage_;
  XShmSegmentInfo shm_info_;
  Rect rect_;
  const uint8_t image_plane = 4;
};

class ScreenCapture {
 public:
  ScreenCapture();
  ~ScreenCapture();

  X11Display::ScreenMap GetScreenMap() { return display_.GetScreenMap(); }
  ScreenInfo GetScreenInfomationes(int32_t screen_id);

  const ScreemImage* GetScreemImage() const { return &image_; }

  bool SetScreen(int32_t screen_id);
  void Capture(char* image_data);

 private:
  ScreemImage image_;
  X11Display display_;
};

}  // namespace rdv

#endif  // SRC_SCREEN_CAPTURE_SCREEN_CAPTURE_H_
