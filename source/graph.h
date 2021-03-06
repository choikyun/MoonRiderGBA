/**
 * @file graph.h
 * @brief グラフィックのヘッダ
 * @date  2019/05/28
 * @author Choi Gyun
 */

/***************************************************
 * Moon Rider
 * ver 1.0.0
 * 2019.04.19
 * Choi Gyun
 *****************************************************/

#ifndef _graph_h_
#define _graph_h_

#include <gba.h>

#ifdef GLOBAL_VALUE_DEFINE
#define GLOBAL
#define GLOBAL_VAL(v) = (v)
#else
#define GLOBAL extern
#define GLOBAL_VAL(v)
#endif

///////////////////////////////////////////////////////////////////// グローバル変数


///////////////////////////////////////////////////////////////////// プロトタイプ

u16
point16 (int x, int y);
void
pset16 (int x, int y, u16 col);
void
pset16_2x (int x, int y, u16 col);

u8
point8 (u16 *, int x, int y);
void
pset8 (u16 *, int x, int y, u8 col);

void
pset8_frame(int x, int y, u8 col);


#endif	/* _graph_h_ */
