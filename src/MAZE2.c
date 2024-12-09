/*
発想：CSVファイルを用いたグラフィック情報とマップのインポート。
構造体からアイテムを利用する。これもCSVファイルでやり取り。
セーブデータの実装。txtファイルによってやりとり。
*/
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "utility.h"
#include "winlib_db.h"

extern struct player p, enemy;

void main() {
    initwin();
    init_rgb_greens();
    main_menu();
    mazemaker();
    while (1) {
        init_variable();
        pthread_create(&pthread, NULL, &func_enemy, NULL);
        pthread_create(&draw_thread, NULL, &draw, NULL);
        action();
        sleep(1);
        if (p.HP > 0) {
            gameclear();
            while (1) {
                if (getchar() == '\n') {
                    exit(1);
                }
            }
        } else {
            gameover();
            while (1) {
                if (getchar() == '\n')
                    break;
            }
        }
    }
}

void init_variable() {
    p.vec[0] = 1.5;
    p.vec[1] = 1.5;
    p.angle = 0;
    p.HP = 99;
    p.ammo = AMMO_MAX;
    p.warp = 3;
    enemy.vec[0] = -10;
    enemy.vec[1] = -10;
    enemy.HP = ENEMY_HP;
    enemy.angle = 0;
    fire_timer = -100;
    DMG_flag = 0;
    reload_flag = 0;
    hit_flag = 0;
    beat_ct = 0;
    map_ct = 0;
    reload_timer = 0;
}