/**
 *
 *  Copyright:  Copyright (c) 2020, ISSuh
 *
 */

#include "screen_capture/screen_capture_X11.h"
#include <X11/Xutil.h>
#include <sys/shm.h>
#include <cmath>
#include <thread>

#include "base/time.h"

namespace rdv {

ScreemImageX11::ScreemImageX11() : ximage_(nullptr), size_(Size()) {
  shm_info_.shmaddr = nullptr;
}

ScreemImageX11::~ScreemImageX11() {
  ximage_ = nullptr;
  shm_info_.shmaddr = nullptr;
}

bool ScreemImageX11::CreateImage(DisplayHandle* display_hanle, Size size) {
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

void ScreemImageX11::DestroyImage(DisplayHandle* display_hanle) {
  if (ximage_) {
    XShmDetach(display_hanle->get(), &shm_info_);
    XDestroyImage(ximage_);
  }

  if (shm_info_.shmaddr) {
    shmdt(shm_info_.shmaddr);
  }
}

ScreenCaptureX11::ScreenCaptureX11()
    : running_(true), fps_(0), screen_(nullptr) {
#if defined(DEBUG_X11_WINDOW)
  window_size_.SetSize(1920, 1080);
  window_.CreateWindow(display_.GetDisplayHandle(), window_size_);
#endif
}

ScreenCaptureX11::~ScreenCaptureX11() {
  image_.DestroyImage(display_.GetDisplayHandle());
  screen_ = nullptr;
}

bool ScreemImageX11::CreateShm(Size size) {
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

bool ScreenCaptureX11::SetScreen(int32_t screen_id) {
  auto screen_map = display_.GetScreenMap();
  if (screen_map.find(screen_id) == screen_map.end()) {
    return false;
  }

  screen_ = &screen_map[screen_id];

  if (image_.GetImage()) {
    image_.DestroyImage(screen_->GetDisplayHanel());
  }

  image_.CreateImage(screen_->GetDisplayHanel(), screen_->GetSize());

  return true;
}

void ScreenCaptureX11::Run() {
  auto* display = display_.GetDisplayHandle()->get();

#if defined(DEBUG_X11_WINDOW)
  ScreemImageX11 dst;
  auto window_handle = window_.GetWindowHandle().get();

  dst.CreateImage(display_.GetDisplayHandle(), window_size_);

  XGCValues xgcvalues;
  xgcvalues.graphics_exposures = false;

  GC gc = XCreateGC(display, window_handle, GCGraphicsExposures, &xgcvalues);
  Atom delete_atom = XInternAtom(display, "WM_DELETE_WINDOW", false);

  XSetWMProtocols(display, window_handle, &delete_atom, true);
#endif

  auto frame_time_tick = std::chrono::steady_clock::now();
  auto frame_time = std::time(0);
  auto last_frame_time = std::time(0);

  while (running_.load()) {
    Capture(screen_->GetPoint());
#if defined(DEBUG_X11_WINDOW)
    XShmPutImage(display, window_handle, gc, image_.GetImage(), 0, 0, 0, 0,
                 screen_->GetSize().width(), screen_->GetSize().height(), false);
    XSync(display, false);
#endif

    frame_time_tick += std::chrono::milliseconds(ScreenCaptureX11::FPS_30);
    std::this_thread::sleep_until(frame_time_tick);

    frame_time = std::time(0);

    if (frame_time != last_frame_time) {
      std::cout << "FPS : " << fps_.load() << " / " << std::time(0) << '\n';
      fps_.store(0);
    }

    fps_.fetch_add(1);
    last_frame_time = frame_time;
  }

#if defined(DEBUG_X11_WINDOW)
  dst.DestroyImage(display_.GetDisplayHandle());
#endif
}

void ScreenCaptureX11::Capture(Point pos) {
  auto* display = display_.GetDisplayHandle()->get();
  auto* image = image_.GetImage();
  XShmGetImage(display, XDefaultRootWindow(display), image, pos.x(), pos.y(),
               AllPlanes);
}

}  // namespace rdv
