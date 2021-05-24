/**
 *
 *  Copyright:  Copyright (c) 2020, ISSuh
 *
 */

#include "screen_capture_bridge.h"
#include "screen_capture.h"

void* create_rdv_hadle() {
  return new rdv::ScreenCapture();
}

void destroy_rdv_hadle(void* handle) {
  if (!handle) {
    return;
  }
  delete static_cast<rdv::ScreenCapture*>(handle);
}

int screen_count(void* handle) {
  if (!handle) {
    return -1;
  }

  rdv::ScreenCapture* screen_capture = static_cast<rdv::ScreenCapture*>(handle);
  return screen_capture->NumberOfScreen();
}

void screen_infomations(void* handle, Screen* remote_screen) {
  if (!handle || !remote_screen) {
    return;
  }

  rdv::ScreenCapture* screen_capture = static_cast<rdv::ScreenCapture*>(handle);

  uint8_t screen_count = screen_capture->NumberOfScreen();
  for (uint8_t i = 0 ; i < screen_count ; ++i) {
    rdv::Rect rect = screen_capture->ScreenRect(i);

    remote_screen[i].id = i;
    remote_screen[i].width = rect.width();
    remote_screen[i].height = rect.height();
    remote_screen[i].x = rect.x();
    remote_screen[i].y = rect.y();
  }
}

void capture(void* handle, int screen_id, void* image) {
  if (!handle || !image) {
    return;
  }

  rdv::ScreenCapture* screen_capture = static_cast<rdv::ScreenCapture*>(handle);
  screen_capture->Capture(screen_id, static_cast<uint8_t*>(image));
}
