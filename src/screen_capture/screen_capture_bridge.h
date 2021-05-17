/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRC_SCREEN_CAPTURE_SCREEN_CAPTURE_BRIDGE_H_
#define SRC_SCREEN_CAPTURE_SCREEN_CAPTURE_BRIDGE_H_

#include "screen_capture/screen_capture.h"

#pragma once
#ifdef __cplusplus
extern "C" {
#endif

void* Create_ScreenCapture_hadle();
void Destroy_ScreenCapture_hadle(void* handle);

int 
int Screen_Capture(void* handle);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // SRC_SCREEN_CAPTURE_SCREEN_CAPTURE_BRIDGE_H_
