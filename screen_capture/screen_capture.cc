/**
 *
 *  Copyright:  Copyright (c) 2020, ISSuh
 *
 */

#include "screen_capture.h"
#include <X11/Xutil.h>
#include <sys/shm.h>
#include <cmath>
#include <thread>
#include <algorithm>

namespace rdv {

ScreemImage::ScreemImage() : ximage_(nullptr), rect_(Rect()) {
  shm_info_.shmaddr = nullptr;
}

ScreemImage::~ScreemImage() {
  ximage_ = nullptr;
  shm_info_.shmaddr = nullptr;
}

bool ScreemImage::CreateImage(DisplayHandle* display_hanle, Rect rect) {
  if (rect.IsEmpty()) {
    return false;
  }

  rect_ = rect;
  if (!CreateShm()) {
    return false;
  }

  auto* display = display_hanle->get();
  ximage_ =
      XShmCreateImage(display, XDefaultVisual(display, XDefaultScreen(display)),
                      DefaultDepth(display, XDefaultScreen(display)), ZPixmap,
                      0, &shm_info_, 0, 0);

  if (!ximage_) {
    DestroyImage(display_hanle);
    return false;
  }

  ximage_->data = shm_info_.shmaddr;
  ximage_->width = rect_.width();
  ximage_->height = rect_.height();

  XShmAttach(display, &shm_info_);
  XSync(display, false);

  return true;
}

void ScreemImage::DestroyImage(DisplayHandle* display_hanle) {
  if (ximage_) {
    XShmDetach(display_hanle->get(), &shm_info_);
    XDestroyImage(ximage_);
  }

  if (shm_info_.shmaddr) {
    shmdt(shm_info_.shmaddr);
  }
}

ScreenCapture::ScreenCapture() {}

ScreenCapture::~ScreenCapture() {
  image_.DestroyImage(display_.GetDisplayHandle());
}

bool ScreemImage::CreateShm() {
  shm_info_.readOnly = false;
  shm_info_.shmid = shmget(IPC_PRIVATE, GetImageBufferSize(),
                           IPC_CREAT | 0600);
  if (shm_info_.shmid == -1) {
    return false;
  }

  shm_info_.shmaddr = reinterpret_cast<char*>(shmat(shm_info_.shmid, 0, 0));
  if (shm_info_.shmaddr == reinterpret_cast<char*>(-1)) {
    return false;
  }

  shmctl(shm_info_.shmid, IPC_RMID, 0);
  return true;
}


ScreenInfo ScreenCapture::GetScreenInfomationes(int32_t screen_id) {
  auto screen_map = display_.GetScreenMap();
  if (screen_map.find(screen_id) == screen_map.end()) {
    return ScreenInfo();
  }
  return screen_map[screen_id];
}

bool ScreenCapture::SetScreen(int32_t screen_id) {
  auto screen_map = display_.GetScreenMap();
  if (screen_map.find(screen_id) == screen_map.end()) {
    return false;
  }

  auto& screen_info = screen_map[screen_id];

  if (image_.GetXImage()) {
    image_.DestroyImage(screen_info.GetDisplayHanel());
  }

  image_.CreateImage(screen_info.GetDisplayHanel(), screen_info.GetRect());

  return true;
}

void ScreenCapture::Capture(char* image_data) {
  if (!image_data) {
    return;
  }

  auto* display = display_.GetDisplayHandle()->get();
  auto* image = image_.GetXImage();
  Rect rect = image_.GetRect();
  int32_t len = image_.GetImageBufferSize();

  XShmGetImage(display, XDefaultRootWindow(display), image, rect.x(), rect.y(),
               AllPlanes);

  std::copy(image->data, image->data + len, image_data);
}

}  // namespace rdv
