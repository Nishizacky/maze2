#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "kbhit.h"
#include "utility.h"
#include "winlib_db.h"

extern int maze[MEIRO_HEIGHT][MEIRO_WIDTH];
extern int pthread_kill;

void moveVec(int);
void turnVec(int);

//基本的な操作を纏めてある
void action() {
    pthread_kill = 0;
    char c;
    int r = rand() % ((MEIRO_HEIGHT - 3) / 2);
    while (1) {
        int r;
        if (kbhit()) {
            switch (c = getchar()) {
            default:usleep(1000000/60);
            case 'w':
                moveVec(0);
                break;

            case 'a':
                moveVec(3);
                break;

            case 's':
                moveVec(2);
                break;

            case 'd':
                moveVec(1);
                break;

            case 'l':
                turnVec(0);
                break;

            case 'j':
                turnVec(1);
                break;

            case 'i':
                if (p.ammo > 0) {
                    fire_weapon();
                    fire_timer = clock();
                }
                break;

            case 'q':
                if (p.warp > 0) {
                    r = rand() % ((MEIRO_WIDTH - 3) / 2);
                    p.vec[0] = 2 * r + 1.5;
                    r = rand() % ((MEIRO_HEIGHT - 3) / 2);
                    p.vec[1] = 2 * r + 1.5;
                    p.warp--;
                }
                break;
            case 'r':
                if (p.ammo != AMMO_MAX && reload_flag != 1) {
                    reload_flag = 1;
                    p.ammo = 0;
                    reload_timer = NOW_TIME;
                }
                break;
            case 'u':
                p.angle += M_PI;
                break;
            }
        }
        if (maze[(int)p.vec[0]][(int)p.vec[1]] == GOAL || p.HP < 0) {
            pthread_kill = 1;
            break;
        }
    }
}

void moveVec(int i) {
    double l;
    int nx, ny;
    l = 0;
    while (1) {
        nx = p.vec[0] + l * cos(p.angle + i * M_PI / 2);
        if (maze[nx][(int)p.vec[1]] != WALL) {
            p.vec[0] += l * cos(p.angle + i * M_PI / 2);
        }
        if (l > UnitMoveLength) {
            break;
        }
        l += 0.05;
    }
    l = 0;
    while (1) {
        //切り捨て問題：マイナス値のとき、都合の悪い切り捨てられ方をされるため回避
        if (p.vec[1] + l * sin(p.angle + i * M_PI / 2) < 0) {
            ny = -(p.vec[1] + l * sin(p.angle + i * M_PI / 2));
            ny *= -1;
        } else {
            ny = p.vec[1] + l * sin(p.angle + i * M_PI / 2);
        }

        if (maze[(int)p.vec[0]][ny] != WALL) {
            p.vec[1] += l * sin(p.angle + i * M_PI / 2);
        }
        if (l > UnitMoveLength) {
            break;
        }
        l += 0.05;
    }

    // printf("result (%d, %d)\n",nx,ny);
}

void turnVec(int i) {
    // 0,1で反時計回り、時計回り
    p.angle += M_PI / 180 * UnitTurnAngle * (1 - 2 * i);
    if (p.angle > 2 * M_PI) {
        p.angle -= 2 * M_PI;
    }
    if (p.angle < -2 * M_PI) {
        p.angle += 2 * M_PI;
    }
}

void reload_weapon() {
    if ((NOW_TIME - reload_timer)/CLOCKS_PER_SEC < RELOAD_TIME) {
        double length = ((NOW_TIME - reload_timer)) / RELOAD_TIME/CLOCKS_PER_SEC;
        XDrawRectangle(disp, sub_pix, gc_green[255], (SUB_SIZE - 5 * AMMO_MAX) / 2, SUB_SIZE - 20, 5 * AMMO_MAX, 15);
        XFillRectangle(disp, sub_pix, gc_orange[150], (SUB_SIZE - 5 * AMMO_MAX) / 2 + 1, SUB_SIZE - 20 + 1, 5 * AMMO_MAX * length, 13);
        XDrawString(disp, sub_pix, gc_red, (SUB_SIZE - 5 * AMMO_MAX) / 2 + 50, SUB_SIZE - 25, "RELOADING", strlen("RELOADING"));
    } else {
        p.ammo = AMMO_MAX;
        reload_flag = 0;
    }
}

void fire_weapon() {
    double theta, x, y, theta2;
    x = enemy.vec[0] - p.vec[0];
    y = enemy.vec[1] - p.vec[1];
    theta = atan2(y, x);
    theta2 = theta - p.angle;
    if (theta2 > M_PI) {
        theta2 -= M_PI * 2;
    }
    if (theta2 < -M_PI) {
        theta += M_PI * 2;
    }
    if (fabs(theta2) < fabs(M_PI / 180 / 2 * hit_angle)) {
        enemy.HP -= hit_DMG;
        hit_flag = 1;
    }
    // printf("%2.3f\n", (theta2 - M_PI / 180 / 2 * hit_angle)*180/M_PI);
    p.ammo--;
}

