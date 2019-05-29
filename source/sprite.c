/**
 * @file sprite.c
 * @brief スプライト
 * @date  2019/05/28
 * @author Choi Gyun
 */

/***************************************************
 * Moon Rider
 * ver 1.0.0
 * 2019.04.19
 * Choi Gyun
 *****************************************************/

#include <gba.h>
#include <stdio.h>
#include <stdlib.h>

#include "bg.h"
#include "sprite.h"

//// スプライト
// 自機
#include "sprite_ship1.h"
#include "sprite_ship2.h"
#include "sprite_ship3.h"
#include "sprite_ship4.h"
#include "sprite_ship5.h"

// 炎
#include "sprite_fire1.h"
#include "sprite_fire2.h"

// スター
#include "sprite_star1.h"
#include "sprite_star2.h"

// 柱
#include "sprite_pole1.h"

// ブロック
#include "sprite_block1.h"

// リング
#include "sprite_ring1.h"

// ガイド
#include "sprite_guide1.h"

// 境界線
#include "sprite_boundary1.h"

// リングアイコン
#include "sprite_ringicon1.h"

// ブースター
#include "sprite_booster1.h"

// ブースターアイコン
#include "sprite_booster_icon1.h"
#include "sprite_booster_icon2.h"

// 爆風
#include "sprite_bomb1.h"
#include "sprite_bomb2.h"
#include "sprite_bomb3.h"
#include "sprite_bomb4.h"

// ブラボー
#include "sprite_bravo1.h"

// 矢印
#include "sprite_arrow.h"

/***************************************************
 スプライト初期化
 ***************************************************/
void init_sprite(void)
{
    int i;

    //すべて画面外へ
    for (i = 0; i < MAX_SPRITE; i++) {
        move_sprite(i, SCREEN_WIDTH, SCREEN_HEIGHT);
    }
}

/***************************************************
 スプライト非表示
 ***************************************************/
void erase_sprite(u16 num)
{
    move_sprite(num, SCREEN_WIDTH, SCREEN_HEIGHT);
}

/***************************************************
 スプライト移動
 ***************************************************/
void move_sprite(u16 num, s16 x, s16 y)
{
    OBJATTR* sp = (OBJATTR*)OAM + num;

    //if(x < 0) x += 512;
    //if(y < 0) y += 256;

    sp->attr1 &= 0xfe00;
    sp->attr0 &= 0xff00;
    sp->attr1 |= OBJ_X(x);
    sp->attr0 |= OBJ_Y(y);
}

/***************************************************
 スプライト形状　カラー サイズ
 ***************************************************/
void set_sprite_form(u16 num, u16 size, u16 form, u16 col)
{
    OBJATTR* sp = (OBJATTR*)OAM + num;

    sp->attr0 &= 0x1fff;
    sp->attr1 &= 0x3fff;
    sp->attr0 |= col | form;
    sp->attr1 |= size;
}

/***************************************************
 タイルキャラクタ設定
 **************************************************/
void set_sprite_tile(u16 num, u16 tile)
{
    OBJATTR* sp = (OBJATTR*)OAM + num;

    sp->attr2 &= 0xfc00;
    sp->attr2 |= tile;
}

/***************************************************
 スプライト 表示優先度
 **************************************************/
void set_sprite_priority(u16 num, u16 priority)
{
    OBJATTR* sp = (OBJATTR*)OAM + num;

    sp->attr2 &= 0xf3ff;
    sp->attr2 |= OBJ_PRIORITY(priority);
}


/***************************************************
 スプライトX座標読み取り
 ***************************************************/
s16 read_sprite_x(u16 num)
{
    OBJATTR* sp = (OBJATTR*)OAM + num;

    return sp->attr1 & ~0xfe00;
}

/***************************************************
 スプライトY座標読み取り
 ***************************************************/
s16 read_sprite_y(u16 num)
{
    OBJATTR* sp = (OBJATTR*)OAM + num;

    return sp->attr0 & ~0xff00;
}

/**********************************************/ /**
 * @brief アフィン設定 スプライトとアフィンパラメータの対応
 * 
 * @param num スプライト番号 0-127
 * @param a_num 対応するアフィンパラメータ 0-31
 * @param d_flg ダブルサイズを使用するか 0|SP_SIZE_DOUBLE
 ***********************************************/
void set_affine_setting(u16 num, u16 a_num, u16 d_flg)
{
    OBJATTR* sp = (OBJATTR*)OAM + num;

    sp->attr0 |= OBJ_ROT_SCALE_ON;

    sp->attr1 &= 0xC1ff;
    sp->attr1 |= OBJ_ROT_SCALE(a_num);
    sp->attr0 &= 0xfdff;
    sp->attr0 |= d_flg;
}

/**********************************************/ /**
 * @brief 拡大縮小
 * 
 * @param num アフィンパラメータ 0-31
 * @param x_sc Xスケール 1-100%
 * @param y_sc Yスケール 1-100%
 ***********************************************/
void set_scale(u16 num, u16 x_sc, u16 y_sc)
{
    OBJAFFINE* rot = (OBJAFFINE*)OAM + num;

    rot->pa = 256 * 100 / x_sc;
    rot->pb = 0;
    rot->pc = 0;
    rot->pd = 256 * 100 / y_sc;
}

