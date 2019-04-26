/**
 * @file game.c
 * @brief ゲーム本体
 * @date  2019.04.20 作成
 * @author Choe Gyun(choikyun)
 */

/***************************************************
 * Starbow
 * ver 1.0.0
 * 2019.04.19
 * Choe Gyun (choikyun)
 *****************************************************/

#define GLOBAL_VALUE_DEFINE

#include <gba.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "bg.h"
#include "game.h"
#include "graph.h"
#include "music.h"
#include "score.h"
#include "sprite.h"

void init_game();
static void
init_ship();
static void
init_stars();
static void
move_ship();
static void
disp_ship();
static void
disp_stars();
static void
disp_fire();
static void
draw_bg();
static void
init_sprite_setting();
static void
init_stage();
static void
move_stars();
static void
check_stage_boundary();

//debug
void vbaPrint(char* s);

/**********************************************/ /**
 * @brief メインループ
 ***********************************************/
void game()
{
    game_state.key = keysDown();
    game_state.keyr = keysDownRepeat();

    seed++;

    switch (game_state.scene) {
    case GAME_TITLE:
        //disp_title();
        //select_mode();
        break;

    case GAME_DEMO:
        break;

    case GAME_MAIN:
        draw_bg();

        move_ship();
        move_stars();

        disp_ship();
        disp_fire();
        disp_stars();
        break;

    case GAME_READY:
        break;

    case GAME_PAUSE:
        break;

    case GAME_OVER:
        break;
    }
}

/**********************************************/ /**
 * @brief ゲーム初期化
 ***********************************************/
void init_game()
{
    // ゲームステート初期化
    game_state.scene = GAME_MAIN;
    stage.mode = 0;

    // パラメータ初期化
    score = 0;
    stage.lv = 1;

    // ハイスコアのロード
    //hiscore = load_hiscore();

    // スプライト初期化
    init_sprite_setting();

    // 自機初期化
    init_ship();

    // スター初期化
    init_stars();

    // ステージ初期化
    init_stage();
}

/**********************************************/ /**
 * @brief スプライト初期化
 ***********************************************/
static void
init_sprite_setting()
{
    //// 自機 32*32 dot
    set_sprite_form(SPRITE_SHIP, OBJ_SIZE(2), OBJ_SQUARE, OBJ_256_COLOR);
    set_sprite_tile(SPRITE_SHIP, TILE_SHIP1);

    //// 炎 16*16 dot
    set_sprite_form(SPRITE_FIRE, OBJ_SIZE(1), OBJ_SQUARE, OBJ_256_COLOR);
    set_sprite_tile(SPRITE_FIRE, TILE_FIRE1);

    //// スター 64*64 dot
    for (int i = 0; i < MAX_STARS; i++) {
        set_sprite_form(SPRITE_STAR + i, OBJ_SIZE(3), OBJ_SQUARE, OBJ_256_COLOR);
        set_sprite_tile(SPRITE_STAR + i, TILE_STAR2);
        set_affine_setting(SPRITE_STAR + i, i, 0);
        set_scale(i, 100, 100);
    }
}

/**********************************************/ /**
 * @brief リスタート
 ***********************************************/
static void
restart()
{
}

/**********************************************/ /**
 * @brief ステージ初期化
 ***********************************************/
static void
init_stage()
{

    // テスト
    // スター作成
    stars.list[0].vec.x = -60;
    stars.list[0].vec.y = 10;
    stars.list[0].vec.z = 100 << FIX;
    stars.list[0].center.x = STAR_W / 2 - 1;
    stars.list[0].center.y = STAR_H / 2 - 1;
    stars.num = 1;

    // ステージの座標
    stage.center.x = 0;
    stage.center.y = 0;
}

/**********************************************/ /**
 * @brief 自機初期化
 ***********************************************/
static void
init_ship()
{
    // 加速度　自機はZ未使用
    ship.acc.x = ship.acc.y = 0;

    // 自機スプライト
    ship.chr = SPRITE_SHIP;
    ship.vec.x = SHIP_X << FIX;
    ship.vec.y = SHIP_Y << FIX;
    ship.vec.z = SHIP_Z << FIX;
    ship.center.x = SHIP_W / 2;
    ship.center.y = SHIP_H / 2;

    // 炎 スプライト
    fire.sprite.chr = SPRITE_FIRE;
    fire.sprite.vec.x = FIRE_X << FIX;
    fire.sprite.vec.y = FIRE_Y << FIX;
    fire.sprite.vec.z = FIRE_Z << FIX;
    fire.sprite.center.x = FIRE_W / 2;
    fire.sprite.center.y = FIRE_H / 2;

    // 炎 アニメーション
    fire.anime.frame = 0;
    fire.anime.max_frame = 2;
    fire.anime.interval = fire.anime.interval_rel = FIRE_INTERVAL;
    fire.anime.is_start = true;
}

