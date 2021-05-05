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

ScreemImageX11::ScreemImageX11() : ximage_(nullptr) {
  shm_info_.shmaddr = nullptr;
}

ScreemImageX11::~ScreemImageX11() {
  ximage_ = nullptr;
  shm_info_.shmaddr = nullptr;
}

bool ScreemImageX11::CreateImage(DisplayHandle* display_hanle, Size size) {
  auto* display = display_hanle->get();

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
  ximage_->width = size.width();
  ximage_->height = size.height();

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

ScreenCaptureX11::ScreenCaptureX11() : running_(true), fps_(0) {
#if defined(DEBUG_X11_WINDOW)
  window_size_.SetSize(600, 300);
  window_.CreateWindow(display_.GetDisplayHandle(), window_size_);
#endif
}

ScreenCaptureX11::~ScreenCaptureX11() {
  image_.DestroyImage(display_.GetDisplayHandle());
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

void ScreenCaptureX11::Initialize() {
  image_.CreateImage(display_.GetDisplayHandle(), Size(1920, 720));
}

void ScreenCaptureX11::Run() {
  auto* display = display_.GetDisplayHandle()->get();

#if defined(DEBUG_X11_WINDOW)
  ScreemImageX11 dst;
  bool init = false;
  auto window_handle = window_.GetWindowHandle().get();

  dst.CreateImage(display_.GetDisplayHandle(), window_size_);

  XGCValues xgcvalues;
  xgcvalues.graphics_exposures = false;

  GC gc = XCreateGC(display, window_handle, GCGraphicsExposures, &xgcvalues);
  Atom delete_atom = XInternAtom(display, "WM_DELETE_WINDOW", false);

  XSetWMProtocols(display, window_handle, &delete_atom, true);
  XEvent xevent;
#endif

  auto frame_time_tick = std::chrono::steady_clock::now();
  auto frame_time = std::time(0);
  auto last_frame_time = std::time(0);

  while (running_.load()) {
#if defined(DEBUG_X11_WINDOW)
    while (XPending(display)) {
      XNextEvent(display, &xevent);

      if ((xevent.type == ClientMessage &&
           xevent.xclient.data.l[0] == delete_atom) ||
          xevent.type == DestroyNotify) {
        running_.store(false);
        break;
      } else if (xevent.type == ConfigureNotify) {
        if (xevent.xconfigure.width == window_size_.width() &&
            xevent.xconfigure.height == window_size_.height()) {
          init = true;
        }
      }
    }
#endif

#if defined(DEBUG_X11_WINDOW)
    if (init) {
#endif
      Capture();
#if defined(DEBUG_X11_WINDOW)
      if (!ProcessImage(&image_, &dst)) {
        continue;
      }

      XShmPutImage(display, window_handle, gc, dst.GetImage(), 0, 0, 0, 0,
                   window_size_.width(), window_size_.height(), false);
      XSync(display, false);
#endif

      frame_time_tick += std::chrono::milliseconds(ScreenCaptureX11::FPS_60);
      std::this_thread::sleep_until(frame_time_tick);

      frame_time = std::time(0);

      if (frame_time != last_frame_time) {
        std::cout << "FPS : " << fps_.load() << " / " << std::time(0) << '\n';
        fps_.store(0);
      }

      fps_.fetch_add(1);
      last_frame_time = frame_time;
#if defined(DEBUG_X11_WINDOW)
    }
#endif
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

bool ScreenCaptureX11::ProcessImage(ScreemImageX11* src, ScreemImageX11* dst) {
  int sw = src->GetImage()->width;
  int sh = src->GetImage()->height;
  int dw = dst->GetImage()->width;
  int dh = dst->GetImage()->height;

  // Here you can set the resulting position and size of the captured screen
  // Because of the limitations of this example, it must fit in dst->ximage
  int w = dw / 2;
  int h = dh / 2;
  int x = (dw - w);
  int y = (dh - h) / 2;

  // Just in case...
  if (x < 0 || y < 0 || x + w > dw || y + h > dh || sw < dw || sh < dh) {
    // printf(NAME ": This is only a limited example\n");
    // printf(NAMESP "  Please implement a complete scaling algorithm\n");
    return false;
  }

  uint8_t* d = reinterpret_cast<uint8_t*>(dst->GetImage()->data + y * dw + x);
  int r = dw - w;
  int j, i;

  for (j = 0; j < h; ++j) {
    for (i = 0; i < w; ++i) {
      *d++ = getpixel(src, j, i, w, h);
    }
    d += r;
  }

  return true;
}

uint8_t ScreenCaptureX11::getpixel(ScreemImageX11* image,
                                   int j,
                                   int i,
                                   int w,
                                   int h) {
  int x = (float)(i * image->GetImage()->width) / (float)w;
  int y = (float)(j * image->GetImage()->height) / (float)h;
  return image->GetImage()->data[y * image->GetImage()->width + x];
}

}  // namespace rdv
