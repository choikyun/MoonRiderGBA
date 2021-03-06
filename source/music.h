/**
 * @file music.h
 * @brief BGM, SEのヘッダ
 * @date  2019/05/28
 * @author Choi Gyun
 */

/***************************************************
 * Moon Rider
 * ver 1.0.0
 * 2019.04.19
 * Choi Gyun
 *****************************************************/

#ifndef _music_h_
#define _music_h_

#include <gba.h>
#include "h/DMA.h"
#include "h/Sound.h"
#include "h/Timer.h"
#include "h/IRQ.h"

#ifdef GLOBAL_VALUE_DEFINE
#define GLOBAL
#define GLOBAL_VAL(v) = (v)
#else
#define GLOBAL extern
#define GLOBAL_VAL(v)
#endif

#define MAX_MUSIC (9)
#define MAX_SOUND (9)

////// http://f1.aaa.livedoor.jp/~progrm/gba/ よりほぼ丸々引用

//*************************************************************
// 楽曲データ
// DirectsoundAで再生
//*************************************************************
extern u8 m0_bin[]; // 曲0 タイトル
extern u8 m1_bin[]; // 曲1 ステージ
extern u8 m2_bin[]; // 曲2 ステージ
extern u8 m3_bin[]; // 曲3 ステージ
extern u8 m4_bin[]; // 曲4 ステージ

#define SAMPLING_M0 (264914)
#define SAMPLING_M1 (151199)
#define SAMPLING_M2 (132299)
#define SAMPLING_M3 (145985)
#define SAMPLING_M4 (162831)

typedef struct
{
    u8 *dataPtr;   // データアドレス
    u32 dataMax;
    int priority;
} WaveListType;

enum
{
  MUSIC_TITLE, MUSIC_STAGE,
};

//*************************************************************
// 効果音データ
// DirectsoundBで再生
//*************************************************************
extern u8 s0_bin[]; // 効果音0　噴射
extern u8 s1_bin[]; // 効果音1　ブラボー
extern u8 s2_bin[]; // 効果音2　爆発
extern u8 s3_bin[]; // 効果音3　ゲームオーバー
extern u8 s4_bin[]; // 効果音4　レベルアップ
extern u8 s5_bin[]; // 効果音5　アイテム
extern u8 s6_bin[]; // 効果音6　衝突


#define SAMPLING_S0 (8000)
#define SAMPLING_S1 (12809)
#define SAMPLING_S2 (4421)
#define SAMPLING_S3 (16269)
#define SAMPLING_S4 (6707)
#define SAMPLING_S5 (2905)
#define SAMPLING_S6 (2621)

enum
{
  SOUND_BOOSTER, SOUND_BRAVO, SOUND_BOMB, SOUND_OVER, SOUND_LEVELUP, SOUND_ITEM, SOUND_CRASH,
};

//*************************************************************
// DIRECTサウンド制御構造体
//*************************************************************
struct ds_struc
{
    u32 ds_Count;       // データ（BYTE）カウンタ サンプリングカウント用変数
    u8 ds_Playing;     // 楽曲再生中フラグ 0:停止 1:再生中
    u8 ds_snd_no;      // 再生中の楽曲（効果音）の種類を示す番号 0～
    u8 loopflg;        // ループ演奏 0:ループしません 1:ループ
};

GLOBAL struct ds_struc DSmusic;	// 楽曲用
GLOBAL struct ds_struc DSeffect;	// 効果音用

GLOBAL u16 save_REG_SGCNT0_H;

#define PLAY_LOOP_ON  (1) // ループ演奏 1:ループ
#define PLAY_LOOP_OFF (0) // ループ演奏 0:ループ無し

//////////////////////////////////////////////////////////////////// プロトタイプ
void
StopSound ();
void
StopMusic ();
void
PlayMusic (int _no, int loop);
void
PlaySound (int _no);
void
DirectSoundInitialize ();

void
IRQ_Music ();
void
IRQ_Sound ();

#endif	/* _music_h_ */