/**********************************************/ /**
 * @brief スター初期化
 ***********************************************/
static void
init_stars()
{
    for (int i = 0; i < MAX_STARS; i++) {
        // 加速度
        stars.list[i].acc.x = stars.list[i].acc.y = stars.list[i].acc.z = 0;

        // スプライト
        stars.list[i].chr = SPRITE_STAR + i;
        stars.list[i].vec.x = 0;
        stars.list[i].vec.y = 0;
        stars.list[i].vec.z = MIN_Z << FIX;
    }

    stars.num = 0;
}

/**********************************************/ /**
 * @brief 3D座標からデバイス座標に変換 右手座標系
 * 
 * @param *v 3D座標 (-119)-(119), (-79)-(79), (1)-(100) 補正済みのこと
 * @param cx X中心のオフセット
 * @param cy Y中心のオフセット
 ***********************************************/
static void
trans_device_coord(VectorType* v, int cx, int cy)
{
    // スプライトのスケールを決定 1-100%
    v->scale = (v->z * 100) / MAX_Z;

    // XY座標の変換
    v->x = ((v->x * v->z) / MAX_Z) + FIX_STAGE_X - cx;
    v->y = ((v->y * v->z) / MAX_Z) + FIX_STAGE_Y - cy;
}

/**********************************************/ /**
 * @brief 自機移動
 ***********************************************/
static void
move_ship()
{
    u16 key = game_state.keyr;

    // デフォルトタイル
    set_sprite_tile(SPRITE_SHIP, TILE_SHIP1);

    // 4方向移動
    if (key & KEY_UP) {
        ship.acc.y -= SHIP_SPEED;
        if (ship.acc.y < -MAX_SHIP_ACC) {
            ship.acc.y = -MAX_SHIP_ACC;
        }
        set_sprite_tile(SPRITE_SHIP, TILE_SHIP4);
    } else if (key & KEY_DOWN) {
        ship.acc.y += SHIP_SPEED;
        if (ship.acc.y > MAX_SHIP_ACC) {
            ship.acc.y = MAX_SHIP_ACC;
        }
        set_sprite_tile(SPRITE_SHIP, TILE_SHIP5);
    }

    if (key & KEY_LEFT) {
        ship.acc.x -= SHIP_SPEED;
        if (ship.acc.x < -MAX_SHIP_ACC) {
            ship.acc.x = -MAX_SHIP_ACC;
        }
        set_sprite_tile(SPRITE_SHIP, TILE_SHIP3);
    } else if (key & KEY_RIGHT) {
        ship.acc.x += SHIP_SPEED;
        if (ship.acc.x > MAX_SHIP_ACC) {
            ship.acc.x = MAX_SHIP_ACC;
        }
        set_sprite_tile(SPRITE_SHIP, TILE_SHIP2);
    }

    // 自然減速
    if (!(key & (KEY_UP | KEY_DOWN | KEY_LEFT | KEY_RIGHT))) {
        ship.acc.x += -ship.acc.x * SHIP_FRIC;
        ship.acc.y += -ship.acc.y * SHIP_FRIC;
    }

    // 自機は中央固定なのでステージ座標が変化する
    // 自機とは逆方向に加速
    stage.center.x -= ship.acc.x;
    stage.center.y -= ship.acc.y;
    
    // 境界判定
    check_stage_boundary();
}

/**********************************************/ /**
 * @brief ステージ境界チェック
 * 
 * @param *tx ターゲットX座標
 * @param *ty ターゲットY座標
 ***********************************************/
static void
check_stage_boundary()
{
    int x = stage.center.x >> FIX;
    int y = stage.center.y >> FIX;

    if (x < SHIP_MOVE_MIN_X) {
        stage.center.x = SHIP_MOVE_MIN_X << FIX;
        ship.acc.x /= -2;// 加速度反転
    } else if (x > SHIP_MOVE_MAX_X) {
        stage.center.x = SHIP_MOVE_MAX_X << FIX;
        ship.acc.x /= -2;
    }

    if (y < SHIP_MOVE_MIN_Y) {
        stage.center.y = SHIP_MOVE_MIN_Y << FIX;
        ship.acc.y /= -2;
    } else if (y > SHIP_MOVE_MAX_Y) {
        stage.center.y = SHIP_MOVE_MAX_Y << FIX;
        ship.acc.y /= -2;
    }
}

/**********************************************/ /**
 * @brief スター移動
 ***********************************************/
static void
move_stars()
{
    for (int i = 0; i < stars.num; i++) {
        // Z方向の移動
    }
}

/**********************************************/ /**
 * @brief 自機表示
 ***********************************************/
