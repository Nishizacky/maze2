#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "utility.h"
#include "winlib_db.h"

#define P1 100, 100
#define P2 100, 200
#define P3 150, 200
#define P4 200, 100
#define P5 200, 200
#define P6 300, 100
#define P7 300, 200
#define P8 400, 100
#define P9 400, 200
#define P10 500, 100
#define P11 500, 150
#define P12 500, 200

void anime_menu();
void anime_newgame();
void anime_loadgame();
void anime_gameover();
void anime_gameclear();
void anime_gameover();

void main_menu() {
    char c;
    int i;
    int pointer = 0;
    int flag = 0;
    int before_pointer = 1;
    anime_menu();
    while ((1)) {
        if (kbhit()) {
            switch (c = getchar()) {
            case 'w':
                pointer = 0;
                break;
            case 's':
                pointer = 1;
                break;
            case 'x':
                pointer = 2;
                break;
            case 'k':
                flag = 1; //決定コマンド
                break;
            }
        }
        if (pointer != before_pointer) {
            XFillRectangle(disp, pixmap, gc_black, 370, 280, 20, 63);
            XDrawString(disp, pixmap, gc_orange[255], 370, 295 + pointer * 20, "<k", strlen("<k"));
            before_pointer = pointer;
            display_update();
        }
        if (flag == 1) {
            if (pointer == 0) {
                anime_newgame();
            }
            if (pointer == 1) {
                printf("write a code\n");
                exit(1);
            }
            if (pointer == 2) {
                printf("\n");
                exit(1);
            }
            break;
        }
    }
}

void gameclear() { anime_gameclear(); }
void gameover() { anime_gameover(); }

void anime_menu() {
    // TITLE
    // XFillRectangle(disp, pixmap, gc\\,100,100,400,100);

    int i;
    for (i = 3; i < 301; i++) {
        g_circle(DEFAULT_WIDTH / 2, DEFAULT_HEIGHT / 2, i, gc_green[255]);
        // g_circle(DEFAULT_WIDTH / 2, DEFAULT_HEIGHT / 2, i-1, gc_green[200]);
        // g_circle(DEFAULT_WIDTH / 2, DEFAULT_HEIGHT / 2, i-2, gc_green[150]);
        // g_circle(DEFAULT_WIDTH / 2, DEFAULT_HEIGHT / 2, i-3, gc_green[100]);
        display_update();
        XFillRectangle(disp, pixmap, gc_green[5*(i%10)], 0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT);
        usleep(1000);
    }
    XDrawLine(disp, pixmap, gc_green[255], P1, P2);
    XDrawLine(disp, pixmap, gc_green[255], P1, P3);
    XDrawLine(disp, pixmap, gc_green[255], P3, P4);
    XDrawLine(disp, pixmap, gc_green[255], P4, P5);
    XDrawLine(disp, pixmap, gc_green[255], P5, P6);
    XDrawLine(disp, pixmap, gc_green[255], 250, 150, 300, 150);
    XDrawLine(disp, pixmap, gc_green[255], P6, P7);
    XDrawLine(disp, pixmap, gc_green[255], P6, P8);
    XDrawLine(disp, pixmap, gc_green[255], P7, P8);
    XDrawLine(disp, pixmap, gc_green[255], P7, P9);
    XDrawLine(disp, pixmap, gc_green[255], P8, P9);
    XDrawLine(disp, pixmap, gc_green[255], P8, P10);
    XDrawLine(disp, pixmap, gc_green[255], 400, 150, P11);
    XDrawLine(disp, pixmap, gc_green[255], P9, P12);
    display_update();
    usleep(2000000);
    for (i = 0; i < 256; i++) {
        XFillRectangle(disp, pixmap, gc_green[255 * 25000 / 65535 * i / 256], 250 - 7, 50, 15, 220 - 50);
        XFillRectangle(disp, pixmap, gc_green[255 * 25000 / 65535 * i / 256], 350 - 7, 50, 15, 220 - 50);
        XFillRectangle(disp, pixmap, gc_green[255 * 25000 / 65535 * i / 256], 200, 50, 200, 15);
        XFillRectangle(disp, pixmap, gc_green[255 * 25000 / 65535 * i / 256], 200, 220, 200, 15);
        XDrawLine(disp, pixmap, gc_green[255], P1, P2);
        XDrawLine(disp, pixmap, gc_green[255], P1, P3);
        XDrawLine(disp, pixmap, gc_green[255], P3, P4);
        XDrawLine(disp, pixmap, gc_green[255], P4, P5);
        XDrawLine(disp, pixmap, gc_green[255], P5, P6);
        XDrawLine(disp, pixmap, gc_green[255], 250, 150, 300, 150);
        XDrawLine(disp, pixmap, gc_green[255], P6, P7);
        XDrawLine(disp, pixmap, gc_green[255], P6, P8);
        XDrawLine(disp, pixmap, gc_green[255], P7, P8);
        XDrawLine(disp, pixmap, gc_green[255], P7, P9);
        XDrawLine(disp, pixmap, gc_green[255], P8, P9);
        XDrawLine(disp, pixmap, gc_green[255], P8, P10);
        XDrawLine(disp, pixmap, gc_green[255], 400, 150, P11);
        XDrawLine(disp, pixmap, gc_green[255], P9, P12);
        display_update();
        usleep(100000 / FPS * 5);
    }
    XDrawString(disp, pixmap, gc_orange[160], 250, 260, "by Kai NISHIZAKI", strlen("by Kai NISHIZAKI"));
    usleep(1500000);
    display_update();
    usleep(1500000);
    XDrawRectangle(disp, pixmap, gc_green[255], 200, 275, 200, 70);
    XDrawString(disp, pixmap, gc_green[255], 270, 295, "NEW GAME :w", strlen("NEW GAME :w"));
    XDrawString(disp, pixmap, gc_green[255], 240, 315, "LOAD CASTOM MAP :s", strlen("LOAD CASTOM MAP :s"));
    XDrawString(disp, pixmap, gc_green[255], 290, 335, "EXIT :x", strlen("EXIT :x"));
}

