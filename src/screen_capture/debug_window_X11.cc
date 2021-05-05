/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#include "screen_capture/debug_window_X11.h"

namespace rdv {

DebugWindow::DebugWindow()
  : width_(0), height_(0) {}

DebugWindow::~DebugWindow() {}

void DebugWindow::CreateWindow(DisplayHandle* display_handle, Size size) {
  uint64_t mask = CWBackingStore;
  XSetWindowAttributes attributes;

  attributes.backing_store = NotUseful;
  mask |= CWBackingStore;

  auto* dsp = display_handle->get();
  auto& window = window_.get();

  window = XCreateWindow(dsp, DefaultRootWindow(dsp),
                                0, 0, size.width(), size.height(), 0,
                                DefaultDepth(dsp, XDefaultScreen(dsp)),
                                InputOutput, CopyFromParent, mask, &attributes);

  XStoreName(dsp, window, "DbugWindowX11");
  XSelectInput(dsp, window, StructureNotifyMask);
  XMapWindow(dsp, window);
}

void DebugWindow::DestroyWindow(DisplayHandle* display_handle) {
  XDestroyWindow(display_handle->get(), window_.get());
}

}  // namespace rdv
