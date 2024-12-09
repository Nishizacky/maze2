#ifndef UTILITY_H
#define UTILITY_H

#include <pthread.h>

// 迷路の大きさを決定する
#define MEIRO_WIDTH 7  // 奇数にすること
#define MEIRO_HEIGHT 7 // 上に同じく

#define WALL 0
#define PATH 1
#define START 2
#define GOAL 3
extern int maze[MEIRO_HEIGHT][MEIRO_WIDTH]; // maze の定義を他のファイルに移動し、ここでは宣言

#define NOW_TIME clock()
#define FIRE_FRUSH_LEN 0.2

// プレイヤーデータ
#define UnitRayLength 3                           // 視野の距離
#define RayAmount 300                             // 光線の数
#define FOV 60                                    // 視野角
#define FOV_RAD (FOV / 180 * M_PI)                // ラジアン版
#define hit_angle 10                              // 銃の命中判定
#define hit_DMG 2                                 // 銃撃のダメージ
#define UnitRayAngle M_PI / 180 * FOV / RayAmount // 光線の間隔
#define UnitMoveLength 0.035                      // 移動距離
#define UnitTurnAngle 2.5                         // 角度の代わり具合
#define FPS 60                                    // フレームレート
#define flamerate 1000000 / FPS                   // usleep用
#define MAP_TIMER 8                               // マップの表示時間
#define AMMO_MAX 30                               // 弾倉あたりの弾丸数
#define RELOAD_TIME 1                             // リロード時間[s]
extern double fire_timer;
extern int DMG_flag;
extern int reload_flag;
extern int hit_flag;
extern double beat_ct;
extern double map_ct;
extern double reload_timer;
extern double dmg_timer;

#define ENEMY_HP 15
#define ENEMY_SPEED 0.05                   // 敵の移動速度
#define ENEMY_RESPONE_DELAY 7              // 次の敵が湧く時間[s]
#define ENEMY_CLOCK_DELAY 0.75             // 敵が巡回するときの頻度[us]
#define ENEMY_SEARCH_RANGE 3               // 敵の視野の長さ
#define ENEMY_SEARCH_ANGLE M_PI / 180 * 60 // 敵の視野角
#define ENEMY_REACH 0.1                    // 攻撃範囲
#define ENEMY_DMG 40                       // 敵の攻撃力
#define DESPONE_LANGE 6                    // デスポーン距離
extern int detect_flag;

extern pthread_t pthread, draw_thread;
extern int pthread_kill;

struct player {
    double vec[2];
    double angle;
    int HP;
    int ammo;
    int warp;
};

extern struct player p, enemy;

void main_menu();
int mazemaker(void);
void fire_weapon();
void action();
void reload_weapon();
void *draw();
void *func_enemy();
void init_variable();

#endif // UTILITY_H
