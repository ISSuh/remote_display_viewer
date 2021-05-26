/**
 *
 *  Copyright:  Copyright (c) 2020, ISSuh
 *
 */

#include "screen_capture.h"

#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include <algorithm>

namespace rdv {

Screens::Screens() : screen_handle_(nullptr) {
  if (!gtk_init_check(nullptr, nullptr)) {
    gtk_init(nullptr, nullptr);
  }

  screen_handle_ = gdk_screen_get_default();
}

Screens::~Screens() {
  screen_handle_ = nullptr;
}

void* Screens::GetRootWindowHandle() {
  if (!screen_handle_) {
    return nullptr;
  }

  GdkScreen* screen = static_cast<GdkScreen*>(screen_handle_);
  return gdk_screen_get_root_window(screen);
}

uint8_t Screens::NumberOfScreen() {
  if (!screen_handle_) {
    return 0;
  }

  GdkScreen* screen = static_cast<GdkScreen*>(screen_handle_);
  return gdk_screen_get_n_monitors(screen);
}

Rect Screens::ScreenRect(uint8_t screen_id) {
  if (screen_id >= NumberOfScreen()) {
    return Rect();
  }

  GdkScreen* screen = static_cast<GdkScreen*>(screen_handle_);
  GdkRectangle gdk_rect;

  gdk_screen_get_monitor_geometry(screen, screen_id, &gdk_rect);
  return Rect(gdk_rect.width, gdk_rect.height, gdk_rect.x, gdk_rect.y);
}

ScreenCapture::ScreenCapture() {}

ScreenCapture::~ScreenCapture() {}

void ScreenCapture::Capture(uint8_t screen_id, uint8_t* image) {
  GdkWindow* root = static_cast<GdkWindow*>(screens_.GetRootWindowHandle());
  Rect screen_rect = ScreenRect(screen_id);

  GdkPixbuf* screenshot =
      gdk_pixbuf_get_from_window(root, screen_rect.x(), screen_rect.y(),
                                 screen_rect.width(), screen_rect.height());
  if (!screenshot) {
    image = nullptr;
    return;
  }

  DrawMousePonter(screen_rect, screenshot);

  const uint8_t* screenshot_buf = gdk_pixbuf_read_pixels(screenshot);
  int32_t buf_size = gdk_pixbuf_get_byte_length(screenshot);
  std::copy(screenshot_buf, screenshot_buf + buf_size, image);
}

void ScreenCapture::DrawMousePonter(const Rect& screen_rect, void* screenshot) {
  GdkPixbuf* screen_image = static_cast<GdkPixbuf*>(screenshot);
  GdkScreen* screen = static_cast<GdkScreen*>(screens_.handle());
  GdkDisplay* display = gdk_screen_get_display(screen);

  g_autoptr(GdkCursor) cursor =
      gdk_cursor_new_for_display(display, GDK_LEFT_PTR);
  g_autoptr(GdkPixbuf) cursor_pixbuf = gdk_cursor_get_image(cursor);

  if (cursor_pixbuf != nullptr) {
    GdkDeviceManager* device_manager = gdk_display_get_device_manager(display);
    GdkDevice* device = gdk_device_manager_get_client_pointer(device_manager);
    Rect cursor_rect;
    gint cx, cy, xhot, yhot;

    gdk_device_get_position(device, &screen, &cx, &cy);

    sscanf(gdk_pixbuf_get_option(cursor_pixbuf, "x_hot"), "%d", &xhot);
    sscanf(gdk_pixbuf_get_option(cursor_pixbuf, "y_hot"), "%d", &yhot);

    GdkRectangle gdk_screen_rect;
    gdk_screen_rect.x = screen_rect.x();
    gdk_screen_rect.y = screen_rect.y();
    gdk_screen_rect.width = screen_rect.width();
    gdk_screen_rect.height = screen_rect.height();

    GdkRectangle gdk_cursor_rect;
    gdk_cursor_rect.x = cx + screen_rect.x();
    gdk_cursor_rect.y = cy + screen_rect.y();
    gdk_cursor_rect.width = gdk_pixbuf_get_width(cursor_pixbuf);
    gdk_cursor_rect.height = gdk_pixbuf_get_height(cursor_pixbuf);

    if (gdk_rectangle_intersect(&gdk_screen_rect, &gdk_cursor_rect,
                                &gdk_cursor_rect)) {
      gint cursor_x = cx - xhot;
      gint cursor_y = cy - yhot;

      gdk_pixbuf_composite(cursor_pixbuf, screen_image, cursor_x, cursor_y,
                           gdk_cursor_rect.width, gdk_cursor_rect.height,
                           cursor_x, cursor_y, 1.0, 1.0, GDK_INTERP_BILINEAR,
                           255);
    }
  }
}

}  // namespace rdv
