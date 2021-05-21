 /**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#include <iostream>
#include "../../screen_capture/screen_capture_bridge.h"

int main(int argc, char *argv[]) {
  std::cout << "Remote Display View\n";

  void* rdv_handle = create_rdv_hadle();
  int count = screen_count(rdv_handle);

  ScreenInfo* remote_screens = new ScreenInfo[count];
  screen_infomations(rdv_handle, remote_screens);

  for (int i = 0 ; i < count ; ++i) {
    std::cout << remote_screens[i].id << " : "
              << remote_screens[i].width << "x"
              << remote_screens[i].height << "/"
              << remote_screens[i].x << ","
              << remote_screens[i].y << "\n";
  }

  ScreenImage image;
  create_screen_image(rdv_handle, 0, &image);

  capture(rdv_handle, 0, &image);
  std::cout << std::endl << image.buffer << std::endl;

  delete remote_screens;
  destroy_screen_image(&image);
  destroy_rdv_hadle(rdv_handle);
}
