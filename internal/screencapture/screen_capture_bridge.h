/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRC_SCREEN_CAPTURE_SCREEN_CAPTURE_BRIDGE_H_
#define SRC_SCREEN_CAPTURE_SCREEN_CAPTURE_BRIDGE_H_

#pragma once
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  int id;
  int width;
  int height;
  int x;
  int y;
} Screen;

void* create_rdv_hadle();
void destroy_rdv_hadle(void* handle);

int screen_count(void* handle);
void screen_infomations(void* handle, Screen* remote_screen);

void capture(void* handle, int screen_id, void* image);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // SRC_SCREEN_CAPTURE_SCREEN_CAPTURE_BRIDGE_H_
