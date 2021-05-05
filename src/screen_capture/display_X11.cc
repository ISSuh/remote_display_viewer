/**
 *
 *  Copyright:  Copyright (c) 2020, ISSuh
 *
 */

#include <iostream>
#include <memory>
#include <utility>

#include <X11/extensions/Xinerama.h>

#include "screen_capture/display_X11.h"

namespace rdv {

Screen::Screen() : display_handle_(nullptr), id_(0), rect_(Rect()) {}

Screen::Screen(DisplayHandle* display_handle, uint32_t id, Rect rect)
    : display_handle_(display_handle), id_(id), rect_(rect) {}

Screen::~Screen() {
  display_handle_ = nullptr;
}

X11Display::X11Display() {}

X11Display::~X11Display() {}

const X11Display::ScreenMap& X11Display::GetScreenMap() {
  if (!UpdateScreen()) {
    return std::move(X11Display::ScreenMap());
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

  // int32_t all_screen_width = XDisplayWidth(dsp, screen);
  // int32_t all_screen_height = 0;
  // int32_t all_screen_x = 0;
  // int32_t all_screen_y = 0;

  if (count) {
    for (int id = 0; id < count; ++id) {
      Rect rect(info[id].width, info[id].height, info[id].x_org,
                info[id].y_org);
      screen_map_.insert({id, Screen(&display_handle_, id, rect)});
    }
  } else {
    std::cout << "ERROR : XineramaQueryScreens says there aren't any\n";
    return false;
  }

  XFree(info);
  return true;
}
}  // namespace rdv
