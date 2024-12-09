#ifndef WINLIB_DB_H
#define WINLIB_DB_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// グローバル変数の宣言（定義は別の .c ファイルで行う）
extern Display *disp;
extern int scrn;
extern Window win, subwin;
extern Colormap cmap;
extern GC gc, gc_green[256], gc_orange[256], gc_red, gc_black;
extern XImage *image;
extern XColor color;
extern Pixmap pixmap, sub_pix; // for double buffering

// 関数プロトタイプ
void initwin();
void dot(int x, int y);
void text(int x, int y, char *str);
void g_line(int x0, int y0, int x1, int y1);
void g_rgb(unsigned short r, unsigned short g, unsigned short b);
void init_rgb_all();
void init_rgb_greens();
void g_fill(int x0, int y0, int wid, int hei);
void g_box(int x0, int y0, int wid, int hei);
void g_circle(int, int, int, GC);
void g_fcircle(int, int, int);
void g_clear(int i);
void display_update();

// マクロ定義
#define DEFAULT_WIDTH 600
#define DEFAULT_HEIGHT 400
#define SUB_SIZE 300
#define DISPLAY_DEPTH 8

#endif // WINLIB_DB_H
