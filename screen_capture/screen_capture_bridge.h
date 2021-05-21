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
} ScreenInfo;

typedef struct {
  unsigned char* buffer;
  int width;
  int height;
  int plane;
} ScreenImage;

void* create_rdv_hadle();
void destroy_rdv_hadle(void* handle);

int screen_count(void* handle);
void screen_infomations(void* handle, ScreenInfo* remote_screen);

void create_screen_image(void* handle, int screen_id, ScreenImage* image);
void destroy_screen_image(ScreenImage* image);

void capture(void* handle, int screen_id, ScreenImage* image);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // SRC_SCREEN_CAPTURE_SCREEN_CAPTURE_BRIDGE_H_
