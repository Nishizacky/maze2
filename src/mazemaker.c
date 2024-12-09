//生成⇒配列⇒MAZE2へインポート
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "utility.h"

/* 迷路の各マス表す値の定義⇒utility.hへ移動 */
// #define PATH 1
// #define WALL 0
// #define START 2
// #define GOAL 3

/* 方向を表す値の定義 */
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

extern int maze[MEIRO_HEIGHT][MEIRO_WIDTH];

void initRand();              //乱数シード生成
void printMaze(int w, int h); // CSVファイルへ出力
void startup(int i, int j);   //開始地点の決定
void dig(int w, int h);       //掘る

int mazemaker() {
    initRand();
    startup(MEIRO_WIDTH, MEIRO_HEIGHT);
    printMaze(MEIRO_WIDTH, MEIRO_HEIGHT);
    return 0;
}

void initRand() {
    time_t now;
    now = time(NULL);
    srand((unsigned int)now);
}

void printMaze(int w, int h) {
    int i, j;
    FILE *fp;
    fp = fopen("mapdata.csv", "w");

    for (j = 0; j < h; j++) {
        for (i = 0; i < w - 1; i++) {
            /* 配列の値に応じて記号/文字を表示 */
            if (maze[j][i] == PATH) {
                fprintf(fp, "%d,", PATH);
            }
            if (maze[j][i] == WALL) {
                fprintf(fp, "%d,", WALL);
            }
            if (maze[j][i] == GOAL) {
                fprintf(fp, "%d,", GOAL);
            }
        }
        fprintf(fp, "%d\n", WALL);
    }
    fclose(fp);
}

void dig(int i, int j) {
    /* どの方向を掘ろうとしたかを覚えておく変数 */
    int up, down, left, right;

    up = 0;
    down = 0;
    left = 0;
    right = 0;

    /* 全方向で一度も掘ることができなければ終了 */
    while (up == 0 || down == 0 || left == 0 || right == 0) {
        int d = rand() % 4; //どの方向へ掘るかランダムに決定する

        switch (d) {
        case UP:
            if (j - 2 >= 0 && j - 2 < MEIRO_HEIGHT) {
                if (maze[j - 2][i] == WALL) {
                    maze[j - 2][i] = PATH;
                    maze[j - 1][i] = PATH;
                    dig(i, j - 2);
                }
            }
            up++;
            break;
        case DOWN:
            if (j + 2 >= 0 && j + 2 < MEIRO_HEIGHT) {
                if (maze[j + 2][i] == WALL) {
                    maze[j + 2][i] = PATH;
                    maze[j + 1][i] = PATH;
                    dig(i, j + 2);
                }
            }
            down++;
            break;
        case LEFT:
            if (i - 2 >= 0 && i - 2 < MEIRO_WIDTH) {
                if (maze[j][i - 2] == WALL) {
                    maze[j][i - 2] = PATH;
                    maze[j][i - 1] = PATH;
                    dig(i - 2, j);
                }
            }
            left++;
            break;
        case RIGHT:
            if (i + 2 >= 0 && i + 2 < MEIRO_WIDTH) {
                if (maze[j][i + 2] == WALL) {
                    maze[j][i + 2] = PATH;
                    maze[j][i + 1] = PATH;
                    dig(i + 2, j);
                }
            }
            right++;
            break;
        }
    }
}

void startup(int w, int h) {
    int i, j;

    /* 初期化（すべて壁にする） */
    for (j = 0; j < h; j++) {
        for (i = 0; i < w; i++) {
            maze[j][i] = WALL;
        }
    }

    /* ゴールをランダム（奇数座標）に決定する */
    while (1) {
        i = 2 * (rand() % (MEIRO_WIDTH / 2)) + 1;
        j = 2 * (rand() % (MEIRO_HEIGHT / 2)) + 1;
        if (i != 1 || j != 1) {
            maze[j][i] = GOAL;
            break;
        }
    }
    dig(i, j);
}