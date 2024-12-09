#include "utility.h"
#include "winlib_db.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fp;

int width = DEFAULT_WIDTH;
int height = DEFAULT_HEIGHT;

void FatalError(char *str) {
    if (fp != NULL) {
        fclose(fp);
    }
    fprintf(stderr, "Error:%s\n", str);

    fflush(stderr);
    exit(1);
}

void initwin() {
    int i;
    XSetWindowAttributes wa;
    XVisualInfo vi;
    XSizeHints hints;
    XEvent event;
    if ((disp = XOpenDisplay(NULL)) == NULL) {
        FatalError("Can't open display.");
    }
    scrn = DefaultScreen(disp);

    /*  if(! XMatchVisualInfo(disp,scrn,DISPLAY_DEPTH,DirectColor, &vi)){
      FatalError("Can't open color display.");
      }
    wa.background_pixel = 0;
    wa.border_pixel =0;
    wa.colormap =XCreateColormap(disp,RootWindow(disp,scrn),
                                vi.visual,AllocNone);
    wa.event_mask = KeyPressMask |StructureNotifyMask |ExposureMask
      |ButtonPressMask;
    */
    win = XCreateSimpleWindow(disp, RootWindow(disp, scrn), 0, 0, width, height, 2, 2, WhitePixel(disp, scrn));
    subwin = XCreateSimpleWindow(disp, RootWindow(disp, scrn), 0, 0, SUB_SIZE, SUB_SIZE, 2, 2, WhitePixel(disp, scrn));
    pixmap = XCreatePixmap(disp, win, width, height, DefaultDepth(disp, 0));
    sub_pix = XCreatePixmap(disp, subwin, SUB_SIZE, SUB_SIZE, DefaultDepth(disp, 0));

    /*  image =XCreateImage(disp, qvi.visual, vi.depth,
                        ZPixmap, 0, 0, width, height, 32, 0);
    if ((image->data = (char *)malloc(image->bytes_per_line*height)) ==NULL){
      FatalError("No memory..");
    }
    memset(image->data,0, image->bytes_per_line*height);
    */
    gc = XCreateGC(disp, win, 0, 0);
    gc_red = XCreateGC(disp, win, 0, 0);
    gc_black = XCreateGC(disp, win, 0, 0);
    for (i = 0; i < 256; i++) {
        gc_green[i] = XCreateGC(disp, win, 0, 0);
        gc_orange[i] = XCreateGC(disp, win, 0, 0);
    }
    XSetForeground(disp, gc, BlackPixel(disp, scrn));
    XSetForeground(disp, gc_red, BlackPixel(disp, scrn));
    XSetForeground(disp, gc_black, BlackPixel(disp, scrn));
    for (i = 0; i < 256; i++) {
        XSetForeground(disp, gc_green[i], BlackPixel(disp, scrn));
        XSetForeground(disp, gc_orange[i], BlackPixel(disp, scrn));
    }
    XSetBackground(disp, gc, WhitePixel(disp, scrn));
    XSetBackground(disp, gc_red, WhitePixel(disp, scrn));
    XSetBackground(disp, gc_black, WhitePixel(disp, scrn));
    for (i = 0; i < 256; i++) {
        XSetBackground(disp, gc_green[i], WhitePixel(disp, scrn));
        XSetBackground(disp, gc_orange[i], WhitePixel(disp, scrn));
    }
    XSetFunction(disp, gc, GXcopy);
    XSetFunction(disp, gc_red, GXcopy);
    XSetFunction(disp, gc_black, GXcopy);
    for (i = 0; i < 256; i++) {
        XSetFunction(disp, gc_green[i], GXcopy);
        XSetFunction(disp, gc_orange[i], GXcopy);
    }
    init_rgb_all();
    XMapRaised(disp, win);
    XMapRaised(disp, subwin);
    g_clear(1);
    XFlush(disp);
}

void dot(int x, int y) { XDrawLine(disp, pixmap, gc, x, y, x, y); }

void text(int x, int y, char *str) { XDrawString(disp, pixmap, gc, x, y, str, strlen(str)); }

void g_line(int x0, int y0, int x1, int y1) { XDrawLine(disp, pixmap, gc, x0, y0, x1, y1); }

void g_rgb(unsigned short r, unsigned short g, unsigned short b) {
    XColor xc;
    xc.red = r;
    xc.green = g;
    xc.blue = b;
    XAllocColor(disp, DefaultColormap(disp, scrn), &xc);
    XSetForeground(disp, gc, xc.pixel);
}

void init_rgb(unsigned short r, unsigned short g, unsigned short b, GC gc) {
    XColor xc;
    xc.red = r;
    xc.green = g;
    xc.blue = b;
    XAllocColor(disp, DefaultColormap(disp, scrn), &xc);
    XSetForeground(disp, gc, xc.pixel);
}

void g_fill(int x0, int y0, int wid, int hei) { XFillRectangle(disp, pixmap, gc, x0, y0, wid, hei); }

void g_box(int x0, int y0, int wid, int hei) { XDrawRectangle(disp, pixmap, gc, x0, y0, wid, hei); }

void g_clear(int color) // 1->black,0->white
{
    if (color == 1) {
        XFillRectangle(disp, win, gc_black, 0, 0, width, height);
        XFillRectangle(disp, subwin, gc_black, 0, 0, SUB_SIZE, SUB_SIZE);
    } else {
        XFillRectangle(disp, win, gc, 0, 0, width, height);
        XFillRectangle(disp, subwin, gc_black, 0, 0, SUB_SIZE, SUB_SIZE);
    }
}

void display_update() { // for double buffering
    XCopyArea(disp, pixmap, win, gc, 0, 0, width, height, 0, 0);
    XCopyArea(disp, sub_pix, subwin, gc, 0, 0, SUB_SIZE, SUB_SIZE, 0, 0);
    XFlush(disp);
}

void g_circle(int x, int y, int r, GC GC) { XDrawArc(disp, pixmap, GC, x - r, y - r, 2 * r, 2 * r, 0, 64 * 360); }

void init_rgb_all() {
    int i;
    init_rgb(65535, 0, 0, gc_red);
    init_rgb(0, 0, 0, gc_black);
    init_rgb(0, 65535, 0, gc_green[255]);
}
void init_rgb_greens() {
    int i, x, y, h, w;
    x = 150;
    y = 210;
    w = 300;
    h = 20;
    XDrawString(disp, pixmap, gc_green[255], 270, 200, "Loading ...", strlen("Loading ..."));
    XDrawRectangle(disp, pixmap, gc_green[255], x, y, w, h);
    display_update();
    for (i = 0; i < 256; i++) {
        init_rgb(0, 65535 * i / 256, 0, gc_green[i]);
        init_rgb(65535 * i / 256, 65535 / 2 * i / 256, 0, gc_orange[i]);
        XFillRectangle(disp, pixmap, gc_green[255], x, y, w * i / 256, h);
        printf("Load #%d/256 completed\n", i + 1);
        display_update();
    }
    XFillRectangle(disp, pixmap, gc_black, 0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT);
    display_update();
}

void g_circle(int, int, int, GC);