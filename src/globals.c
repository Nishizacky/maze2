#include "winlib_db.h"
#include "utility.h"

// winlib_db.h
Display *disp;
int scrn;
Window win, subwin;
Colormap cmap;
GC gc, gc_green[256], gc_orange[256], gc_red, gc_black;
XImage *image;
XColor color;
Pixmap pixmap, sub_pix;

// utility.h
int maze[MEIRO_HEIGHT][MEIRO_WIDTH];
double fire_timer;
int DMG_flag;
int reload_flag;
int hit_flag;
double beat_ct;
double map_ct;
double reload_timer;
double dmg_timer;
int detect_flag;
pthread_t pthread, draw_thread;
int pthread_kill;
struct player p, enemy;
