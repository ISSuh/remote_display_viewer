/**
 *
 *  Copyright:  Copyright (c) 2020, ISSuh
 *
 */

#include <iostream>
#include <X11/extensions/Xinerama.h>

#include "display_X11.h"

namespace rdv {

Screen::Screen()
    : display_handle_(nullptr), id_(0), size_(Size(0, 0)), point_(Point(0, 0)) {}

Screen::Screen(DisplayHandle* display_handle, uint32_t id, Size size, Point point)
    : display_handle_(display_handle), id_(id), size_(size), point_(point) {}

Screen::~Screen() {
  display_handle_ = nullptr;
}

X11Display::X11Display() {}

X11Display::~X11Display() {}

bool X11Display::UpdateScreen() {
  if (!display_handle_.Get()) {
    return false;
  }

  int dummy_1 = 0;
  int dummy_2 = 0;

  if (!XineramaQueryExtension(display_handle_.Get(), &dummy_1, &dummy_2)) {
    std::cout << "ERROR : No Xinerama extension\n";
    return false;
  }

  if (!XineramaIsActive(display_handle_.Get())) {
    std::cout << "ERROR : Xinerama not active\n";
    return false;
  }

  screen_map_.clear();

  int count = 0;
  XineramaScreenInfo* info = XineramaQueryScreens(display_handle_.Get(), &count);
  if (count) {
    for (int id = 0; id < count; ++id) {
      std::cout << "[" << id << "] : " <<
          info[id].width << "x" << info[id].height << " at " <<
          info[id].x_org << "," << info[id].y_org << std::endl;

      Size size(info[id].width, info[id].height);
      Point point(info[id].x_org, info[id].y_org);
      screen_map_.insert({id, Screen(&display_handle_, id, size, point)});
    }
  } else {
    std::cout << "ERROR : XineramaQueryScreens says there aren't any\n";
    return false;
  }

  XFree(p);
  return true;
}
}  // namespace rdv
