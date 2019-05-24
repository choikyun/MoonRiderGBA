/**
 * @file score.c
 * @brief スコア
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

#include "game.h"
#include "score.h"

/***************************************************
 スコア 初期化
 ***************************************************/
void init_hiscore(void)
{
    if (SRAMRead32(SRAM_CHECK) != SRAM_ON) {
        hiscore = 0;
        SRAMWrite32(SRAM_HISCORE_NORMAL, 0);
        SRAMWrite32(SRAM_HISCORE_HARD, 0);
        SRAMWrite32(SRAM_CHECK, SRAM_ON);

        SRAMWrite32(SRAM_TROPHY_NORMAL, 0);
        SRAMWrite32(SRAM_TROPHY_NORMAL + 4, 0);
        SRAMWrite32(SRAM_TROPHY_NORMAL + 8, 0);
        SRAMWrite32(SRAM_TROPHY_NORMAL + 12, 0);
        SRAMWrite32(SRAM_TROPHY_NORMAL + 16, 0);
        SRAMWrite32(SRAM_TROPHY_NORMAL + 20, 0);

        SRAMWrite32(SRAM_TROPHY_HARD, 0);
        SRAMWrite32(SRAM_TROPHY_HARD + 4, 0);
        SRAMWrite32(SRAM_TROPHY_HARD + 8, 0);
        SRAMWrite32(SRAM_TROPHY_HARD + 12, 0);
        SRAMWrite32(SRAM_TROPHY_HARD + 16, 0);
        SRAMWrite32(SRAM_TROPHY_HARD + 20, 0);
    }
}

/***************************************************
 ハイスコア クリア
 ***************************************************/
void clear_hiscore(void)
{
    hiscore = 0;
    SRAMWrite32(SRAM_HISCORE_NORMAL, 0);
    SRAMWrite32(SRAM_HISCORE_HARD, 0);

    SRAMWrite32(SRAM_TROPHY_NORMAL, 0);
    SRAMWrite32(SRAM_TROPHY_NORMAL + 4, 0);
    SRAMWrite32(SRAM_TROPHY_NORMAL + 8, 0);
    SRAMWrite32(SRAM_TROPHY_NORMAL + 12, 0);
    SRAMWrite32(SRAM_TROPHY_NORMAL + 16, 0);
    SRAMWrite32(SRAM_TROPHY_NORMAL + 20, 0);

    SRAMWrite32(SRAM_TROPHY_HARD, 0);
    SRAMWrite32(SRAM_TROPHY_HARD + 4, 0);
    SRAMWrite32(SRAM_TROPHY_HARD + 8, 0);
    SRAMWrite32(SRAM_TROPHY_HARD + 12, 0);
    SRAMWrite32(SRAM_TROPHY_HARD + 16, 0);
    SRAMWrite32(SRAM_TROPHY_HARD + 20, 0);
}

/***************************************************
 ハイスコア セーブ
 ***************************************************/
void save_hiscore(int sc)
{
    u32 check = SRAMRead32(SRAM_CHECK);

    if (sc > hiscore && check == SRAM_ON && stage.mode == 0)
        SRAMWrite32(SRAM_HISCORE_NORMAL, sc);
    else if (sc > hiscore && check == SRAM_ON && stage.mode == 1)
        SRAMWrite32(SRAM_HISCORE_HARD, sc);

    // トロフィー
    save_trophy(trophy_unlocked);
}

/***************************************************
 ハイスコア　ロード
 ***************************************************/
int load_hiscore()
{
    int hi;

    if (stage.mode == 0)
        hi = SRAMRead32(SRAM_HISCORE_NORMAL);
    else
        hi = SRAMRead32(SRAM_HISCORE_HARD);

    // トロフィー
    load_trophy(trophy_unlocked);        

    return hi;
}

/***************************************************
トロフィー　セーブ
 ***************************************************/
