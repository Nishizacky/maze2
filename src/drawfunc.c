#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include "utility.h"
#include "winlib_db.h"

extern double map_ct;

void draw_view();
void draw_map();
void draw_cpass();
void draw_life();
void draw_ammo();
void draw_warp();
void dmg_disp();
void g_num(int n, int x, int y, int size); // n:描画する数字
void g_num_Red(int n, int x, int y, int size);
void draw_enemy();
void firefrush();
void subDispDraw();

void *draw() {
    while (1) {
        XFillRectangle(disp, pixmap, gc_black, 0, 0, 600, 400);
        XFillRectangle(disp, sub_pix, gc_black, 0, 0, SUB_SIZE, SUB_SIZE);
        draw_view();
        dmg_disp();
        draw_enemy();
        draw_map();
        draw_life();
        draw_ammo();
        draw_cpass();
        draw_warp();
        subDispDraw();
        display_update();
        usleep(1000000 / FPS);
        if (pthread_kill == 1) {
            break;
        }
    }
}

void draw_view() {
    float l, height;
    double theta = p.angle - M_PI / 2;
    int j, k;
    int x, y;
    int green_d;
    int ol, nl;
    // printf("(%f,%f),deg %d\n", (float)p.vec[0], (float)p.vec[1],(int)(p.angle/M_PI * 180));
    //星の描画
    for (k = 0; k < 50; k++) {
        XDrawPoint(disp, pixmap, gc_green[256 / 5 * 2], rand() % 600, rand() % 150);
    }
    for (k = 0; k < 10; k++) {
        XFillRectangle(disp, pixmap, gc_green[256 / 5 * 2], rand() % 600, rand() % 150, 3, 3);
    }
    if ((NOW_TIME - fire_timer) / CLOCKS_PER_SEC < FIRE_FRUSH_LEN) {
        XFillRectangle(disp, pixmap, gc_orange[120], 0, 0, DEFAULT_WIDTH, 300);
    }
    //メイン
    for (k = 0; k < RayAmount; k++) {
        nl = 0;
        for (l = 0; l < UnitRayLength; l += 0.01) {
            nl = 256 * (1 - l / UnitRayLength);
            x = p.vec[0] + l * cos(p.angle + (k - (RayAmount + 1) / 2) * UnitRayAngle);
            y = p.vec[1] + l * sin(p.angle + (k - (RayAmount + 1) / 2) * UnitRayAngle);
            if (maze[x][y] == GOAL) {
                height = 300 * (UnitRayLength - l * cos((k - (RayAmount + 1) / 2) * UnitRayAngle)) / UnitRayLength;
                XDrawRectangle(disp, pixmap, gc_orange[nl], k * DEFAULT_WIDTH / RayAmount, 150 - height / 2, DEFAULT_WIDTH / RayAmount, height);
                break;
            }
            if (maze[x][y] == WALL) {
                if (ol != nl) {
                    ol = nl;
                }
                height = 300 * (UnitRayLength - l * cos((k - (RayAmount + 1) / 2) * UnitRayAngle)) / UnitRayLength;
                if ((NOW_TIME - fire_timer) / CLOCKS_PER_SEC > FIRE_FRUSH_LEN) {
                    XFillRectangle(disp, pixmap, gc_green[nl], k * DEFAULT_WIDTH / RayAmount + 0.5, 150 - height / 2, DEFAULT_WIDTH / RayAmount - 1, height);
                }
                if ((NOW_TIME - fire_timer) / CLOCKS_PER_SEC < FIRE_FRUSH_LEN) {
                    XFillRectangle(disp, pixmap, gc_orange[nl], k * DEFAULT_WIDTH / RayAmount + 0.5, 150 - height / 2, DEFAULT_WIDTH / RayAmount - 1, height);
                }

                break;
            }
        }
    }
}

// int map_memoX = 0;
// int map_memoY = 0;

