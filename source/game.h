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
 * ブロックの出現間隔
 */
#define BLOCK_INTERVAL (19)

/**
 * ブロックの同時出現数
 */
#define APPER_MAX_BLOCKS (1)

/**
 * ブロック 速度
 */
//#define BLOCK_SPEED (-4096 * 25)

/**
 * @brief 次のレベルまで（フレーム）
 * 
 */
#define NEXT_LEVEL (30 * 60)

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
#define STAGE_Y_BLANK (80 + 40)

/**
 * 座標補正X
 * ステージの論理座標からデバイス座標へ変換
 */
#define FIX_STAGE_X (SCREEN_WIDTH / 2)

/**
 * 座標補正Y
 * ステージの論理座標からデバイス座標へ変換
 */
#define FIX_STAGE_Y (16)

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
#define SHIP_Y (128)

/**
 * 自機 Y座標
 */
#define SHIP_Z MIN_Z

/**
 * @brief 自機 移動X座標最小値
 * 
 */
#define SHIP_MOVE_MIN_X (-200)

/**
 * @brief 自機 移動X座標最大値
 * 
 */
#define SHIP_MOVE_MAX_X (200)

/**
 * @brief 自機 移動Y座標最小値
 * 
 */
#define SHIP_MOVE_MIN_Y (STAGE_Y_BLANK)

/**
 * @brief 自機 移動Y座標最大値
 * 
 */
#define SHIP_MOVE_MAX_Y (SCREEN_HEIGHT - SHIP_H / 2)

/**
 * 自機 速度
 */
#define SHIP_SPEED (4096 * 2)

/**
 * @brief 自機自然減速
 * 
 */
#define SHIP_FRIC (0.01)

/**
 * @brief 自機最大加速度
 * 
 */
#define MAX_SHIP_ACC (3 << FIX)

/***************************************************
 * エネルギー
 ***************************************************/

/**
 * @brief エネルギー用の小数点
 * 
 */
#define E_FIX (8)

/**
 * @brief エネルギー
 * 
 */
#define MAX_ENERGY (30)

/**
 *  @brief エネルギー 余剰分
 * 
 */
#define MAX_ENERGY_BLANK (2)

/**
 * @brief エネルギー表示X座標
 * 
 */
#define ENERGY_X (168)

/**
 * @brief エネルギー表示Y座標
 * 
 */
#define ENERGY_Y (0)

/**
 * @brief エネルギー表示 バーの幅
 * 
 */
#define ENERGY_W (2)

/**
 * @brief エネルギー表示 バーの高さ
 * 
 */
#define ENERGY_H (8)

/**
 * @brief ダメージ
 * 
 */
#define DAMAGE_ENERGY (-10 << E_FIX)

/**
 * @brief 回復
 * 
 */
#define RECOVERY_ENERGY (5 << E_FIX)

/***************************************************
 * 逆噴射
 ***************************************************/

/**
 * @brief 使用エネルギー
 */
#define BOOTER_ENERGY (-RECOVERY_ENERGY)

/**
 * @brief 逆噴射の加速
 */
#define BOOSTER_ACC (2 << FIX)

/**
 * @brief 逆噴射時間
 */
#define BOOST_TIME (1 * 60)

/**
 * @brief 逆噴射 Y座標補正
 */
#define BOOSTER_FIXED_Y (-8)

/**
 * @brief 逆噴射アニメ インターバル
 */
#define BOOSTER_ICON_INTERVAL (3)

/**
 * @brief 逆噴射利用開始までの時間
 */
#define USE_BOOSTER_INTERVAL (20 * 60)

/**
 * @brief 逆噴射利用
 */
#define ALLOW_BOOSTER_ENERGY (MAX_ENERGY / 2)


/***************************************************
 * 振動アニメ
 ***************************************************/

/**
 * 振幅
 */
#define SHOCK_RANGE (4)


/**
 * 間隔
 */
#define SHOCK_INTERVAL (2)


/**
 * 長さ
 */
#define SHOCK_DURATION (SHOCK_INTERVAL * 6)

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
 * ブロック スプライト
 ***************************************************/

/**
 * ブロック 幅
 */
#define BLOCK_W (40)
//#define BLOCK_W (80)

/**
 * ブロック 高さ
 */
#define BLOCK_H (40)
//#define BLOCK_H (80)

/**
 * @brief スプライト幅
 * 
 */
#define BLOCK_SP_W (64)
//#define BLOCK_SP_W (128)