/***************************************************
 キャラクタ初期化
 ***************************************************/
void init_sprite_chr(void)
{
    // スプライトデータ
    u16* oam = DEF_MODE < MODE_3 ? OBJ_BASE_ADR : BITMAP_OBJ_BASE_ADR;

    // スプライトパレット
    u16* pal = OBJ_COLORS;

    // 共通パレット転送
    CpuSet(sprite_ship1Pal, pal, (COPY32 | sprite_ship1PalLen / 4));

    // キャラクタデータ転送
    //ship
    CpuSet(sprite_ship1Tiles, oam, (COPY32 | sprite_ship1TilesLen / 4)); // 32dot
    CpuSet(sprite_ship2Tiles, oam + 512, (COPY32 | sprite_ship2TilesLen / 4)); // 32dot
    CpuSet(sprite_ship3Tiles, oam + 512 * 2, (COPY32 | sprite_ship3TilesLen / 4)); // 32dot
    CpuSet(sprite_ship4Tiles, oam + 512 * 3, (COPY32 | sprite_ship4TilesLen / 4)); // 32dot
    CpuSet(sprite_ship5Tiles, oam + 512 * 4, (COPY32 | sprite_ship5TilesLen / 4)); // 32dot

    // 炎
    CpuSet(sprite_fire1Tiles, oam + 512 * 5, (COPY32 | sprite_fire1TilesLen / 4)); // 16dot
    CpuSet(sprite_fire2Tiles, oam + 512 * 5 + 128, (COPY32 | sprite_fire2TilesLen / 4)); // 16dot

    // スター
    CpuSet(sprite_block1Tiles, oam + 512 * 5 + 128 * 2, (COPY32 | sprite_block1TilesLen / 4)); // 64dot
    
    // リング
    CpuSet(sprite_ring1Tiles, oam + 512 * 5 + 128 * 2 + 2048, (COPY32 | sprite_ring1TilesLen / 4)); // 64dot

    // ガイド
    CpuSet(sprite_guide1Tiles, oam + 512 * 5 + 128 * 2 + 2048 * 2, (COPY32 | sprite_guide1TilesLen / 4)); // 8dot

    // 境界線
    CpuSet(sprite_boundary1Tiles, oam + 512 * 5 + 128 * 2 + 2048 * 2 + 32, (COPY32 | sprite_boundary1TilesLen / 4)); // 8dot

    // リングアイコン
    CpuSet(sprite_ringicon1Tiles, oam + 512 * 5 + 128 * 2 + 2048 * 2 + 32 + 128, (COPY32 | sprite_ringicon1TilesLen / 4)); // 16dot

    // ブースター
    CpuSet(sprite_booster1Tiles, oam + 512 * 5 + 128 * 2 + 2048 * 2 + 32 + 128 * 2, (COPY32 | sprite_booster1TilesLen / 4)); // 16dot
    CpuSet(sprite_fire2Tiles, oam + 512 * 5 + 128 * 2 + 2048 * 2 + 32 + 128 * 3, (COPY32 | sprite_fire2TilesLen / 4)); // 16dot

    // ブースターアイコン
    CpuSet(sprite_booster_icon1Tiles, oam + 512 * 5 + 128 * 2 + 2048 * 2 + 32 + 128 * 4, (COPY32 | sprite_booster_icon1TilesLen / 4)); // 8dot
    CpuSet(sprite_booster_icon2Tiles, oam + 512 * 5 + 128 * 2 + 2048 * 2 + 32 + 128 * 4 + 32, (COPY32 | sprite_booster_icon1TilesLen / 4)); // 8dot

    // 爆風
    CpuSet(sprite_bomb1Tiles, oam + 512 * 5 + 128 * 2 + 2048 * 2 + 32 + 128 * 4 + 32 * 2, (COPY32 | sprite_bomb1TilesLen / 4)); // 16dot
    CpuSet(sprite_bomb2Tiles, oam + 512 * 5 + 128 * 2 + 2048 * 2 + 32 + 128 * 4 + 32 * 2 + 128, (COPY32 | sprite_bomb2TilesLen / 4)); // 16dot
    CpuSet(sprite_bomb3Tiles, oam + 512 * 5 + 128 * 2 + 2048 * 2 + 32 + 128 * 4 + 32 * 2 + 128 * 2, (COPY32 | sprite_bomb3TilesLen / 4)); // 16dot
    CpuSet(sprite_bomb4Tiles, oam + 512 * 5 + 128 * 2 + 2048 * 2 + 32 + 128 * 4 + 32 * 2 + 128 * 3, (COPY32 | sprite_bomb4TilesLen / 4)); // 16dot

    // ブラボー
    CpuSet(sprite_bravo1Tiles, oam + 512 * 5 + 128 * 2 + 2048 * 2 + 32 + 128 * 4 + 32 * 2 + 128 * 4, (COPY32 | sprite_bravo1TilesLen / 4)); // 32dot

    // 矢印
    CpuSet(sprite_arrowTiles, oam + 512 * 5 + 128 * 2 + 2048 * 2 + 32 + 128 * 4 + 32 * 2 + 128 * 5, (COPY32 | sprite_arrowTilesLen / 4)); // 8dot
}
