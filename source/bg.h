/**
 * @file bg.ｈ
 * @brief バックグラウンド描画のヘッダ
 * @date  2019/05/28
 * @author Choi Gyun
 */

/***************************************************
 * Moon Rider
 * ver 1.0.0
 * 2019.04.19
 * Choi Gyun
 *****************************************************/

#ifndef _bg_h_
#define _bg_h_

#include <gba.h>

// タイトル
#include "bmp_title.h"

// 背景
#include "bmp_bg1.h"

// エネルギーバー
#include "bmp_energy1.h"
#include "bmp_energy2.h"

// 数字
#include "bmp_num.h"

// ゾーン
#include "bmp_zone1.h"
#include "bmp_zone2.h"

// warning
#include "bmp_warning.h"

// gameover
#include "bmp_over.h"

// bonus
#include "bmp_bonus.h"

// total
#include "bmp_total.h"

// pause
#include "bmp_pause.h"

// トロフィー オン
#include "bmp_trophy.h"

// トロフィー オフ
#include "bmp_notrophy.h"

// アンロックメッセージ
#include "bmp_unlocked.h"

// 自機
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

// パレット
#define DEF_BG_PALLET sprite_ship1Pal
#define DEF_BG_PALLET_LEN sprite_ship1PalLen

// タイトルビットマップデータ
#define DEF_TITLE_LEN bmp_titleBitmapLen
#define DEF_TITLE_BITMAP bmp_titleBitmap

// MODE4 バックフレーム
#define BACKFRAME ((u16 *)0x0600A000)

// MODE4 バックフレーム切り替え
#define FRAME_FLIP_ADDRESS ((u32)0xA000)

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
fill_frame_bitmap(int, int, const u8*);



#endif	/* _bg_h_ */