/**
 * @brief スプライト高さ
 * 
 */
#define BLOCK_SP_H (64)
//#define BLOCK_SP_H (128)

/**
 * ブロック最大数
 */
#define MAX_BLOCKS (16)

/**
 * @brief dデフォルトの加速度
 * 
 */
#define DEF_BLOCK_ACC (-4096 * 1)

/**
 * Z加速度最大値
 */
#define BLOCK_MAX_ACC (4) << FIX)

/**
 * ブロック出現間隔 X方向
 */
#define BLOCK_X_STEP (40)

/**
 * @brief ブロック出現 Y方向
 * 
 */
#define BLOCK_Y_STEP (40)

/**
 * @brief X方向の出現数
 * 
 */
#define BLOCK_X_STEP_NUM (10 - 1)

/**
 * @brief X方向の余白 2キャラ分
 * 
 */
#define BLOCK_X_STEP_BLANK (BLOCK_X_STEP *2)

/**
 * @brief Y方向の出現数
 * 
 */
#define BLOCK_Y_STEP_NUM (0)

/**
 * @brief ブロック着地点の余白
 * 
 */
#define BLOCK_Y_TARGET_BLANK (SCREEN_HEIGHT - BLOCK_H * 2 - FIX_STAGE_Y)

/**
 * @brief ブロック着地点 Y座標
 * 
 */
#define BLOCK_Y_TARGET (SHIP_Y - 40)

/***************************************************
 * ガイド
 ***************************************************/

/**
 * @brief ガイド幅
 * 
 */
#define GUIDE_W (8)

/**
 * @brief ガイド高さ
 * 
 */
#define GUIDE_H (8)

/**
 * @brief ガイドY座標
 * 
 */
#define GUIDE_Y (SHIP_Y + 12)

/***************************************************
 * 地平線
 ***************************************************/

/**
 * @brief 地平線の最大数
 * 
 */
#define MAX_LINES (10)

/**
 * @brief 地平線 速度
 * 
 */
#define LINE_SPEED (-4096 * 25)

/**
 * @brief 地平線出現間隔 Z座標
 */
#define LINE_INTERVAL (MAX_Z / MAX_LINES)

/**
 * @brief 地平線 Z座標 デフォルト
 */
#define LINE_Z MAX_Z

/**
 * @brief 地平線幅
 */
#define LINE_W (SCREEN_WIDTH)

/**
 * @brief X座標
 */
#define LINE_X (-120)

/**
 * @brief 地平線の目標Y座標
 */
#define LINE_Y_TARGET (SCREEN_HEIGHT - FIX_STAGE_Y)

/**
 * @brief 地平線の色 2dot分
 */
#define LINE_COLOR ((38 << 24) | (38 << 16) | (38 << 8) | 38)

/***************************************************
 * 境界線
 ***************************************************/

/**
 * @brief 境界線幅
 * 
 */
#define BOUNDARY_W (7)

/**
 * @brief 境界線高さ
 * 
 */
#define BOUNDARY_H (32)

/**
 * @brief 境界線Y座標
 * 
 */
#define BOUNDARY_Y (SHIP_Y - BOUNDARY_H / 2)

/***************************************************
 * アイコン
 ***************************************************/

/**
 * @brief リングアイコン Y座標
 * 
 */
#define ICON_RING_Y (-5)

/**
 * @brief リングアイコン ターゲット Y座標
 * 
 */
#define ICON_TARGET_Y (-20)

/**
 * @brief アイコン 表示時間
 * 
 */
#define ICON_LIFE (30)

/**
 * @brief アイコン 幅
 * 
 */
#define ICON_W (11)

/**
 * @brief アイコン 高さ
 * 
 */
#define ICON_H (11)

/**
 * @brief アイコン Y 加速度
 * 
 */
#define ICON_Y_ACC (-1 << FIX)

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
#define SCORE_X (76)
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
 *　リング
 ***************************************************/

/**
 * @brief リング数　桁
 */
#define RING_DIGIT (2)

/**
 * @brief リング数 X座標
 */
#define RING_X (152)

/**
 * @brief リング数　Y座標
 */
#define RING_Y (0)

/***************************************************
 *　メッセージ表示
 ***************************************************/

/**
 * @brief メッセージ表示　X座標
 */
#define MES_X (50)

/**
 * @brief メッセージ表示　Y座標
 */
#define MES_Y (60)

/**
 * @brief メッセージ表示　幅
 */
#define MES_W (140)

