#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "utility.h"
#include "winlib_db.h"

#define DETECTED 1
#define SEARCHING 0

extern int maze[MEIRO_HEIGHT][MEIRO_WIDTH];
extern struct player p, enemy;
extern int detect_flag;

void respone_enemy();
void active_enemy();
void search_player();
void seak_player();
void wondering();



void *func_enemy() {
    while (1) {
        if (enemy.vec[0] < 0 && enemy.vec[1] < 0) {
            sleep(ENEMY_RESPONE_DELAY);
            respone_enemy();
        } else {
            active_enemy();
        }
        if (p.HP < 0 || pthread_kill == 1) {
            break;
        }
        usleep(100000 / FPS);
    }
}

void respone_enemy() {
    //プレイヤーの近くに湧くように設定する
    int i, j;
    while (1) {
        i = (int)p.vec[0] + (1 - rand() % 3);
        j = (int)p.vec[1] + (1 - rand() % 3);
        if (maze[i][j] == PATH) {
            enemy.vec[0] = i + 0.5;
            enemy.vec[1] = j + 0.5;
            break;
        }
    }
}

void active_enemy() {
    while (1) {
        // search_player();
        seak_player();
        if (enemy.HP < 0) {
            enemy.vec[0] = -10;
            enemy.vec[1] = -10;
            enemy.HP = ENEMY_HP;
            map_ct = clock();
            beat_ct = clock();
            break;
        }
    }
}

void search_player() {
    double dx, dy;
    dx = p.vec[0] - enemy.vec[0];
    dy = p.vec[1] - enemy.vec[1];
    if (dx * dx + dy * dy < ENEMY_SEARCH_RANGE * ENEMY_SEARCH_RANGE) {
        if (fabs(atan(dy / dx) - enemy.angle) < ENEMY_SEARCH_ANGLE / 2) {
            detect_flag = 1;
            seak_player();
        } else {
            wondering();
        }
    } else if (dx * dx + dy * dy > DESPONE_LANGE * DESPONE_LANGE) {
        enemy.HP = -1;
    }
}

void seak_player() {
    double vec_x, vec_y;
    vec_x = p.vec[0] - enemy.vec[0];
    vec_y = p.vec[1] - enemy.vec[1];
    enemy.angle = atan2(vec_y, vec_x);

    enemy.vec[0] += ENEMY_SPEED * cos(enemy.angle);
    enemy.vec[1] += ENEMY_SPEED * sin(enemy.angle);
    if (vec_x * vec_x + vec_y * vec_y < ENEMY_REACH * ENEMY_REACH) {
        p.HP -= ENEMY_DMG + (7 - rand() % 15); //攻撃
        DMG_flag = 1;
        dmg_timer = clock();
        sleep(5);
    }
    if (vec_x * vec_x + vec_y * vec_y > DESPONE_LANGE * DESPONE_LANGE) {
        enemy.HP = -1;
    }
    usleep(600000);
}

void wondering() {
    enemy.angle += M_PI / 12;
    double l;
    int nx, ny;
    l = 0;
    while (1) {
        nx = enemy.vec[0] + l * cos(enemy.angle);
        if (maze[nx][(int)enemy.vec[1]] != WALL) {
            enemy.vec[0] += l * cos(enemy.angle);
        }
        if (l > ENEMY_SPEED) {
            break;
        }
        l += 0.05;
    }
    l = 0;
    while (1) {
        //切り捨て問題：マイナス値のとき、都合の悪い切り捨てられ方をされるため回避
        if (enemy.vec[1] + l * sin(enemy.angle) < 0) {
            ny = -(enemy.vec[1] + l * sin(enemy.angle));
            ny *= -1;
        } else {
            ny = enemy.vec[1] + l * sin(enemy.angle);
        }

        if (maze[(int)enemy.vec[0]][ny] != WALL) {
            enemy.vec[1] += l * sin(enemy.angle);
        }
        if (l > ENEMY_SPEED) {
            break;
        }
        l += 0.05;
    }
}