void draw_map() {
    int i, j, h;
    // if(abs(p.vec[0]-map_memoX)>1 || abs(p.vec[1]-map_memoY)>1){
    //迷路地図
    if ((clock() - map_ct) / CLOCKS_PER_SEC < MAP_TIMER) {
        for (i = 0; i < 5; i++) {
            for (j = 0; j < 5; j++) {
                switch (maze[((int)p.vec[0]) - 2 + j][((int)p.vec[1]) - 2 + i]) {
                case PATH:
                    XDrawRectangle(disp, pixmap, gc_green[256 / 5 * 4], 200 + 20 * j, 300 + 20 * i, 20, 20);
                    break;
                case GOAL:
                    XDrawRectangle(disp, pixmap, gc_green[256 / 5 * 4], 200 + 20 * j, 300 + 20 * i, 20, 20);
                    XDrawRectangle(disp, pixmap, gc_green[256 / 5 * 4], 200 + 20 * j + 4, 300 + 20 * i + 4, 10, 10);
                    break;
                default:
                    XFillRectangle(disp, pixmap, gc_green[256 / 5 * 4], 200 + 20 * j, 300 + 20 * i, 20, 20);
                    break;
                }
            }
            h = 100 * (1 - (clock() - map_ct) / CLOCKS_PER_SEC / MAP_TIMER);
            XFillRectangle(disp, pixmap, gc_green[256 / 5 * 2], 180, 400 - h, 18, h);
        }
    } else {
        XDrawRectangle(disp, pixmap, gc_black, 200, 300, 100, 100);
    }
    XDrawRectangle(disp, pixmap, gc_green[256 / 5 * 4], 180, 300, 18, 100);
    //レーダー
    XDrawArc(disp, pixmap, gc_green[256 / 5 * 4], 300 + 1, 300 + 1, 98, 98, 0, 64 * 360);
    XDrawLine(disp, pixmap, gc_green[256 / 5 * 4], 350, 350, 350 + 98 / 2 * cos(M_PI / 360 * FOV - M_PI / 2), 350 + 98 / 2 * sin(M_PI / 360 * FOV - M_PI / 2));
    XDrawLine(disp, pixmap, gc_green[256 / 5 * 4], 350, 350, 350 + 98 / 2 * cos(-M_PI / 360 * FOV - M_PI / 2), 350 + 98 / 2 * sin(-M_PI / 360 * FOV - M_PI / 2));
}

void draw_cpass() {
    int l = 8, m = 8;
    double theta = p.angle;
    double deg45 = M_PI / 4;
    XDrawLine(disp, pixmap, gc_green[256 / 5 * 4], 250 + l * cos(theta), 350 + l * sin(theta), 250 - l * cos(theta + deg45), 350 - l * sin(theta + deg45));
    XDrawLine(disp, pixmap, gc_green[256 / 5 * 4], 250 + l * cos(theta), 350 + l * sin(theta), 250 - l * cos(theta - deg45), 350 - l * sin(theta - deg45));
}

void draw_life() {
    int a1, a10;
    a10 = p.HP / 10;
    a1 = p.HP % 10;
    extern int DMG_flag;
    if (DMG_flag == 1) {
        XFillRectangle(disp, pixmap, gc_red, 0, 300, 100, 100);
        DMG_flag = 0;
    } else {
        if (p.HP > 20) {
            g_num(a10, 30, 330, 50);
            g_num(a1, 70, 330, 50);
            XDrawRectangle(disp, pixmap, gc_green[256 / 5 * 4], 15, 385, 100, 10);
            XFillRectangle(disp, pixmap, gc_green[256 / 5 * 4], 15, 385, 100 * p.HP / 99, 10);
        } else {
            g_num_Red(a10, 30, 330, 50);
            g_num_Red(a1, 70, 330, 50);
            XDrawRectangle(disp, pixmap, gc_red, 15, 385, 100, 10);
            XFillRectangle(disp, pixmap, gc_red, 15, 385, 100 * p.HP / 99, 10);
        }
    }
}