/**
 * @brief メッセージ表示　高さ
 */
#define MES_H (24)

/**
 * @brief ワーニングメッセージ開始まで
 * 
 */
#define WAIT_WARNING (3 * 60)


/***************************************************
 *　レベル
 ***************************************************/

/**
 * @brief 最高レベル
 * 
 */
#define MAX_LV (8)

/**
 * @brief レベル表示　桁
 */
#define LV_DIGIT (1)

/**
 * @brief レベル表示 X座標
 */
#define LV_X (30)

/**
 * @brief レベル表示　Y座標
 */
#define LV_Y (0)

/**
 * @brief レベルメッセージ X座標
 * 
 */
#define LV_MES_X (0)

/**
 * @brief レベルメッセージ Y座標
 * 
 */
#define LV_MES_X (0)

/**
 * @brief レベルメッセージ 幅
 * 
 */
#define LV_MES_W (28)

/**
 * @brief レベルメッセージ 高さ
 * 
 */
#define LV_MES_H (8)

/***************************************************
 *　爆風
 ***************************************************/

/**
 * @brief 爆風 最大値 ブロック
 * 
 */
#define MAX_BLOCK_BOMBS (3)

/**
 * @brief 爆風 最大値 ゲームオーバー
 * 
 */
#define MAX_OVER_BOMBS (5)

/**
 * @brief 爆風 発生範囲
 * 
 */
#define BOMB_RANGE (32)

/**
 * @brief 爆風 インターバル
 */
#define BOMB_INTERVAL (5)

/**
 * @brief 爆風 幅
 */
#define BOMB_W (16)

/**
 * @brief 爆風 高さ
 */
#define BOMB_H (16)

/***************************************************
 *　BGM
 ***************************************************/

/**
 * @brief ステージBGM数
 */
#define MAX_STAGE_BGM (3)

///////////////////////////////////////////////////////////////////// ENUM

/**
 * @brief シーン
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
 * @brief スプライトキャラクタ
 */
enum {
    // 爆風
    SPRITE_BOMB,
    // 炎
    SPRITE_FIRE,
    // 自機
    SPRITE_SHIP,
    // 逆噴射
    SPRITE_BOOSTER,
    // 逆噴射アイコン
    SPRITE_BOOSTERICON,
    // ガイド
    SPRITE_GUIDE,
    // 境界線
    SPRITE_BOUNDARY_L,
    // 境界線
    SPRITE_BOUNDARY_R,
    // リングアイコン
    SPRITE_RINGICON,
    // ブロック
    SPRITE_BLOCK,
};

/**
 * @brief ブロックの種類
 */
typedef enum {
    // 通常のブロック
    NORMAL = 0,
    // リング
    RING,
} BlockTypeEnum;

/**
 * @brief 爆風の種類
 * 
 */
typedef enum {
    BOMB_BLOCK,
    BOMB_OVER
};

///////////////////////////////////////////////////////////////////// 構造体

/**
 * @brief 座標 2d
 */
typedef struct
{
    int x;
    int y;
} ALIGN(4) PointType;

/**
 * @brief 矩形
 */
typedef struct
{
    int x;
    int y;
    int w;
    int h;
} ALIGN(4) RectangleType;

/**
 * @brief 座標 3d
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
 * @brief 振動
 */
typedef struct
{
    int range;
    int direc;
    int interval;
    int duration;
} ALIGN(4) ShockType;

/**
 * @brief フラッシュ
 */
typedef struct
{
    u8 color[32];
    int interval;
} ALIGN(4) FlashType;

/**
 * @brief ゲーム管理
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
 * @brief ステージ管理
 */
typedef struct
{
    // レベル
    int lv;
    // 次のレベルまで（フレーム数）
    int next_lv;
    // 獲得リング
    int ring;
    // 経過フレーム
    int frame;
    // ステージの中心座標
    VectorType center;
    // 次のブロックが出現するまでの間隔
    int star_interval;
    // モード
    int mode;
    // デモモード
    bool demo;
    // フラッシュ
    FlashType flash;
} ALIGN(4) StageType;

/**
 * @brief フレームアニメ
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
 * @brief スプライト
 */
typedef struct
{
    // キャラクタ番号 0-127
    int chr;
    // タイル番号 512-1024
    int tile;
    // キャラクタータイプ
    BlockTypeEnum type;
    // 現在の座標
    VectorType vec;
    // 加速度
    VectorType acc;
    // 目標の座標
    PointType target;
    // 座標補正用
    PointType fix;
    // 中心のオフセット
    PointType center;
    // 矩形
    RectangleType rect;
    // 当たり判定用矩形
    RectangleType hit;
    // 表示フラグ
    bool show;
} ALIGN(4) SpriteCharType;

