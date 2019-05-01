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
disp_title();
static void
init_ship();
static void
init_stars();
static void
init_star(int, int, int);
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
static void
create_new_stars();

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
        disp_title();
        //select_mode();
        break;

    case GAME_DEMO:
        break;

    case GAME_MAIN:
        draw_bg();

        create_new_stars();

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
    game_state.scene = GAME_TITLE;
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
 * @brief スター作成
 ***********************************************/
static void
create_new_stars()
{
    // 一定間隔で出現
    if (--stars.interval <= 0) {
        for (int i = 0; i < RND(1, APPER_MAX_STARS); i++) {
            if (stars.num < MAX_STARS) {
                if (!RND(0, 3)) {
                    init_star(
                        stars.num++,
                        // 自機周辺をねらう
                        (ship.vec.x >> FIX) + RND(0, SHIP_W * 2) - SHIP_W,
                        STAR_TARGET_Y);
                } else {
                    init_star(
                        stars.num++,
                        RND(0, SCREEN_WIDTH) - SCREEN_WIDTH / 2,
                        STAR_TARGET_Y);
                }
            }
        }

        // 次の出現間隔
        stars.interval = STAR_INTERVAL;
    }
}

/**********************************************/ /**
 * @brief スター移動
 ***********************************************/
static void
move_stars()
{
    int cur, end;
    OBJATTR* sp = OAM + SPRITE_STAR;

    if (!stars.num) {
        return;
    }

    // 後ろからバッファを操作
    cur = end = stars.num - 1;
    for (int i = stars.num; i > 0; i--, cur--) {
        
        // 加速
        /*stars.list[cur].acc.z += STAR_SPEED;
        if (stars.list[cur].acc.z > STAR_MAX_ACC) {
            stars.list[cur].acc.z = STAR_MAX_ACC;
        }*/
        stars.list[cur].vec.z += stars.list[cur].acc.z;

        if ((stars.list[cur].vec.z >> FIX) < MIN_Z) {

            // 消去 データとスプライト
            CpuSet(
                &stars.list[end],
                &stars.list[cur],
                (COPY32 | (sizeof(SpriteCharType) / 4)));
            CpuSet (sp + end, sp + cur, (COPY32 | sizeof(OBJATTR) / 4));

            erase_sprite(SPRITE_STAR + end);
            stars.num--;
            end--;
        }
    }
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

    //// スター 64*32 dot
    for (int i = 0; i < MAX_STARS; i++) {
        set_sprite_form(SPRITE_STAR + i, OBJ_SIZE(Sprite_64x32), OBJ_TALL, OBJ_256_COLOR);
        set_sprite_tile(SPRITE_STAR + i, TILE_STAR1);
        set_affine_setting(SPRITE_STAR + i, i, 0);
        set_scale(i, 1, 1);
    }
}

/**********************************************/ /**
 * @brief リスタート
 ***********************************************/
static void
restart()
{
  game_state.scene = GAME_MAIN;

    // ランダマイズ
    srand(REG_TM0CNT + seed);
    // seedをSRAMに保存
    SRAMWrite32(SRAM_SEED, seed);

    // ステージ初期化
    init_stage ();
}

/**********************************************/ /**
 * @brief ステージ初期化
 ***********************************************/
static void
init_stage()
{
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
    ship.target.x = SHIP_X;
    ship.target.y = SHIP_Y;

    // 炎 スプライト
    fire.sprite.chr = SPRITE_FIRE;
    fire.sprite.vec.x = FIRE_X << FIX;
    fire.sprite.vec.y = FIRE_Y << FIX;
    fire.sprite.vec.z = FIRE_Z << FIX;
    fire.sprite.center.x = FIRE_W / 2;
    fire.sprite.center.y = FIRE_H / 2;
    fire.sprite.target.x = FIRE_X;
    fire.sprite.target.y = FIRE_Y;


    // 炎 アニメーション
    fire.anime.frame = 0;
    fire.anime.max_frame = 2;
    fire.anime.interval = fire.anime.interval_rel = FIRE_INTERVAL;
    fire.anime.is_start = true;
}

/**********************************************/ /**
 * @brief すべてのスター初期化
 ***********************************************/
static void
init_stars()
{
    for (int i = 0; i < MAX_STARS; i++) {
        // スプライトの割当
        stars.list[i].chr = SPRITE_STAR;
        stars.list[i].center.x = STAR_W / 2;
        stars.list[i].center.y = STAR_H / 2;
    }

    stars.interval = 1;
    stars.num = 0;
}

/**********************************************/ /**
 * @brief スター初期化
 ***********************************************/
