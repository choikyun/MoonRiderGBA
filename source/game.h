/**
 * @file game.h
 * @brief ゲーム本体のヘッダ
 * @date  2017/02/01 更新
 * @author Choe Gyun(choikyun)
 */

/***************************************************
 * Moon Defender GBA
 * ver 1.0.0
 * 2017.02.01
 * Choe Gyun (choikyun)
 *****************************************************/

#ifndef _game_h_
#define _game_h_

#include <gba.h>
#include "bg.h"

#ifdef GLOBAL_VALUE_DEFINE
#define GLOBAL
#define GLOBAL_VAL(v) = (v)
#else
#define GLOBAL extern
#define GLOBAL_VAL(v)
#endif

///////////////////////////////////////////////////////////////////// 各種定義

/***************************************************
 * 汎用的な定義
 ***************************************************/

/**
 * 固定小数点部分 16bit
 */
#define FIX (16)

/**
 * ステージ幅　最小
 */
#define STAGE_W_MIN (0)
/**
 * ステージ幅　最大
 */
#define STAGE_W_MAX (SCREEN_WIDTH - 16)
/**
 * ステージ高さ　最小
 */
#define STAGE_H_MIN (8)
/**
 * ステージ高さ　最大
 */
#define STAGE_H_MAX (SCREEN_HEIGHT - 32 - 16)

/**
 * 数字　幅
 */
#define NUM_W (8)
/**
 * 数字　高さ
 */
#define NUM_H (8)

/**
 * 数字の実際の幅
 *
 * MODE4では16bit単位で書き込むため
 */
#define REAL_NUM_W (NUM_W / 2)

/**
 * キー入力　長さ
 */
#define DEF_KEY_DELAY (1)
/**
 * キー入力 リピート
 */
#define DEF_KEY_REPEAT (1)

/**
 * メッセージ表示ウェイト
 */
#define MES_WAIT (20)

/**
 * メッセージ点滅カウント
 */
#define MES_COUNT (10)

/**
 * 一般オブジェクト　幅
 */
#define OBJ_W (32)
/**
 * 一般オブジェクト　高さ
 */
#define OBJ_H (32)

/***************************************************
 * ゲーム個別
 ***************************************************/

/***************************************************
 * モード
 ***************************************************/

/**
 * ゲームモード数
 */
#define MAX_MODE (2)

/***************************************************
 * ステージ
 ***************************************************/

/**
 * Z座標 最大値
 */
#define MAX_Z (100)
/**
 * Z座標 最小値
 */
#define MIN_Z (1)

/***************************************************
 * 自機 スプライト
 ***************************************************/

/**
 * 自機 幅
 */
#define SHIP_W (32)
/**
 * 自機 高さ
 */
#define SHIP_H (32)

/**
 * 自機 X座標
 */
#define SHIP_X (SCREEN_WIDTH / 2 - SHIP_W / 2)

/**
 * 自機 Y座標
 */
#define SHIP_Y ((SCREEN_HEIGHT - SHIP_H) / 2 - SHIP_H / 2) + 32

/**
 * 自機 Y座標
 */
#define SHIP_Z MAX_Z

/**
 * 自機 速度
 */
#define SHIP_SPEED (1)

/***************************************************
 * スター スプライト
 ***************************************************/

/**
 * スター 幅
 */
#define STAR_W (64)
/**
 * スター 高さ
 */
#define STAR_H (64)

/**
 * スター最大数
 */
#define MAX_STARS (16)

/***************************************************
 * フラッシュ
 ***************************************************/

/**
 * フラッシュ　間隔
 */
#define FLASH_INTERVAL (4)

/**
 * フラッシュ　色
 */
#define FLASH_COLOR (15)

/***************************************************
 *　スコア
 ***************************************************/

/**
 * スコア加算のための高度間隔
 */
#define SCORE_HEIGHT_RANGE (32)

/**
 * スコア表示桁
 */
#define SCORE_DIGIT (8)

/**
 * スコア　X座標
 */
#define SCORE_X (88)
/**
 * スコア　Y座標
 */
#define SCORE_Y LV_Y
/**
 * ハイスコア　X座標
 */
#define HISCORE_X (70)
/**
 * ハイスコア　Y座標
 */
#define HISCORE_Y (64)

/***************************************************
 *　BGM
 ***************************************************/

/**
 * ステージBGM数
 */
#define MAX_STAGE_BGM (3)

///////////////////////////////////////////////////////////////////// クラス

/**
 * 座標 2d
 */
typedef struct
{
    int x;
    int y;
} ALIGN(4) PointType;

/**
 * 座標 3d
 */
typedef struct
{
    int x;
    int y;
    int z;
} ALIGN(4) VectorType;

/**
 * 振動
 */
typedef struct
{
    int range;
    int direc;
    int interval;
    int duration;
} ALIGN(4) ShockType;

/**
 * フラッシュ
 */
typedef struct
{
    u8 color[32];
    int interval;
} ALIGN(4) FlashType;

