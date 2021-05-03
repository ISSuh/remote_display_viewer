/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRC_SCREEN_CAPTURE_SCREEN_CAPTURE_X11_H_
#define SRC_SCREEN_CAPTURE_SCREEN_CAPTURE_X11_H_

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/shm.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XShm.h>

#include <cmath>

namespace rdv {

struct ShmImage {
  XShmSegmentInfo shminfo;
  XImage* ximage;
  unsigned int* data;  // will point to the image's BGRA packed pixels
};

class ScreenCaptureX11 {
 public:
  ScreenCaptureX11() {}
  ~ScreenCaptureX11() {}

  
};

}  // namespace rdv

#endif  // SRC_SCREEN_CAPTURE_SCREEN_CAPTURE_X11_H_