static void
init_star(int num, int x, int y)
{
    // 加速度
    stars.list[num].acc.z = STAR_SPEED;

    // スプライト
    stars.list[num].target.x = x; // X座標目標値
    stars.list[num].target.y = y; // Y座標目標値
    stars.list[num].vec.z = MAX_Z << FIX;
}

/**********************************************/ /**
 * @brief 3D座標からデバイス座標に変換
 * 
 * @param *v 3D座標
 * @param *t ターゲット座標
 * @param *c 中心のオフセット
 ***********************************************/
static void
trans_device_coord(VectorType* v, PointType* t, PointType* c)
{
    // XY座標の変換

    // sc1 - sc0 でスケールを求める
    float sc0 = (((float)t->x / (float)v->z) + 0.5) * MIN_Z;
    float sc1 = ((((float)t->x + STAR_W) / v->z) + 0.5) * MIN_Z;
    
    v->x = sc0 + FIX_STAGE_X - c->x;
    v->y = (((float)t->y / (float)v->z) + 0.5) * MIN_Z + FIX_STAGE_Y - c->y;

    v->scale = ((sc1 - sc0) / STAR_W) * 100; 
    if (v->scale <= 0) {
        v->scale = 1;
    }
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

    // デフォルトの炎の座標
    fire.sprite.vec.x = FIRE_X << FIX;
    fire.sprite.vec.y = FIRE_Y << FIX;

    // 左右向移動
    if (key & KEY_LEFT) {
        ship.acc.x -= SHIP_SPEED;
        if (ship.acc.x < -MAX_SHIP_ACC) {
            ship.acc.x = -MAX_SHIP_ACC;
        }
        set_sprite_tile(SPRITE_SHIP, TILE_SHIP3);
        fire.sprite.vec.x = FIRE_X_LEFT << FIX;
    } else if (key & KEY_RIGHT) {
        ship.acc.x += SHIP_SPEED;
        if (ship.acc.x > MAX_SHIP_ACC) {
            ship.acc.x = MAX_SHIP_ACC;
        }
        set_sprite_tile(SPRITE_SHIP, TILE_SHIP2);
        fire.sprite.vec.x = FIRE_X_RIGHT << FIX;
    }

    // 自然減速
    if (!(key & (KEY_UP | KEY_DOWN | KEY_LEFT | KEY_RIGHT))) {
        ship.acc.x += -ship.acc.x * SHIP_FRIC;
        ship.acc.y += -ship.acc.y * SHIP_FRIC;
    }

    ship.vec.x += ship.acc.x;
    ship.vec.y += ship.acc.y;

    fire.sprite.vec.x = ship.vec.x;

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
    int x = ship.vec.x >> FIX;
    int y = ship.vec.y >> FIX;

    if (x < SHIP_MOVE_MIN_X) {
        ship.vec.x = SHIP_MOVE_MIN_X << FIX;
        ship.acc.x /= -2; // 加速度反転
    } else if (x > SHIP_MOVE_MAX_X) {
        ship.vec.x = SHIP_MOVE_MAX_X << FIX;
        ship.acc.x /= -2;
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
    trans_device_coord(&v, &v, &ship.center);

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
    v.x >>= FIX;
    v.y >>= FIX;
    v.z >>= FIX;

    // 座標変換
    // 炎は中央固定
    trans_device_coord(&v, &v, &fire.sprite.center);

    move_sprite(fire.sprite.chr, v.x, v.y);
}

/**********************************************/ /**
 * @brief スター表示
 ***********************************************/
static void
disp_stars()
{
    VectorType v;
    PointType t, c;

    for (int i = 0; i < stars.num; i++) {
        v = stars.list[i].vec;
        v.x >>= FIX; // i8:f8
        v.y >>= FIX;
        v.z >>= FIX;

        // 座標変換
        trans_device_coord(&v, &stars.list[i].target, &stars.list[i].center);

        // アフィンパラメータ設定
        set_affine_setting(SPRITE_STAR + i, i, 0);
        // スケールを設定
        set_scale(i, v.scale, v.scale);

        // ステージ座標の補正
        move_sprite(
            stars.list[i].chr + i,
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

/**********************************************//**
 * @brief タイトル
 ***********************************************/
static void
disp_title ()
{
  u16 key = game_state.key;

  if (key & KEY_START)
  {
    restart ();
    stage.demo = false;
  //  StopMusic ();
  }
  else if ((key & KEY_R) && (key & KEY_B))
  {
  //  clear_hiscore ();
  //  update_hiscore ();
  }
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