/**
 * ゲーム管理
 */
typedef struct
{
    int scene;
    int next_scene;
    int intermission; // 次のシーンまでの幕間
    u16 key;
    u16 keyr;
    int demo_start;
} ALIGN(4) GameStateType;

/**
 * ステージ管理
 */
typedef struct
{
    int lv;
    int mode;
    int cities;
    bool demo;

    FlashType flash;
} ALIGN(4) StageType;

/**
 * フレームアニメ
 */
typedef struct
{
    bool is_start;    // 開始
    int frame;        // 現在のフレーム
    int max_frame;    // フレーム最大値
    int interval;     // インターバル
    int interval_rel; // インターバル既定値
} ALIGN(4) AnimeType;

/**
 * スプライト
 */
typedef struct
{
    int chr;     /**< キャラクタ番号 */
    int tile;    /**< タイル番号 */
    VectorType vec; /**< 座標 */
    VectorType acc; /**< 加速度 */
    int cx;         /**< 中心X のオフセット */
    int cy;         /**< 中心Y のオフセット */
    int w;          /**< 高さ */
    int h;          /**< 幅 */
    bool scaling;   /**< 拡縮するか */
    int aff;     /**< アフィンパラメータ番号 0-31 */
    bool show;      /**< 表示フラグ */
} ALIGN(4) SpriteCharType;

/**
 * スター管理
 */
typedef struct
{
    int num;                        /**< 現在のスター数 */
    SpriteCharType list[MAX_STARS]; /**< スターのリスト */
} ALIGN(4) StarType;

/**
 * 点滅メッセージ
 */
typedef struct
{
    bool is_start;
    int x;
    int y;
    int wait;
    int wait_rel;
    int count;
    int chr;
} ALIGN(4) BlinkMessageType;

/**
 * ウェイト
 */
typedef struct
{
    int wait;
} ALIGN(4) WaitType;

///////////////////////////////////////////////////////////////////// ENUM

/**
 * シーン
 */
enum
{
    GAME_MAIN = 0,
    GAME_TITLE = 1,
    GAME_PAUSE = 2,
    GAME_OVER = 4,
    GAME_READY = 8,
    GAME_OVER_PRE = 16,
    GAME_CRASH = 32,
    GAME_CLEAR = 64,
    GAME_DEMO = 128,
};

/**
 * スプライトキャラクタ
 */
enum
{
    // 自機
    SPRITE_SHIP = 0,

    // スター
    SPRITE_STAR = 4 // -19 まで

} SpriteEnum;

///////////////////////////////////////////////////////////////////// タイル番号

#define TILE_SIZE_8 (2)
#define TILE_SIZE_16 (8)

// 自機
#define TILE_SHIP1 (512)
#define TILE_SHIP2 (544)
#define TILE_SHIP3 (576)

// スター
#define TILE_STAR (608)

///////////////////////////////////////////////////////////////////// SRAM

/**
 * ハイスコアSRAM保存用
 */
#define SRAM_CHECK (0)
/**
 * ハイスコアSRAM保存用
 */
#define SRAM_HISCORE_NORMAL (4)
/**
 * ハイスコアSRAM保存用
 */
#define SRAM_HISCORE_HARD (8)

/**
 * 乱数の種
 */
#define SRAM_SEED (12)

/**
 * ゲームモード保存
 */
#define SRAM_MODE (16)

/**
 * SRAM書き込みフラグ
 */
#define SRAM_ON ((u32)0xf0f0f0f0)

///////////////////////////////////////////////////////////////////// COLOR

/**
 * 15bit 透過色
 */
#define COLOR_TRANS RGB5(0, 0, 0)

///////////////////////////////////////////////////////////////////// PALETTE COLOR

/**
 * 透過色
 */
#define PAL_TRANS (0)

///////////////////////////////////////////////////////////////////// timer

#define DEF_TIMER_RELOAD (0)

///////////////////////////////////////////////////////////////////// マクロ

/**
 * ランダム
 */
#define RND(MIN, MAX) ((MIN) + (int)(rand() / (float)RAND_MAX * ((MAX) - (MIN) + 1)))

///////////////////////////////////////////////////////////////////// グローバル変数

/**
 * ゲーム管理
 */
GLOBAL GameStateType game_state;

/**
 * ステージ管理
 */
GLOBAL StageType stage;

/**
 * 自機
 */
GLOBAL SpriteCharType ship;

/**
 * スター
 */
GLOBAL StarType stars;

/**
 * スコア
 */
GLOBAL int score;
/**
 * ハイスコア
 */
GLOBAL int hiscore;

/**
 * 乱数の種
 */
GLOBAL u32 seed;

/**
 * 現在のステージBGM
 */
GLOBAL int stage_bgm;

///////////////////////////////////////////////////////////////////// プロトタイプ

void game();
void init_game();
void update_hiscore();
void disp_game_mode();
void load_title();

#endif /* _game_h_ */