void draw_ammo() {
    int a1, a10;
    a10 = p.ammo / 10;
    a1 = p.ammo % 10;
    if (p.ammo > 5) {
        g_num(a10, DEFAULT_WIDTH - 75, 330, 50);
        g_num(a1, DEFAULT_WIDTH - 35, 330, 50);
    } else {
        g_num_Red(a10, DEFAULT_WIDTH - 75, 330, 50);
        g_num_Red(a1, DEFAULT_WIDTH - 35, 330, 50);
    }
    int i;
    // sabWin
    if (reload_flag == 0) {
        for (i = 0; i < p.ammo; i++) {
            XFillRectangle(disp, sub_pix, gc_orange[160], (SUB_SIZE - 5 * AMMO_MAX) / 2 + i * 5, SUB_SIZE - 20, 4, 15);
        }
        for (i = 0; i < AMMO_MAX; i++) {
            if (p.ammo > 0) {
                XDrawRectangle(disp, sub_pix, gc_green[255], (SUB_SIZE - 5 * AMMO_MAX) / 2 + i * 5, SUB_SIZE - 20, 4, 15);
            } else {
                XDrawRectangle(disp, sub_pix, gc_red, (SUB_SIZE - 5 * AMMO_MAX) / 2 + i * 5, SUB_SIZE - 20, 4, 15);
            }
        }
    } else {
        reload_weapon();
    }
}

void draw_warp() {
    int i;
    int x, y, h, w;
    x = 460;
    y = 325;
    h = 19;
    w = 40;
    if (p.warp > 0) {
        for (i = 0; i < 3; i++) {
            XFillRectangle(disp, pixmap, gc_green[256 / 5 * 4], x, y + 23 * i, w, h);
        }
        for (i = 0; i < (3 - p.warp); i++) {
            XFillRectangle(disp, pixmap, gc_black, x + 1, y + 1 + 23 * i, w - 2, h - 2);
        }
    } else {
        for (i = 0; i < 3; i++) {
            XFillRectangle(disp, pixmap, gc_red, x, y + 23 * i, w, h);
        }
        for (i = 0; i < (3 - p.warp); i++) {
            XFillRectangle(disp, pixmap, gc_black, x + 1, y + 1 + 23 * i, w - 2, h - 2);
        }
    }
}

void dmg_disp() {
    if ((clock() - dmg_timer) / CLOCKS_PER_SEC < 0.5) {
        XFillRectangle(disp,pixmap,gc_red,0,0,DEFAULT_WIDTH,DEFAULT_HEIGHT-100);
        XFillArc(disp, pixmap, gc_black, 150, 0, 300, 300, 0, 64 * 360);
    }
}

void draw_enemy() {
    double dx, dy, theta, dist, height;
    dx = enemy.vec[0] - p.vec[0];
    dy = enemy.vec[1] - p.vec[1];
    theta = atan2(dy, dx) - p.angle;
    if (theta > M_PI * 2) {
        theta -= M_PI * 2;
    }
    if (theta < -M_PI * 2) {
        theta += M_PI * 2;
    }
    double map_theta = theta - M_PI_2;
    dist = dx * dx + dy * dy;
    height = 300 * (UnitRayLength - sqrt(dist)) / UnitRayLength;
    if (dist < UnitRayLength * UnitRayLength) {
        //ミニマップへの描画
        int map_x, map_y;
        map_x = sqrt(dist) / UnitRayLength * 50 * cos(map_theta);
        map_y = sqrt(dist) / UnitRayLength * 50 * sin(map_theta);
        map_x += 350;
        map_y += 350;
        XFillArc(disp, pixmap, gc_red, map_x - 2, map_y - 2, 4, 4, 0, 360 * 64);
        //視界への描画
        if (theta < M_PI / 180 * FOV && -(M_PI / 180 * FOV) < theta) {
            int draw_x, draw_y;
            draw_x = 300 * sin(theta) + 300;
            draw_y = 150 - height / 2;
            // printf("%f\n", sqrt(dist));
            XFillRectangle(disp, pixmap, gc_red, draw_x - height / 8, draw_y + 30, height / 4, height);
        }
    }
    extern int detect_flag;
    if (detect_flag == 1) {
        XDrawRectangle(disp, pixmap, gc_red, 1, 1, DEFAULT_WIDTH - 2, DEFAULT_HEIGHT - 2);
        detect_flag == 0;
    }
    // printf("%2.3f, %2.3f, %2.3f\n", enemy.vec[0] - p.vec[0], enemy.vec[1] -
    // p.vec[1], theta);
}

