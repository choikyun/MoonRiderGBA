/**
 * @file bg.ｈ
 * @brief バックグラウンド描画のヘッダ
 * @date  2017/02/01 更新
 * @author Choe Gyun(choikyun)
 */

/***************************************************
 * Moon Defender GBA
 * ver 1.0.0
 * 2017.02.01
 * Choe Gyun (choikyun)
 *****************************************************/


#ifndef _bg_h_
#define _bg_h_

#include <gba.h>

#include "bmp_bg1.h"
#include "bmp_energy1.h"
#include "bmp_energy2.h"
#include "bmp_num.h"

#include "sprite_ship1.h"


#ifdef GLOBAL_VALUE_DEFINE
#define GLOBAL
#define GLOBAL_VAL(v) = (v)
#else
#define GLOBAL extern
#define GLOBAL_VAL(v)
#endif

///////////////////////////////////////////////////////////////////// 各種定義

//// BGタイル モード0

// BGコントロールレジスタ
#define DEF_BG_REG REG_BG0CNT

// BGモード
// bitmap
#define DEF_MODE MODE_4

// BGイネーブル
#define DEF_BG_ENABLE BG0_ENABLE

// BGサイズ
#define DEF_BG_SIZE BG_SIZE_0

// BGカラー
#define DEF_BG_COLOR BG_256_COLOR

// BGキャラクタ
#define DEF_CHAR_BASE CHAR_BASE(0)
#define DEF_BG_BASE_ADR CHAR_BASE_ADR(0)
#define MAX_MAP_X (32)
#define MAX_MAP_Y (32)
#define MAP_WIDTH (32)
#define MAP_HEIGHT (32)

// BGマップ
#define DEF_MAP_BASE MAP_BASE(28)
#define DEF_MAP_BASE_ADR MAP_BASE_ADR(28)

// BGビットマップデータ
#define DEF_BG_LEN bmp_bg1BitmapLen
#define DEF_BG_BITMAP bmp_bg1Bitmap

// BGパレット
#define DEF_BG_PALLET bmp_bg1Pal
#define DEF_BG_PALLET_LEN sprite_ship1PalLen

// タイトルビットマップデータ
//#define DEF_TITLE_LEN bmp_titleBitmapLen
//#define DEF_TITLE_BITMAP bmp_titleBitmap

// MODE4 バックフレーム
#define BACKFRAME ((u16 *)0x0600A000)

// MODE4 バックフレーム切り替え
#define FRAME_FLIP_ADDRESS ((u32)0xA000)

// スクリーンのブランク ステータス表示部分は描画しない
//#define SCREEN_BLANK (8 * 240 / 2)
//#define SCREEN_BLANK4 (8 * 240 / 4)



///////////////////////////////////////////////////////////////////// グローバル変数

/*
 * 2フレームモード（MODE4,MODE5）のときのカレントフレーム
 */
GLOBAL u16 *current_frame GLOBAL_VAL(BACKFRAME);

///////////////////////////////////////////////////////////////////// プロトタイプ

/***************************************************
 BG初期化
 ***************************************************/
void
init_bg (void);
/***************************************************
 フレーム初期化
 ***************************************************/
void
reset_frame (void);
/***************************************************
 VRAMにBGビットマップ転送
 ***************************************************/
void
load_bg_bitmap (const u16 *, int);
/***************************************************
 VRAMにBGビットマップ転送 32byte単位
 ***************************************************/
void
load_bg_bitmap_fast (const u16 *, int);
/***************************************************
 VRAMにビットマップ転送 圧縮LZ77
 ***************************************************/
void
load_bg_bitmap_lz77 (const u16 *);
/***************************************************
 BG ビットマップモード用パレット転送
 ***************************************************/
void
load_bg_bitmap_pallet (void);
/***************************************************
 BG タイル・パレット転送
 ***************************************************/
void
load_bg_tile (void);

/***************************************************
 BG ビットマップ描画 15bit
 ***************************************************/
void
draw_bitmap16 (int, int, int, int, const u16 *, u16);
/***************************************************
 BG ビットマップ退避 15bit
 ***************************************************/
void
save_bitmap16 (int, int, int, int, u16 *);
/***************************************************
 BG ビットマップ復帰
 ***************************************************/
void
load_bitmap16 (int, int, int, int, u16 *);
/***************************************************
 BG ビットマップ描画 8bit
 ***************************************************/
void
draw_bitmap8 (int, int, int, int, const u16 *);
/***************************************************
 BG ビットマップ描画 8bit　フレーム
 ***************************************************/
void
draw_bitmap_frame (int, int, int, int, const u16 *);
/***************************************************
 FRAME切り替え MODE4/MODE5
 ***************************************************/
void
flip_frame(void);
/***************************************************
  カレントフレームにビットマップ転送 圧縮LZ77
 ***************************************************/
void
load_frame_bitmap_lz77 (const u16 *);
/***************************************************
 カレントフレームにビットマップ転送
 ***************************************************/
void
load_frame_bitmap(const u16 *);
/***************************************************
 カレントフレームをFILL
 ***************************************************/
void
fill_frame_bitmap(const u8*);



#endif	/* _bg_h_ */
