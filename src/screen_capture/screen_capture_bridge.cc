/**
 *
 *  Copyright:  Copyright (c) 2020, ISSuh
 *
 */

#include "screen_capture/screen_capture_bridge.h"
#include "screen_capture/screen_capture.h"

void* create_rdv_hadle() {
  return new rdv::ScreenCapture();
}

void destroy_rdv_hadle(void* handle) {
  if (!handle) {
    return;
  }
  delete handle;
}

int screen_count(void* handle) {
  if (!handle) {
    return -1;
  }

  rdv::ScreenCapture* screen_capture = static_cast<rdv::ScreenCapture*>(handle);
  rdv::X11Display::ScreenMap screen_map = screen_capture->GetScreenMap();
  return screen_map.size();
}

void screen_infomations(void* handle, RemoteScreen* remote_screen) {
  if (!handle || !remote_screen) {
    return;
  }

  rdv::ScreenCapture* screen_capture = static_cast<rdv::ScreenCapture*>(handle);
  rdv::X11Display::ScreenMap screen_map = screen_capture->GetScreenMap();

  int index = 0;
  for (const auto& screen : screen_map) {
    rdv::ScreenInfo info = screen.second;

    remote_screen[index].id = screen.first;
    remote_screen[index].width = info.GetRect().width();
    remote_screen[index].height = info.GetRect().height();
    remote_screen[index].x = info.GetRect().x();
    remote_screen[index].y = info.GetRect().y();

    ++index;
  }
}


void create_screen_image(void* handle, int screen_id, RemoteScreenImage* image) {
  if (!handle || !image) {
    return;
  }

  rdv::ScreenCapture* screen_capture = static_cast<rdv::ScreenCapture*>(handle);
  screen_capture->SetScreen(screen_id);

  const rdv::ScreemImage* screen_image = screen_capture->GetScreemImage();
  image->width = screen_image->GetRect().width();
  image->height = screen_image->GetRect().height();
  image->plane = screen_image->GetImagePlane();
  image->buffer = new char[screen_image->GetImageBufferSize()];
}

void destroy_screen_image(RemoteScreenImage* image) {
  if (!image) {
    return;
  }
  delete image->buffer;
}

void capture(void* handle, RemoteScreenImage* image) {
  if (!handle || !image) {
    return;
  }

  rdv::ScreenCapture* screen_capture = static_cast<rdv::ScreenCapture*>(handle);
  screen_capture->Capture(image->buffer);
}
