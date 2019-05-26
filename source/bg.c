/**
 * @file bg.c
 * @brief バックグラウンド描画
 * @date  2017/02/01 更新
 * @author Choe Gyun(choikyun)
 */

/***************************************************
 * Moon Defender GBA
 * ver 1.0.0
 * 2017.02.01
 * Choe Gyun (choikyun)
 *****************************************************/

#include <gba.h>
#include <stdio.h>
#include <stdlib.h>

#include "bg.h"

/***************************************************
 BG初期化
 ***************************************************/
void init_bg()
{
    //// MODE4を使用

    // モード設定
    SetMode(DEF_MODE | BG2_ENABLE | OBJ_ENABLE | OBJ_1D_MAP);

    // パレット
    load_bg_bitmap_pallet();

    // BGをフレームにロード
    load_frame_bitmap(DEF_BG_BITMAP);
}

/***************************************************
 フレーム初期化
 ***************************************************/
void reset_frame()
{
    REG_DISPCNT &= 0xffff ^ BACKBUFFER; // 表示はフロントバッファ
    current_frame = BACKFRAME;
}

/*********************************************************************
 * ビットマップモード共通
 * 15bit/8bit モード
 *********************************************************************/

/***************************************************
 VRAMにBGビットマップ転送
 ***************************************************/
void load_bg_bitmap(const u16* data, int len)
{
    u16* screen = (u16*)VRAM;

    CpuSet(data, screen, (COPY16 | len));
}

/***************************************************
 VRAMにBGビットマップ転送 32byte単位
 ***************************************************/
void load_bg_bitmap_fast(const u16* data, int len)
{
    u16* screen = (u16*)VRAM;

    CpuFastSet(data, screen, len);
}

/***************************************************
 VRAMにビットマップ転送 圧縮LZ77
 ***************************************************/
void load_bg_bitmap_lz77(const u16* data)
{
    LZ77UnCompVram((void*)data, (void*)VRAM);
}

/***************************************************
 BG ビットマップモード用パレット転送
 ***************************************************/
void load_bg_bitmap_pallet(void)
{
    CpuSet(DEF_BG_PALLET, BG_PALETTE, (COPY16 | DEF_BG_PALLET_LEN / 2));
}

/***************************************************
 BG タイル・パレット転送
 ***************************************************/
void load_bg_tile(void)
{
}

/*********************************************************************
 * MODE3
 * 15bit
 *********************************************************************/

/***************************************************
 BG ビットマップ描画 15bit
 ***************************************************/
void draw_bitmap16(int x, int y, int w, int h, const u16* data, u16 trans)
{
    int i, j;
    u16* screen = (u16*)VRAM + y * SCREEN_WIDTH + x;

    int blank = SCREEN_WIDTH - w;
    for (j = 0; j < h; j++) {
        for (i = 0; i < w; i++) {
            if (*data != trans)
                *screen++ = *data++;
            else {
                screen++;
                data++;
            }
        }
        screen += blank;
    }
}

/***************************************************
 BG ビットマップ退避 15bit
 ***************************************************/
void save_bitmap16(int x, int y, int w, int h, u16* data)
{
    int i, j;
    u16* screen = (u16*)VRAM + y * SCREEN_WIDTH + x;

    for (j = 0; j < h; j++) {
        for (i = 0; i < w; i++) {
            *data++ = *screen++;
        }
        screen += SCREEN_WIDTH - w;
    }
}

/***************************************************
 BG ビットマップ復帰
 ***************************************************/
void load_bitmap16(int x, int y, int w, int h, u16* data)
{
    int i, j;
    u16* screen = (u16*)VRAM + y * SCREEN_WIDTH + x;

    for (j = 0; j < h; j++) {
        for (i = 0; i < w; i++) {
            *screen++ = *data++;
        }
        screen += SCREEN_WIDTH - w;
    }
}

/*********************************************************************
 * MODE4, MODE5
 * 8bit
 *********************************************************************/

/***************************************************
 BG ビットマップ描画 8bit
 ***************************************************/
void draw_bitmap8(int x, int y, int w, int h, const u16* data)
{
    u16* screen = (u16*)VRAM + (y * SCREEN_WIDTH + x) / 2; // VRAMの書き込みは16/32bit単位

    int blank = (SCREEN_WIDTH - w) / 2;
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w / 2; i++) {
            *screen++ = *data++;
        }
        screen += blank;
    }
}

/***************************************************
 BG ビットマップ描画 8bit　フレーム
 ***************************************************/
void draw_bitmap_frame(int x, int y, int w, int h, const u16* data)
{
    u16* screen = current_frame + (y * SCREEN_WIDTH + x) / 2; // VRAMの書き込みは16/32bit単位

    int blank = (SCREEN_WIDTH - w) / 2;
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w / 2; i++) {
            *screen++ = *data++;
        }
        screen += blank;
    }
}

/***************************************************
 FRAME切り替え MODE4/MODE5
 ***************************************************/
void flip_frame()
{
    REG_DISPCNT ^= BACKBUFFER;
    current_frame = (u16*)((u32)current_frame ^ FRAME_FLIP_ADDRESS);
}

/***************************************************
  カレントフレームにビットマップ転送 圧縮LZ77
 ***************************************************/
void load_frame_bitmap_lz77(const u16* data)
{
    LZ77UnCompVram((void*)data, (void*)current_frame);
}

/***************************************************
 カレントフレームにビットマップ転送
 ***************************************************/
void load_frame_bitmap(const u16* data)
{
    CpuFastSet(data, current_frame, (240 * 160 / 4));
}

/***************************************************
 カレントフレームをFILL FILLは8で割り切れること
 ***************************************************/
void fill_frame_bitmap(int x, int y, const u8* color)
{
    int start = (y * SCREEN_WIDTH + x) / 4;
    u32* screen = (u32*)current_frame + start;

    int count = (240 * 160 / 4) - start;
    CpuFastSet(color, screen, (FILL | count));
}
