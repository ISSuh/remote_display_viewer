/**
 *
 *  Copyright:  Copyright (c) 2020, ISSuh
 *
 */

#include <iostream>
#include <memory>
#include <utility>

#include <X11/extensions/Xinerama.h>

#include "screen_capture/display.h"

namespace rdv {
ScreenInfo::ScreenInfo() : display_handle_(nullptr), id_(-1), rect_(Rect()) {}

ScreenInfo::ScreenInfo(DisplayHandle* display_handle, uint32_t id, Rect rect)
    : display_handle_(display_handle), id_(id), rect_(rect) {}

ScreenInfo::~ScreenInfo() {
  display_handle_ = nullptr;
}

X11Display::X11Display() {}

X11Display::~X11Display() {}

X11Display::ScreenMap X11Display::GetScreenMap() {
  if (!UpdateScreen()) {
    return X11Display::ScreenMap();
  }

  return screen_map_;
}

bool X11Display::UpdateScreen() {
  if (!display_handle_.get()) {
    return false;
  }

  int dummy_1 = 0;
  int dummy_2 = 0;

  if (!XineramaQueryExtension(display_handle_.get(), &dummy_1, &dummy_2)) {
    std::cout << "ERROR : No Xinerama extension\n";
    return false;
  }

  if (!XineramaIsActive(display_handle_.get())) {
    std::cout << "ERROR : Xinerama not active\n";
    return false;
  }

  screen_map_.clear();

  int count = 0;
  XineramaScreenInfo* info =
      XineramaQueryScreens(display_handle_.get(), &count);

  if (count) {
    for (int id = 0; id < count; ++id) {
      Rect rect(info[id].width, info[id].height, info[id].x_org,
                info[id].y_org);
      screen_map_.insert({id, ScreenInfo(&display_handle_, id, rect)});
    }
  } else {
    std::cout << "ERROR : XineramaQueryScreens says there aren't any\n";
    return false;
  }

  XFree(info);
  return true;
}
}  // namespace rdv
