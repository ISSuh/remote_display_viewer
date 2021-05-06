/**
 *
 *  Copyright:  Copyright (c) 2020, ISSuh
 *
 */

#ifndef SRC_SCREEN_CAPTURE_SCREEN_CAPTURE_X11_H_
#define SRC_SCREEN_CAPTURE_SCREEN_CAPTURE_X11_H_

#include <X11/Xlib.h>
#include <X11/extensions/XShm.h>

#include <atomic>
#include <chrono>

#include "base/geometry.h"
#include "screen_capture/display_X11.h"
#include "screen_capture/debug_window_X11.h"

namespace rdv {

class ScreemImageX11 {
 public:
  const uint8_t PLANE = 4;

  ScreemImageX11();
  ~ScreemImageX11();

  bool CreateImage(DisplayHandle* display_hanle, Size size);
  void DestroyImage(DisplayHandle* display_hanle);

  XImage* GetImage() { return ximage_; }
  Size GetSize() const { return size_; }

 private:
  bool CreateShm(Size size);

  XImage* ximage_;
  XShmSegmentInfo shm_info_;
  Size size_;
};

class ScreenCaptureX11 {
 public:
  const uint32_t FPS_60 = 1000 / 60;
  const uint32_t FPS_30 = 1000 / 30;

  ScreenCaptureX11();
  ~ScreenCaptureX11();

  X11Display* GetDisplay() { return &display_; }

  bool SetScreen(int32_t screen_id);

  void Run();
  void changeRunningState(bool state) { running_.store(state); }
  bool IsRunning() { return running_.load(); }

 private:
  void Capture() { Capture(Point(0, 0)); }
  void Capture(Point pos);

  ScreemImageX11 image_;
  X11Display display_;
  Screen* screen_;

  std::atomic<bool> running_;
  std::atomic<uint32_t> fps_;

#if defined(DEBUG_X11_WINDOW)
  DebugWindow window_;
  Size window_size_;
#endif
};

}  // namespace rdv

#endif  // SRC_SCREEN_CAPTURE_SCREEN_CAPTURE_X11_H_
