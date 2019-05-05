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

#include "bg.h"
#include <gba.h>

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

/**
 * @brief 画面の中心
 * 
 */
#define SCREEN_CENTER (SCREEN_WIDTH / 2)

/***************************************************
 * ゲーム個別
 ***************************************************/

/***************************************************
 * ゲームバランスに関連するパラメータ
 ***************************************************/

/**
 * 星の出現間隔
 */
#define STAR_INTERVAL (30)

/**
 * 星の同時出現数
 */
#define APPER_MAX_STARS (1)

/**
 * スター 速度
 */
#define STAR_SPEED (-4096 * 32)

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
#define MAX_Z (300)

/**
 * Z座標 最小値
 */
#define MIN_Z (20)

/**
 * @brief ステータス表示域の高さ
 * 
 */
#define STATUS_HEIGHT (32)

/**
 * @brief ステージY方向の余白
 * 
 */
#define STAGE_Y_BLANK (80)


/**
 * 座標補正X
 * ステージの論理座標からデバイス座標へ変換
 */
#define FIX_STAGE_X (SCREEN_WIDTH / 2 - MIN_Z / 2)

/**
 * 座標補正Y
 * ステージの論理座標からデバイス座標へ変換
 */
#define FIX_STAGE_Y (STATUS_HEIGHT - MIN_Z / 2)

/**
 * @brief ブロック着地点の余白
 * 
 */
#define STAGE_Y_TARGET_BLANK (SCREEN_HEIGHT - STAR_H * 2 - FIX_STAGE_Y)

/***************************************************
 * 自機 スプライト
 ***************************************************/

/**
 * 自機 幅
 */
#define SHIP_W (31)
/**
 * 自機 高さ
 */
#define SHIP_H (31)

/**
 * 自機 X座標 中心
 */
#define SHIP_X (0)

/**
 * 自機 Y座標 中心
 */
#define SHIP_Y (100)

/**
 * 自機 Y座標
 */
#define SHIP_Z MIN_Z

/**
 * @brief 自機 移動X座標最小値
 * 
 */
#define SHIP_MOVE_MIN_X (-120)

/**
 * @brief 自機 移動X座標最大値
 * 
 */
#define SHIP_MOVE_MAX_X (119)

/**
 * @brief 自機 移動Y座標最小値
 * 
 */
#define SHIP_MOVE_MIN_Y (58 + SHIP_H / 2)

/**
 * @brief 自機 移動Y座標最大値
 * 
 */
#define SHIP_MOVE_MAX_Y (120)

/**
 * 自機 速度
 */
#define SHIP_SPEED (4096 * 1)

/**
 * @brief 自機自然減速
 * 
 */
#define SHIP_FRIC (0.02)

/**
 * @brief 自機最大加速度
 * 
 */
#define MAX_SHIP_ACC (2 << FIX)

/***************************************************
 * 炎 スプライト
 ***************************************************/

/**
 * 炎 幅
 */
#define FIRE_W (15)

/**
 * 炎 高さ
 */
#define FIRE_H (15)

/**
 * 炎 X座標 中心
 */
#define FIRE_X (0)

/**
 * 炎 Y座標 中心
 */
#define FIRE_Y (6)

/**
 * 炎 Y座標 中心 自機が上移動のとき
 */
#define FIRE_Y_UP (10)

/**
 * 炎 Y座標 中心 自機が下移動のとき
 */
#define FIRE_Y_DOWN (4)

/**
 * 炎 Y座標 中心 自機が上移動のとき
 */
#define FIRE_X_LEFT (4)

/**
 * 炎 Y座標 中心 自機が下移動のとき
 */
#define FIRE_X_RIGHT (-4)

/**
 * 炎 Y座標
 */
#define FIRE_Z MIN_Z

/**
 * 炎 点滅間隔
 */
#define FIRE_INTERVAL (3)

/***************************************************
 * スター スプライト
 ***************************************************/

