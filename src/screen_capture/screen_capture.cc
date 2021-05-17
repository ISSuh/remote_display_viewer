/**
 *
 *  Copyright:  Copyright (c) 2020, ISSuh
 *
 */

#include "screen_capture/screen_capture.h"
#include <X11/Xutil.h>
#include <sys/shm.h>
#include <cmath>
#include <thread>

#include "base/time.h"

namespace rdv {

ScreemImage::ScreemImage() : ximage_(nullptr), size_(Size()) {
  shm_info_.shmaddr = nullptr;
}

ScreemImage::~ScreemImage() {
  ximage_ = nullptr;
  shm_info_.shmaddr = nullptr;
}

bool ScreemImage::CreateImage(DisplayHandle* display_hanle, Size size) {
  auto* display = display_hanle->get();
  size_.SetSize(size.width(), size.height());

  if (!CreateShm(size)) {
    return false;
  }

  ximage_ =
      XShmCreateImage(display, XDefaultVisual(display, XDefaultScreen(display)),
                      DefaultDepth(display, XDefaultScreen(display)), ZPixmap,
                      0, &shm_info_, 0, 0);

  if (!ximage_) {
    DestroyImage(display_hanle);
    return false;
  }

  ximage_->data = shm_info_.shmaddr;
  ximage_->width = size_.width();
  ximage_->height = size_.height();

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

bool ScreemImage::CreateShm(Size size) {
  shm_info_.readOnly = false;
  shm_info_.shmid = shmget(IPC_PRIVATE, size.width() * size.height() * PLANE,
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

bool ScreenCapture::SetScreen(int32_t screen_id) {
  auto screen_map = display_.GetScreenMap();
  if (screen_map.find(screen_id) == screen_map.end()) {
    return false;
  }

  auto* screen = &screen_map[screen_id];

  if (image_.GetImage()) {
    image_.DestroyImage(screen->GetDisplayHanel());
  }

  image_.CreateImage(screen->GetDisplayHanel(), screen->GetSize());

  return true;
}

// void ScreenCapture::Run() {
//   auto* display = display_.GetDisplayHandle()->get();
//   auto frame_time_tick = std::chrono::steady_clock::now();
//   auto frame_time = std::time(0);
//   auto last_frame_time = std::time(0);

//   while (running_.load()) {
//     Capture(screen_->GetPoint());

//     frame_time_tick += std::chrono::milliseconds(ScreenCapture::FPS_30);
//     std::this_thread::sleep_until(frame_time_tick);

//     frame_time = std::time(0);

//     if (frame_time != last_frame_time) {
//       std::cout << "FPS : " << fps_.load() << " / " << std::time(0) << '\n';
//       fps_.store(0);
//     }

//     fps_.fetch_add(1);
//     last_frame_time = frame_time;
//   }
// }

void ScreenCapture::Capture(Point pos) {
  auto* display = display_.GetDisplayHandle()->get();
  auto* image = image_.GetImage();
  XShmGetImage(display, XDefaultRootWindow(display), image, pos.x(), pos.y(),
               AllPlanes);
}

}  // namespace rdv
