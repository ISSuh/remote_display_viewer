/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#include <iostream>

#include "screen_capture/screen_capture_X11.h"

int main(int argc, char *argv[]) {
  std::cout << "Remote Display View\n";

  rdv::ScreenCaptureX11 screen_capture;
  auto* display = screen_capture.GetDisplay();
  auto infoes = display->GetScreenMap();

  for (const auto& info : infoes) {
    std::cout << info.first << " : "
              << info.second.GetRect().ToString()
              << std::endl;
  }

  screen_capture.Initialize();
  screen_capture.Run();
}