/**
 * スター 幅
 */
#define STAR_W (40)

/**
 * スター 高さ
 */
#define STAR_H (40)

/**
 * @brief スプライト幅
 * 
 */
#define STAR_SP_W (64)

/**
 * @brief スプライト高さ
 * 
 */
#define STAR_SP_H (64)

/**
 * スター最大数
 */
#define MAX_STARS (8)

/**
 * Z加速度最大値
 */
#define STAR_MAX_ACC (4) << FIX)

/**
 * スター出現間隔 X方向
 */
#define STAR_X_STEP (40)

/**
 * @brief スター出現 Y方向
 * 
 */
#define STAR_Y_STEP (40)

/**
 * @brief X方向の出現数
 * 
 */
#define STAR_X_STEP_NUM (SCREEN_WIDTH / STAR_X_STEP - 1)

/**
 * @brief Y方向の出現数
 * 
 */
#define STAR_Y_STEP_NUM ((SCREEN_HEIGHT - STAGE_Y_BLANK) / STAR_Y_STEP - 1)

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

///////////////////////////////////////////////////////////////////// 構造体

/**
 * 座標 2d
 */
typedef struct
{
    int x;
    int y;
} ALIGN(4) PointType;

/**
 * 矩形
 */
typedef struct
{
    int x;
    int y;
    int w;
    int h;
} ALIGN(4) RectangleType;

/**
 * 座標 3d
 */
typedef struct
{
    int x;
    int y;
    int z;
    // スケール 1-100%
    int scale;
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
    // レベル
    int lv;
    // ステージの中心座標
    VectorType center;
    // 次のスターが出現するまでの間隔
    int star_interval;
    // モード
    int mode;
    // デモモード
    bool demo;
} ALIGN(4) StageType;

/**
 * フレームアニメ
 */
typedef struct
{
    bool is_start; // 開始
    int frame; // 現在のフレーム
    int max_frame; // フレーム最大値
    int interval; // インターバル
    int interval_rel; // インターバル既定値
} ALIGN(4) AnimeType;

/**
 * スプライト
 */
typedef struct
{
    // キャラクタ番号 0-127
    int chr;
    // タイル番号 512-1024
    int tile;
    // 現在の3d座標
    VectorType vec;
    // 加速度
    VectorType acc;
    // 目標のXY座標
    PointType target;
    // 中心のオフセット
    PointType center;
    // 当たり判定用矩形
    RectangleType hit;
    // 表示フラグ
    bool show;
} ALIGN(4) SpriteCharType;

/**
 * 炎
 */
typedef struct {
    // アニメーション用
    AnimeType anime;
    // スプライト
    SpriteCharType sprite;
} ALIGN(4) FireType;

/**
 * スター管理
 */
typedef struct
{
    // 出現間隔
    int interval;
    // 出現間隔リロード
    int interval_rel;
    // 現在のスター数
    int num;
    // スターのリスト
    SpriteCharType list[MAX_STARS];
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
enum {
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
enum {
    // 炎
    SPRITE_FIRE = 0,

    // 自機
    SPRITE_SHIP = 1,

    // スター
    SPRITE_STAR = 2, // -17 まで
} SpriteEnum;

///////////////////////////////////////////////////////////////////// タイル番号

// タイルサイズ
#define TILE_SIZE_8 (2)
#define TILE_SIZE_16 (8)
#define TILE_SIZE_32 (32)
#define TILE_SIZE_64 (128)

// 自機
#define TILE_SHIP1 (512) // 32tiles
#define TILE_SHIP2 (544) // 32
#define TILE_SHIP3 (576) // 32
#define TILE_SHIP4 (608) // 32
#define TILE_SHIP5 (640) // 32

// 炎
#define TILE_FIRE1 (672) // 8tiles
#define TILE_FIRE2 (680) // 8

// スター
#define TILE_STAR1 (688) // 64tiles

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
 * 炎
 */
GLOBAL FireType fire;

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
