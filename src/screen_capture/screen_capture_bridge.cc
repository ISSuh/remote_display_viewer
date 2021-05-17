/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#include "screen_capture/screen_capture_bridge.h"

void* Create_ScreenCapture_hadle() {
  new rdv::ScreenCapture();
}

void Destroy_ScreenCapture_hadle(void* handle) {
  delete handle;
}