/**
 * @brief 自機
 */
typedef struct {
    // エネルギー
    int energy;
    // 逆噴射
    int booster;
    // 逆噴射可能か
    bool allows_booster;
    // 振動
    ShockType shock;
    // スプライト
    SpriteCharType sprite;
} ALIGN(4) ShipType;

/**
 * @brief 炎
 */
typedef struct {
    // アニメーション用
    AnimeType anime;
    // スプライト
    SpriteCharType sprite;
} ALIGN(4) FireType;

/**
 * @brief アイコン
 */
typedef struct {
    // 表示期間
    int life;
    // 目標座標
    PointType target;
    // スプライト
    SpriteCharType sprite;
} ALIGN(4) IconType;

/**
 * @brief ブロック管理
 */
typedef struct
{
    // 出現間隔 Z軸
    int interval;
    // 出現間隔 Z軸 リロード
    int interval_rel;
    // 現在の数
    int num;
    // 最大出現数
    int max_blocks;
    // 加速度（全てのブロック共通）
    int acc;
    // デフォルトの速度（全てのブロック共通）
    int speed;
    // リング出現確率
    int ring_chance;
    // ブロックのリスト
    SpriteCharType list[MAX_BLOCKS];
} ALIGN(4) BlockType;

/**
 * @brief 爆発管理
 */
typedef struct
{
    // 現在の数
    int num;
    // 最大数
    int max;
    // ベース座標
    PointType base;
    // アニメ
    AnimeType anime;
    // スプライト
    SpriteCharType sprite;
} ALIGN(4) BombType;

/**
 * @brief 地平線管理
 */
typedef struct
{
    // 現在の数
    int num;
    // 進んだ距離 Z軸
    int z;
    // 地平線のリスト
    SpriteCharType list[MAX_LINES];
} ALIGN(4) LineType;

/**
 * @brief 境界線
 */
typedef struct {
    // アニメーション用
    AnimeType anime;
    // スプライト
    SpriteCharType sprite;
} ALIGN(4) BoundaryType;

/**
 * @brief 点滅メッセージ
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
 * @brief ウェイト
 */
typedef struct
{
    int wait;
} ALIGN(4) WaitType;

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

// ブロック
#define TILE_BLOCK1 (688) // 128tiles

// リング
#define TILE_RING1 (816) // 128tiles

// ガイド
#define TILE_GUIDE1 (944) // 2tiles

// 境界線
#define TILE_BOUNDARY1 (946) // 8tiles

// リングアイコン
#define TILE_RINGICON1 (954) // 8tiles

// 逆噴射
#define TILE_BOOSTER1 (962) // 8tiles
#define TILE_BOOSTER2 (970) // 8

// 逆噴射アイコン
#define TILE_BOOSTERICON1 (978) // 2tiles
#define TILE_BOOSTERICON2 (980) // 2

// 爆風
#define TILE_BOMB1 (982) // 8tiles
#define TILE_BOMB2 (990) // 8
#define TILE_BOMB3 (998) // 8
#define TILE_BOMB4 (1006) // 8

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
GLOBAL ShipType ship;

/**
 * 炎
 */
GLOBAL FireType fire;

/**
 * 逆噴射
 */
GLOBAL FireType booster;

/**
 * ブロック
 */
GLOBAL BlockType blocks;

/**
 * 地平線
 */
GLOBAL LineType lines;

 /**
 * 地平線
 */
GLOBAL BombType bomb;

/**
 * @brief ガイド
 * 
 */
GLOBAL SpriteCharType guide;

/**
 * @brief 境界線
 * 
 */
GLOBAL BoundaryType boundary_r;

/**
 * @brief 境界線
 * 
 */
GLOBAL BoundaryType boundary_l;

/**
 * @brief リングアイコン
 * 
 */
GLOBAL IconType ring_icon;

/**
 * @brief 逆噴射アイコンアニメ用
 * 
 */
GLOBAL AnimeType booster_icon_anime;

/**
 * @brief 通常のメッセージ表示
 */
GLOBAL BlinkMessageType mes;

/**
 * @brief レベルアップ表示
 * 
 */
GLOBAL BlinkMessageType lv_mes;


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