void subDispDraw() {
    // int ratio;
    // ratio = 15;
    // XFillArc(disp, sub_pix, gc_green[255], p.vec[0] * ratio - 5, p.vec[1] * ratio - 5, 10, 10, 0, 360 * 64);
    // XDrawLine(disp, sub_pix, gc_green[225], p.vec[0] * ratio, p.vec[1] * ratio, p.vec[0] * ratio + ratio * cos(p.angle), p.vec[1] * ratio + ratio * sin(p.angle));
    // XDrawArc(disp, sub_pix, gc_red, enemy.vec[0] * ratio, enemy.vec[1] * ratio, 5, 5, 0, 64 * 360);

    double theta, x, y, theta2, dist;
    int height;
    x = enemy.vec[0] - p.vec[0];
    y = enemy.vec[1] - p.vec[1];
    theta = atan2(y, x);
    theta2 = theta - p.angle;
    theta2 /= 2;
    dist = x * x + y * y;
    if (theta2 > M_PI) {
        theta2 -= M_PI * 2;
    }
    if (theta2 < -M_PI) {
        theta2 += M_PI * 2;
    }

    //敵出現時
    if (dist < UnitRayLength * UnitRayLength) {
        // XDrawLine(disp, sub_pix, gc_red, SUB_SIZE / 2 + (SUB_SIZE - 40) / 2 * sin(theta2), 15, SUB_SIZE / 2 + (SUB_SIZE - 40) / 2 * sin(theta2), 45);
        //ガイドアロー表示
        if (theta2 > 0) {
            XDrawString(disp, sub_pix, gc_red, SUB_SIZE - 20, 50, ">", strlen(">"));
        }
        if (theta2 < 0) {

            XDrawString(disp, sub_pix, gc_red, 20, 50, "<", strlen("<"));
        }
        //敵の描画
        if (theta2 < M_PI && theta2 > -M_PI) {
            int draw_y;
            height = 120 * (UnitRayLength - sqrt(dist)) / UnitRayLength;
            draw_y = SUB_SIZE / 2 - height / 2;
            XFillRectangle(disp, sub_pix, gc_red, SUB_SIZE / 2 + SUB_SIZE * sin(theta2) - height / 8, draw_y + 20, height / 4, height);
        }
    }
    //ヒットマーカー
    if (hit_flag > 0) {
        XDrawArc(disp, sub_pix, gc_orange[255], SUB_SIZE / 2 - 20, SUB_SIZE / 2 - 20, 40, 40, 0, 64 * 360);
        display_update();
        usleep(100000);
        hit_flag -= 0.1;
    }
    XDrawArc(disp, sub_pix, gc_orange[50], SUB_SIZE / 2 - 20, SUB_SIZE / 2 - 20, 40, 40, 0, 64 * 360);
    //キルマーカー
    if ((clock() - beat_ct) / CLOCKS_PER_SEC < 1) {
        XDrawString(disp, sub_pix, gc_red, (SUB_SIZE - 90) / 2, (SUB_SIZE + 45) / 2, "ENEMY NEUTRALIZED", strlen("ENEMY NEUTRALIZED"));
        display_update();
    }
    //レティクル
    XDrawRectangle(disp, sub_pix, gc_green[100], SUB_SIZE / 2 - 5, SUB_SIZE / 2 - 5, 10, 10);
    XDrawRectangle(disp, sub_pix, gc_green[130], SUB_SIZE / 2 - 80, SUB_SIZE / 2 - 80, 160, 160);
    XDrawLine(disp, sub_pix, gc_green[200], SUB_SIZE / 2 - 80, SUB_SIZE / 2, SUB_SIZE / 2 - 40, SUB_SIZE / 2);
    XDrawLine(disp, sub_pix, gc_green[200], SUB_SIZE / 2 + 80, SUB_SIZE / 2, SUB_SIZE / 2 + 40, SUB_SIZE / 2);
    XDrawLine(disp, sub_pix, gc_green[200], SUB_SIZE / 2, SUB_SIZE / 2 - 80, SUB_SIZE / 2, SUB_SIZE / 2 - 40);
    XDrawLine(disp, sub_pix, gc_green[200], SUB_SIZE / 2, SUB_SIZE / 2 + 80, SUB_SIZE / 2, SUB_SIZE / 2 + 40);
}