void anime_newgame() {
    int i;
    XFillRectangle(disp, pixmap, gc_black, 0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT);
    XFillRectangle(disp, pixmap, gc_green[255 * 25000 / 65535], 250 - 7, 50, 15, 220 - 50);
    XFillRectangle(disp, pixmap, gc_green[255 * 25000 / 65535], 350 - 7, 50, 15, 220 - 50);
    XFillRectangle(disp, pixmap, gc_green[255 * 25000 / 65535], 200, 50, 200, 15);
    XFillRectangle(disp, pixmap, gc_green[255 * 25000 / 65535], 200, 220, 200, 15);
    display_update();
    usleep(500000);
    for (i = 0; i < 57; i++) {
        XFillRectangle(disp, pixmap, gc_black, 0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT);
        XFillRectangle(disp, pixmap, gc_green[255 * 25000 / 65535], 250 - 7, 50 + i, 15, 220 - 50);
        XFillRectangle(disp, pixmap, gc_green[255 * 25000 / 65535], 350 - 7, 50 + i, 15, 220 - 50);
        XFillRectangle(disp, pixmap, gc_green[255 * 25000 / 65535], 200, 50 + i, 200, 15);
        XFillRectangle(disp, pixmap, gc_green[255 * 25000 / 65535], 200, 220 + i, 200, 15);
        usleep(flamerate / 5);
        display_update();
    }
    usleep(800000);
    XFillRectangle(disp, pixmap, gc_green[255 * 25000 / 65535], 0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT);
    XFillRectangle(disp, pixmap, gc_black, 258, 122, 100, 155);
    display_update();
    for (i = 0; i < 300; i += 1) {
        XFillRectangle(disp, pixmap, gc_black, 258 - i, 122 - i, 100 + 2 * i, 155 + 2 * i);
        display_update();
        usleep(flamerate / 5);
    }
    // g_rgb(Green);
    // turn(45);
    // set_pos(200,107);
    // dragon(290,20,1);
}

void g_line_wait(double x, double y, double x1, double y1) {
    int i;
    int repeat = 5;
    for (i = 0; i < repeat; i++) {
        XDrawLine(disp, pixmap, gc_orange[255 * i / repeat], x, y, x1, y1);
        usleep(5000);
        display_update();
    }
}

void anime_gameclear() {
    float p = 600 / 8;
    float pp = p / 2;
    float p2 = p * 2;
    float p3 = p * 3;
    float p4 = p * 4;
    float p5 = p * 5;
    float p6 = p * 6;
    float p7 = p * 7;

    int i, j, r;
    int a, b;
    a = 100;
    b = 70;
    for (i = 0; i < a; i++) {
        XFillRectangle(disp, pixmap, gc_orange[255 * i / a], 0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT);
        display_update();
        usleep(flamerate);
    }
    for (i = b; i >= 0; i--) {
        XFillRectangle(disp, pixmap, gc_orange[255 * i / b], 0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT);
        display_update();
        usleep(flamerate);
    }

    // G
    g_line_wait(p, 100, p2, 100);
    g_line_wait(p, 100, p, 200);
    g_line_wait(p, 200, p2, 200);
    g_line_wait(p2, 200, p2, 150);
    g_line_wait(p * 1.5, 150, p2, 150);
    // A
    g_line_wait(p2, 200, p3, 100);
    g_line_wait(p3, 100, p3, 200);
    g_line_wait(p2 + pp, 150, p3, 150);
    // M
    g_line_wait(p3, 100, pp + p3, 200);
    g_line_wait(pp + p3, 200, p4, 100);
    g_line_wait(p4, 100, p4, 200);
    // E
    g_line_wait(p4, 100, p5, 100);
    g_line_wait(p4, 150, p5, 150);
    g_line_wait(p4, 200, p5, 200);
    // C
    g_line_wait(p2 + pp, 200, p3, 200);
    g_line_wait(p2 + pp, 200, p2, 250);
    g_line_wait(p2, 250, p2 + pp, 300);
    g_line_wait(p2 + pp, 300, p3, 300);
    // L
    g_line_wait(p3 + pp, 200, p3, 250);
    g_line_wait(p3, 250, p3, 300);
    g_line_wait(p3, 300, p4, 300);
    // E
    g_line_wait(p4, 200, p5, 200);
    g_line_wait(p4, 250, p5, 250);
    g_line_wait(p4, 300, p5, 300);
    // A
    g_line_wait(p6, 200, p5, 300);
    g_line_wait(p6, 300, p6, 200);
    g_line_wait(p5 + pp, 250, p6, 250);
    // R
    g_line_wait(p6, 200, p7, 200);
    g_line_wait(p7, 200, p7, 250);
    g_line_wait(p6, 250, p7, 250);
    g_line_wait(p6 + pp, 250, p7, 300);

    usleep(700000);
    XDrawString(disp, pixmap, gc_orange[255], 250, 320, "Press ENTER to exit", strlen("Press ENTER to exit"));
    display_update();
}

