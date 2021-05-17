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
} RemoteScreen;

typedef struct {
  char* buffer;
  int width;
  int height;
  int plane;
} RemoteScreenImage;


void* create_rdv_hadle();
void destroy_rdv_hadle(void* handle);

int screen_count(void* handle);
void screen_infomations(void* handle, RemoteScreen* remote_screen);

void create_screen_image(void* handle, int screen_id, RemoteScreenImage* image);
void destroy_screen_image(RemoteScreenImage* image);

void capture(void* handle, RemoteScreenImage* image);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // SRC_SCREEN_CAPTURE_SCREEN_CAPTURE_BRIDGE_H_
