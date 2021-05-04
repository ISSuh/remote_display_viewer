/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#include <iostream>

// #include "base/debug_window.h"
#include "screen_capture/display_X11.h"

int main(int argc, char *argv[]) {
  std::cout << "Remote Display View\n";
  rdv::X11Display display;

  display.UpdateScreen();
  auto infoes = display.GetScreenMap();

  for (const auto& info : infoes) {
    std::cout << info.first << " : "
              << info.second.GetRect().ToString()
              << std::endl;
  }
}