void g_line_wait2(double x, double y, double x1, double y1) {
    int i;
    int repeat = 5;
    for (i = 0; i < repeat; i++) {
        XDrawLine(disp, pixmap, gc_green[255 * i / repeat], x, y, x1, y1);
        usleep(5000);
        display_update();
    }
}

void anime_gameover() {
    int i, j;
    j = 25;
    for (i = 0; i <= j; i++) {
        XFillRectangle(disp, pixmap, gc_green[255 * i / j], 0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT);
        display_update();
        usleep(flamerate);
    }
    for (i = 0; i < DEFAULT_HEIGHT / 2 - 1; i += 1) {
        XFillRectangle(disp, pixmap, gc_black, 0, 0, DEFAULT_WIDTH, i);
        XFillRectangle(disp, pixmap, gc_black, 0, DEFAULT_HEIGHT - i, DEFAULT_WIDTH, DEFAULT_HEIGHT / 2);
        display_update();
        usleep(flamerate / 5);
    }
    for (i = 0; i < DEFAULT_WIDTH / 6 * 4; i += 1) {
        XFillRectangle(disp, pixmap, gc_black, 0, 0, i, DEFAULT_HEIGHT);
        XFillRectangle(disp, pixmap, gc_black, DEFAULT_WIDTH - i, 0, DEFAULT_WIDTH / 2, DEFAULT_HEIGHT);
        display_update();
        usleep(flamerate / 10);
    }

    float p = 600 / 8;
    float pp = p / 2;
    float p2 = p * 2;
    float p3 = p * 3;
    float p4 = p * 4;
    float p5 = p * 5;
    float p6 = p * 6;
    float p7 = p * 7;

    p += pp;
    p2 += pp;
    p3 += pp;
    p4 += pp;
    p5 += pp;
    p6 += pp;
    p7 += pp;

    // G
    g_line_wait2(p, 100, p2, 100);
    g_line_wait2(p, 100, p, 200);
    g_line_wait2(p, 200, p2, 200);
    g_line_wait2(p2, 200, p2, 150);
    g_line_wait2(p + pp, 150, p2, 150);
    // A
    g_line_wait2(p2, 200, p3, 100);
    g_line_wait2(p3, 100, p3, 200);
    g_line_wait2(p2 + pp, 150, p3, 150);
    // M
    g_line_wait2(p3, 100, pp + p3, 200);
    g_line_wait2(pp + p3, 200, p4, 100);
    g_line_wait2(p4, 100, p4, 200);
    // E
    g_line_wait2(p4, 100, p5, 100);
    g_line_wait2(p4, 150, p5, 150);
    g_line_wait2(p4, 200, p5, 200);
    // O
    g_line_wait2(p2 + pp, 200, p3, 200);
    g_line_wait2(p2 + pp, 200, p2, 250);
    g_line_wait2(p2, 250, p2, 300);
    g_line_wait2(p2, 300, p3, 300);
    g_line_wait2(p3, 200, p3, 300);
    // V
    g_line_wait2(p3, 300, p4, 200);
    // E
    g_line_wait2(p4, 200, p5, 200);
    g_line_wait2(p4, 250, p5, 250);
    g_line_wait2(p4, 300, p5, 300);
    // R
    g_line_wait2(p5, 200, p5, 300);
    g_line_wait2(p5, 200, p6, 200);
    g_line_wait2(p6, 200, p6, 250);
    g_line_wait2(p5, 250, p6, 250);
    g_line_wait2(p5 + pp, 250, p6, 300);

    XDrawString(disp, pixmap, gc_green[255], 250, 320, "Press ENTER to Retry", strlen("Press ENTER to Retry"));
    usleep(2000000);
    display_update();
}