/**
 * @file sprite.h
 * @brief スプライトのヘッダ
 * @date  2019/05/28
 * @author Choi Gyun
 */

/***************************************************
 * Moon Rider
 * ver 1.0.0
 * 2019.04.19
 * Choi Gyun
 *****************************************************/

#ifndef _sprite_h_
#define _sprite_h_

#include <gba.h>

#ifdef GLOBAL_VALUE_DEFINE
#define GLOBAL
#define GLOBAL_VAL(v) = (v)
#else
#define GLOBAL extern
#define GLOBAL_VAL(v)
#endif

///////////////////////////////////////////////////////////////////// 各種定義

// スプライト最大数
#define MAX_SPRITE 128

///////////////////////////////////////////////////////////////////// グローバル変数

//////////////////////////////////////////////////////////////////// プロトタイプ
void init_sprite(void);
void move_sprite(u16, s16, s16);
void set_sprite_form(u16, u16, u16, u16);
void set_sprite_tile(u16, u16);
void init_sprite_chr(void);
void erase_sprite(u16);
s16 read_sprite_x(u16);
s16 read_sprite_y(u16);
void set_scale(u16, u16, u16);
void set_affine_setting(u16, u16, u16);
void set_sprite_priority(u16, u16);

#endif /* _sprite_h_ */