//以前の課題で作ったデジタル風数字を描画するコード

void g_nFracV(int x0, int y0, int x1,
              int y1) { //縦長の六角形を一つ描画する。上端と下端の座標が必要
    double length = sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
    double d = length / 8;
    double r = d * sqrt(2);
    XDrawLine(disp, pixmap, gc_green[256 / 5 * 4], x0, y0, x0 - r * cos(M_PI / 4), y0 + r * sin(M_PI / 4));
    XDrawLine(disp, pixmap, gc_green[256 / 5 * 4], x0, y0, x0 + r * cos(M_PI / 4), y0 + r * sin(M_PI / 4));
    XDrawLine(disp, pixmap, gc_green[256 / 5 * 4], x1, y1, x1 - r * cos(M_PI / 4), y1 - r * sin(M_PI / 4));
    XDrawLine(disp, pixmap, gc_green[256 / 5 * 4], x1, y1, x1 + r * cos(M_PI / 4), y1 - r * sin(M_PI / 4));
    XDrawLine(disp, pixmap, gc_green[256 / 5 * 4], x0 - r * cos(M_PI / 4), y0 + r * sin(M_PI / 4), x1 - r * cos(M_PI / 4), y1 - r * sin(M_PI / 4));
    XDrawLine(disp, pixmap, gc_green[256 / 5 * 4], x0 + r * cos(M_PI / 4), y0 + r * sin(M_PI / 4), x1 + r * cos(M_PI / 4), y1 - r * sin(M_PI / 4));
}

void g_nFracH(int x0, int y0, int x1,
              int y1) { //横長の六角形を一つ描画する。左端と右端の座標が必要
    double length = sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
    double d = length / 8;
    double r = d * sqrt(2);
    XDrawLine(disp, pixmap, gc_green[256 / 5 * 4], x0, y0, x0 + r * cos(M_PI / 4), y0 - r * sin(M_PI / 4));
    XDrawLine(disp, pixmap, gc_green[256 / 5 * 4], x0, y0, x0 + r * cos(M_PI / 4), y0 + r * sin(M_PI / 4));
    XDrawLine(disp, pixmap, gc_green[256 / 5 * 4], x1, y1, x1 - r * cos(M_PI / 4), y1 - r * sin(M_PI / 4));
    XDrawLine(disp, pixmap, gc_green[256 / 5 * 4], x1, y1, x1 - r * cos(M_PI / 4), y1 + r * sin(M_PI / 4));
    XDrawLine(disp, pixmap, gc_green[256 / 5 * 4], x0 + r * cos(M_PI / 4), y0 + r * sin(M_PI / 4), x1 - r * cos(M_PI / 4), y1 + r * sin(M_PI / 4));
    XDrawLine(disp, pixmap, gc_green[256 / 5 * 4], x0 + r * cos(M_PI / 4), y0 - r * sin(M_PI / 4), x1 - r * cos(M_PI / 4), y1 - r * sin(M_PI / 4));
}

void g_num(int n, int x, int y, int size) {
    int skelton[10][7] = {//各数字のデジタル表記設計図0から順に9まで。
                          {1, 1, 1, 0, 1, 1, 1}, {0, 0, 0, 0, 0, 1, 1}, {0, 1, 1, 1, 1, 1, 0}, {0, 0, 1, 1, 1, 1, 1}, {1, 0, 0, 1, 0, 1, 1},
                          {1, 0, 1, 1, 1, 0, 1}, {1, 1, 1, 1, 1, 0, 1}, {1, 0, 1, 0, 0, 1, 1}, {1, 1, 1, 1, 1, 1, 1}, {1, 0, 1, 1, 1, 1, 1}};

    int i;
    if (skelton[n][0])
        g_nFracV(x, y, x, y + size / 2);
    if (skelton[n][1])
        g_nFracV(x, y + size / 2, x, y + size);
    if (skelton[n][2])
        g_nFracH(x, y, x + size / 2, y);
    if (skelton[n][3])
        g_nFracH(x, y + size / 2, x + size / 2, y + size / 2);
    if (skelton[n][4])
        g_nFracH(x, y + size, x + size / 2, y + size);
    if (skelton[n][5])
        g_nFracV(x + size / 2, y, x + size / 2, y + size / 2);
    if (skelton[n][6])
        g_nFracV(x + size / 2, y + size / 2, x + size / 2, y + size);
}

