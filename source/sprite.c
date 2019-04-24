/**
 * @file sprite.c
 * @brief スプライト
 * @date  2017/02/01 更新
 * @author Choe Gyun(choikyun)
 */

/***************************************************
 * Moon Defender GBA
 * ver 1.0.0
 * 2017.02.01
 * Choe Gyun (choikyun)
 *****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <gba.h>

#include "sprite.h"
#include "bg.h"

//// スプライト
// 自機
#include "sprite_ship1.h"
#include "sprite_ship2.h"
#include "sprite_ship3.h"
#include "sprite_star1.h"
#include "sprite_star2.h"

/***************************************************
 スプライト初期化
 ***************************************************/
void init_sprite(void)
{
    int i;

    //すべて画面外へ
    for (i = 0; i < MAX_SPRITE; i++)
    {
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
    OBJATTR *sp = (OBJATTR *)OAM + num;

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
    OBJATTR *sp = (OBJATTR *)OAM + num;

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
    OBJATTR *sp = (OBJATTR *)OAM + num;

    sp->attr2 &= 0xfc00;
    sp->attr2 |= tile;
}

/***************************************************
 スプライトX座標読み取り
 ***************************************************/
s16 read_sprite_x(u16 num)
{
    OBJATTR *sp = (OBJATTR *)OAM + num;

    return sp->attr1 & ~0xfe00;
}

/***************************************************
 スプライトY座標読み取り
 ***************************************************/
s16 read_sprite_y(u16 num)
{
    OBJATTR *sp = (OBJATTR *)OAM + num;

    return sp->attr0 & ~0xff00;
}

/**********************************************/ /**
 * @brief アフィン設定 スプライトとアフィンパラメータの対応
 * @param num スプライト番号 0-127
 * @param a_num 対応するアフィンパラメータ 0-31
 * @param d_flg ダブルサイズを使用するか 0|SP_SIZE_DOUBLE
 ***********************************************/
void set_affine_setting(u16 num, u16 a_num, u16 d_flg)
{
    OBJATTR *sp = (OBJATTR *)OAM + num;

    sp->attr0 |= OBJ_ROT_SCALE_ON;

    sp->attr1 &= 0xC1ff;
    sp->attr1 |= OBJ_ROT_SCALE(a_num);
    sp->attr0 &= 0xfdff;
    sp->attr0 |= d_flg;
}

/**********************************************/ /**
 * @brief 拡大縮小
 * @param num アフィンパラメータ 0-31
 * @param x_sc Xスケール 1-100%
 * @param y_sc Yスケール 1-100%
 ***********************************************/
void set_scale(u16 num, u16 x_sc, u16 y_sc)
{
    OBJAFFINE *rot = (OBJAFFINE *)OAM + num;

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
    u16 *oam = DEF_MODE < MODE_3 ? OBJ_BASE_ADR : BITMAP_OBJ_BASE_ADR;

    // スプライトパレット
    u16 *pal = OBJ_COLORS;

    // 共通パレット転送
    CpuSet(sprite_ship1Pal, pal, (COPY32 | sprite_ship1PalLen / 4));

    // キャラクタデータ転送
    //ship
    CpuSet(sprite_ship1Tiles, oam, (COPY32 | sprite_ship1TilesLen / 4));           // 32dot
    CpuSet(sprite_ship2Tiles, oam + 512, (COPY32 | sprite_ship2TilesLen / 4));     // 32dot
    CpuSet(sprite_ship3Tiles, oam + 512 * 2, (COPY32 | sprite_ship3TilesLen / 4)); // 32dot

    // スター
    CpuSet(sprite_star1Tiles, oam + 512 * 3, (COPY32 | sprite_star1TilesLen / 4)); // 64dot
    CpuSet(sprite_star2Tiles, oam + 512 * 7, (COPY32 | sprite_star2TilesLen / 4)); // 64dot
}
