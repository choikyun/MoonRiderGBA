/**
 * @file graph.c
 * @brief グラフィック
 * @date  2019/05/28
 * @author Choi Gyun
 */

/***************************************************
 * Moon Rider
 * ver 1.0.0
 * 2019.04.19
 * Choi Gyun
 *****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <gba.h>
#include "graph.h"
#include "bg.h"


/*********************************************************************
 * MODE3
 * 15bit
 *********************************************************************/

/***************************************************
 ポイント
 ***************************************************/
u16
point16 (int x, int y)
{
  u16 *screen = (u16 *) VRAM;

  return *(screen + y * SCREEN_WIDTH + x);
}


/***************************************************
 ポイントセット
 ***************************************************/
void
pset16 (int x, int y, u16 col)
{
  u16 *screen = (u16 *) VRAM;

  screen += y * SCREEN_WIDTH + x;
  *screen = col;
}


/***************************************************
 ポイントセット 2pix
 ***************************************************/
void
pset16_2x (int x, int y, u16 col)
{
  u16 *screen = (u16 *) VRAM;

  screen += y * SCREEN_WIDTH + x;

  *screen = col;
  *(screen + 1) = col;
  *(screen + SCREEN_WIDTH) = col;
  *(screen + SCREEN_WIDTH + 1) = col;
}



/*********************************************************************
 * MODE4, MODE5
 * 8bit モード
 *********************************************************************/

/***************************************************
 ポイント
 ***************************************************/
u8
point8 (u16 *buf, int x, int y)
{
  u16 *dst = buf + (y * SCREEN_WIDTH + x) / 2;

  if(x & 1)
      return (u8)((*dst >> 8) & 0xff);
  else
      return (u8)(*dst & ~0xff);
}


/***************************************************
 ポイントセット
 ***************************************************/
void
pset8(u16 *buf, int x, int y, u8 col)
{
    u16 *dst = buf + (y * SCREEN_WIDTH + x) / 2;

    if(x & 1)
        *dst= (*dst & 0xff) | (col << 8);
    else
        *dst= (*dst & ~0xff) | col;
}

/***************************************************
 ポイントセット カレントフレーム
 ***************************************************/
void
pset8_frame(int x, int y, u8 col)
{
    u16 *dst = current_frame + (y * SCREEN_WIDTH + x) / 2;

    if(x & 1)
        *dst= (*dst & 0xff) | (col << 8);
    else
        *dst= (*dst & ~0xff) | col;
}