//赤色バージョン
void g_nFracV_Red(int x0, int y0, int x1, int y1) {
    double length = sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
    double d = length / 8;
    double r = d * sqrt(2);
    XDrawLine(disp, pixmap, gc_red, x0, y0, x0 - r * cos(M_PI / 4), y0 + r * sin(M_PI / 4));
    XDrawLine(disp, pixmap, gc_red, x0, y0, x0 + r * cos(M_PI / 4), y0 + r * sin(M_PI / 4));
    XDrawLine(disp, pixmap, gc_red, x1, y1, x1 - r * cos(M_PI / 4), y1 - r * sin(M_PI / 4));
    XDrawLine(disp, pixmap, gc_red, x1, y1, x1 + r * cos(M_PI / 4), y1 - r * sin(M_PI / 4));
    XDrawLine(disp, pixmap, gc_red, x0 - r * cos(M_PI / 4), y0 + r * sin(M_PI / 4), x1 - r * cos(M_PI / 4), y1 - r * sin(M_PI / 4));
    XDrawLine(disp, pixmap, gc_red, x0 + r * cos(M_PI / 4), y0 + r * sin(M_PI / 4), x1 + r * cos(M_PI / 4), y1 - r * sin(M_PI / 4));
}

void g_nFracH_Red(int x0, int y0, int x1,
                  int y1) { //横長の六角形を一つ描画する。左端と右端の座標が必要
    double length = sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
    double d = length / 8;
    double r = d * sqrt(2);
    XDrawLine(disp, pixmap, gc_red, x0, y0, x0 + r * cos(M_PI / 4), y0 - r * sin(M_PI / 4));
    XDrawLine(disp, pixmap, gc_red, x0, y0, x0 + r * cos(M_PI / 4), y0 + r * sin(M_PI / 4));
    XDrawLine(disp, pixmap, gc_red, x1, y1, x1 - r * cos(M_PI / 4), y1 - r * sin(M_PI / 4));
    XDrawLine(disp, pixmap, gc_red, x1, y1, x1 - r * cos(M_PI / 4), y1 + r * sin(M_PI / 4));
    XDrawLine(disp, pixmap, gc_red, x0 + r * cos(M_PI / 4), y0 + r * sin(M_PI / 4), x1 - r * cos(M_PI / 4), y1 + r * sin(M_PI / 4));
    XDrawLine(disp, pixmap, gc_red, x0 + r * cos(M_PI / 4), y0 - r * sin(M_PI / 4), x1 - r * cos(M_PI / 4), y1 - r * sin(M_PI / 4));
}

void g_num_Red(int n, int x, int y, int size) {
    int skelton[10][7] = {//各数字のデジタル表記設計図0から順に9まで。
                          {1, 1, 1, 0, 1, 1, 1}, {0, 0, 0, 0, 0, 1, 1}, {0, 1, 1, 1, 1, 1, 0}, {0, 0, 1, 1, 1, 1, 1}, {1, 0, 0, 1, 0, 1, 1},
                          {1, 0, 1, 1, 1, 0, 1}, {1, 1, 1, 1, 1, 0, 1}, {1, 0, 1, 0, 0, 1, 1}, {1, 1, 1, 1, 1, 1, 1}, {1, 0, 1, 1, 1, 1, 1}};

    int i;
    if (skelton[n][0])
        g_nFracV_Red(x, y, x, y + size / 2);
    if (skelton[n][1])
        g_nFracV_Red(x, y + size / 2, x, y + size);
    if (skelton[n][2])
        g_nFracH_Red(x, y, x + size / 2, y);
    if (skelton[n][3])
        g_nFracH_Red(x, y + size / 2, x + size / 2, y + size / 2);
    if (skelton[n][4])
        g_nFracH_Red(x, y + size, x + size / 2, y + size);
    if (skelton[n][5])
        g_nFracV_Red(x + size / 2, y, x + size / 2, y + size / 2);
    if (skelton[n][6])
        g_nFracV_Red(x + size / 2, y + size / 2, x + size / 2, y + size);
}