static void
disp_ship()
{
    VectorType v = ship.vec;
    v.x >>= FIX;
    v.y >>= FIX;
    v.z >>= FIX;

    // 座標変換
    // 自機は中央固定
    trans_device_coord(&v, ship.center.x, ship.center.y);

    move_sprite(ship.chr, v.x, v.y);
}

/**********************************************/ /**
 * @brief 炎表示
 ***********************************************/
static void
disp_fire()
{
    // アニメ
    if (fire.anime.is_start && --fire.anime.interval < 0) {
        fire.anime.interval = fire.anime.interval_rel;
        fire.anime.frame = (fire.anime.frame + 1) % fire.anime.max_frame;
        // タイル切り替え
        set_sprite_tile(SPRITE_FIRE, TILE_FIRE1 + (fire.anime.frame * TILE_SIZE_16));
    }

    VectorType v = fire.sprite.vec;
    v.x >>= FIX; v.y >>= FIX; v.z >>= FIX;

    // 座標変換
    // 炎は中央固定
    trans_device_coord(&v, fire.sprite.center.x, fire.sprite.center.y);

    move_sprite(fire.sprite.chr, v.x, v.y);
}

/**********************************************/ /**
 * @brief スター表示
 ***********************************************/
static void
disp_stars()
{
    VectorType v;

    for (int i = 0; i < stars.num; i++) {
        v = stars.list[i].vec;
        v.x >>= FIX; // i8:f8
        v.y >>= FIX; // i8:f8
        v.z >>= FIX; // i8:f8

        // 座標変換
        trans_device_coord(&v, stars.list[i].center.x, stars.list[i].center.y);
        // スケールを設定
        set_scale(stars.list[i].aff, v.scale, v.scale);

        // ステージ座標の補正
        move_sprite(
            stars.list[i].chr,
            v.x + (stage.center.x >> FIX),
            v.y + (stage.center.y >> FIX));
    }
}

/**********************************************/ /**
 * @brief ステージのBG描画
 ***********************************************/
static void
draw_bg()
{
    flip_frame();
    load_frame_bitmap(DEF_BG_BITMAP);
}

/**********************************************/ /**
 * @brief ポーズ
 ***********************************************/
static void
pause()
{
    /*
    u16 key = game_state.key;

    if (key & KEY_SELECT)
    {
        game_state.scene ^= GAME_PAUSE;
        if (game_state.scene & GAME_PAUSE)
        {
            StopMusic();
            PlaySound(SOUND_ITEM);
        }
        else
        {
            PlayMusic(MUSIC_STAGE + stage_bgm, true);
        }
    }
    */
}

/**********************************************/ /**
 * @brief スコア表示
 ***********************************************/
static void
update_score()
{
    /*
    int i;
    int pos = SCORE_DIGIT * NUM_W - NUM_W;
    int sc = score;

    for (i = 0; i < SCORE_DIGIT; i++)
    {
        disp_num(SCORE_X + pos, SCORE_Y, sc % 10);
        sc /= 10;
        pos -= (NUM_W);
    }
    */
}

/**********************************************/ /**
 * @brief ハイスコア表示
 ***********************************************/
void update_hiscore()
{
    /*
    int i;
    int pos = SCORE_DIGIT * NUM_W - NUM_W;
    int sc = hiscore;

    for (i = 0; i < SCORE_DIGIT; i++)
    {
        disp_num_title(HISCORE_X + pos, HISCORE_Y, sc % 10);
        sc /= 10;
        pos -= (NUM_W);
    }
    */
}

/**********************************************/ /**
 * @brief ワーニングメッセージ表示
 ***********************************************/
static void
disp_warning()
{
}

/**********************************************/ /**
 * @brief ポーズメッセージ表示
 ***********************************************/
static void
disp_pause()
{
}

/**********************************************/ /**
 * @brief 点滅メッセージ用パラメータのリセット
 * @param *m メッセージパラメータポインタ
 ***********************************************/
static void
reset_message(BlinkMessageType* m)
{
}

/**********************************************/ /**
 * @brief 点滅メッセージ用パラメータのリセット
 * @param *m メッセージパラメータポインタ
 ***********************************************/
static void
reset_message_fast(BlinkMessageType* m)
{
}

/**********************************************/ /**
 * @brief BGに数字表示
 * @param x X座標
 * @param y Y座標
 * @param num 表示する数値
 ***********************************************/
static void
disp_num(int x, int y, u16 num)
{
}

/**********************************************/ /**
 *  タイトル ビットマップ転送
 ***********************************************/
void load_title()
{
}

//// デバグ用
//THUMB code
void vbaPrint(char* s)
{
    /*
    asm volatile("mov r0, %0;"
                 "swi 0xff;"
                 : // no ouput
                 : "r"(s)
                 : "r0");
    */
}