void
save_trophy (bool *tropthy_unlocked)
{
  if (stage.mode == 0)
  {
    SRAMWrite32(SRAM_TROPHY_NORMAL, tropthy_unlocked[0]);
    SRAMWrite32(SRAM_TROPHY_NORMAL+4, tropthy_unlocked[1]);
    SRAMWrite32(SRAM_TROPHY_NORMAL+8, tropthy_unlocked[2]);
    SRAMWrite32(SRAM_TROPHY_NORMAL+12, tropthy_unlocked[3]);
    SRAMWrite32(SRAM_TROPHY_NORMAL+16, tropthy_unlocked[4]);
    SRAMWrite32(SRAM_TROPHY_NORMAL+20, tropthy_unlocked[5]);
  }
  else
  {
    SRAMWrite32(SRAM_TROPHY_HARD, tropthy_unlocked[0]);
    SRAMWrite32(SRAM_TROPHY_HARD+4, tropthy_unlocked[1]);
    SRAMWrite32(SRAM_TROPHY_HARD+8, tropthy_unlocked[2]);
    SRAMWrite32(SRAM_TROPHY_HARD+12, tropthy_unlocked[3]);
    SRAMWrite32(SRAM_TROPHY_HARD+16, tropthy_unlocked[4]);
    SRAMWrite32(SRAM_TROPHY_HARD+20, tropthy_unlocked[5]);
  }
}

/***************************************************
トロフィー　ロード
 ***************************************************/
void
load_trophy (bool *tropthy_unlocked)
{
  if (stage.mode == 0)
  {
    tropthy_unlocked[0] = SRAMRead32 (SRAM_TROPHY_NORMAL);
    tropthy_unlocked[1] = SRAMRead32 (SRAM_TROPHY_NORMAL+4);
    tropthy_unlocked[2] = SRAMRead32 (SRAM_TROPHY_NORMAL+8);
    tropthy_unlocked[3] = SRAMRead32 (SRAM_TROPHY_NORMAL+12);
    tropthy_unlocked[4] = SRAMRead32 (SRAM_TROPHY_NORMAL+16);
    tropthy_unlocked[5] = SRAMRead32 (SRAM_TROPHY_NORMAL+20);
  }
  else
  {
    tropthy_unlocked[0] = SRAMRead32 (SRAM_TROPHY_HARD);
    tropthy_unlocked[1] = SRAMRead32 (SRAM_TROPHY_HARD+4);
    tropthy_unlocked[2] = SRAMRead32 (SRAM_TROPHY_HARD+8);
    tropthy_unlocked[3] = SRAMRead32 (SRAM_TROPHY_HARD+12);
    tropthy_unlocked[4] = SRAMRead32 (SRAM_TROPHY_HARD+16);
    tropthy_unlocked[5] = SRAMRead32 (SRAM_TROPHY_HARD+20);
  }
}

/***************************************************
 SRAM操作　http://akkera102.sakura.ne.jp/gbadev/index.php?Doc.8%20GBA%A4%CE%BB%C5%CD%CD%A4%CB%A4%C4%A4%A4%A4%C6%28SRAM%29
 より
 ***************************************************/
// READ SRAM
u8 SRAMRead8(u32 offset)
{
    volatile u8* sram = (u8*)SRAM + offset;
    return (u8)sram[0];
}

u16 SRAMRead16(u32 offset)
{
    volatile u8* sram = (u8*)SRAM + offset;
    return (u16)sram[0] | (u16)sram[1] << 8;
}

u32 SRAMRead32(u32 offset)
{
    volatile u8* sram = (u8*)SRAM + offset;

    return (u32)sram[0] | (u32)sram[1] << 8 | (u32)sram[2] << 16 | (u32)sram[3] << 24;
}

// WRITE SRAM
void SRAMWrite8(u32 offset, u8 data)
{
    volatile u8* sram = (u8*)SRAM + offset;
    *sram = (u8)(data & 0xff);
}

void SRAMWrite16(u32 offset, u16 data)
{
    volatile u8* sram = (u8*)SRAM + offset;
    *sram++ = (u8)((data)&0x00ff);
    *sram = (u8)((data >> 8) & 0x00ff);
}

void SRAMWrite32(u32 offset, u32 data)
{
    volatile u8* sram = (u8*)SRAM + offset;
    *sram++ = (u8)((data)&0x000000ff);
    *sram++ = (u8)((data >> 8) & 0x000000ff);
    *sram++ = (u8)((data >> 16) & 0x000000ff);
    *sram = (u8)((data >> 24) & 0x000000ff);
}

u8* SRAMGetPointer()
{
    return (u8*)SRAM;
}
