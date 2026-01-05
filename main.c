#define _XOPEN_SOURCE 500

#include <raylib.h>

#define Font X11Font
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#undef Font

#include <stdio.h>
#include <stdlib.h>


unsigned long get_pixel_color(Display *disp, int x, int y) { // gets color of the x, y pixel. i just stole ts. :)
  XImage *image;
  image = XGetImage(disp, DefaultRootWindow(disp), x, y, 1, 1, AllPlanes, ZPixmap);
  unsigned long pixel = XGetPixel(image, 0, 0);
  XDestroyImage(image);
  return pixel;
}

Color convert_color(Display *disp, unsigned long pixel) { // converts the raw color into raylib color type.
  XVisualInfo vinfo;
  XMatchVisualInfo(disp, DefaultScreen(disp), DefaultDepth(disp, DefaultScreen(disp)), TrueColor, &vinfo);
  unsigned long r = (pixel & vinfo.visual->red_mask);
  unsigned long g = (pixel & vinfo.visual->green_mask);
  unsigned long b = (pixel & vinfo.visual->blue_mask);

  r >>= 16;
  g >>= 8;

  return (Color){ (unsigned char)r, (unsigned char)g, (unsigned char)b, 255 };
}

void rgb_to_hex(Color color, char* buffer, size_t size) {
  snprintf(buffer, size, "#%02X%02X%02X", color.r, color.g, color.b);
}

int main() {
  Display *display;

  display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "Cannot open display\n");
    return 1;
  }

  int monitor = GetCurrentMonitor();
  int width = GetMonitorWidth(monitor);
  int height = GetMonitorHeight(monitor);

  SetConfigFlags(FLAG_BORDERLESS_WINDOWED_MODE | FLAG_FULLSCREEN_MODE | FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_UNDECORATED);
  InitWindow(width, height, "Grab");

  SetTargetFPS(60);
  SetMouseCursor(MOUSE_CURSOR_CROSSHAIR);

  while (!WindowShouldClose()) {
    Vector2 mouse_pos = GetMousePosition();

    unsigned long raw_pixel = get_pixel_color(display, (int)mouse_pos.x, (int)mouse_pos.y);
    Color current_color = convert_color(display, raw_pixel);

    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      SetWindowPosition(-2000, -2000);

      printf("%d %d %d\n", current_color.r, current_color.g, current_color.b);

      FILE* pipe = popen("xclip -selection clipboard", "w");

      char hex[8];
      rgb_to_hex(current_color, hex, 8);
      printf("%s\n", hex);
      if (pipe != NULL) {
	fprintf(pipe, "%s", hex);
	pclose(pipe);
      }

      break;
    }

    BeginDrawing();
    ClearBackground(BLANK);
    DrawRectangle(mouse_pos.x + 10, mouse_pos.y + 10, 60, 60, GRAY);
    DrawRectangle(mouse_pos.x + 15, mouse_pos.y + 15, 50, 50, current_color);
    EndDrawing();
  }
  CloseWindow();

  XCloseDisplay(display);
  return 0;
}
