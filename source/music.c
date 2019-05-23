/**
 * @file music.c
 * @brief BGM, SE
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
#include "music.h"
#include "h/DMA.h"
#include "h/Sound.h"
#include "h/Timer.h"
#include "h/IRQ.h"

const WaveListType MusicList[] =
  {
    //データアドレス, データ長, 優先度(数字が小さいほど高い）
    { m0_bin, SAMPLING_M0, 10 },
    { m1_bin, SAMPLING_M1, 10 },
    { m2_bin, SAMPLING_M2, 10 },
    { m3_bin, SAMPLING_M3, 10 },
    { m4_bin, SAMPLING_M3, 10 },
  };

const WaveListType SoundList[] =
  {
    //データアドレス, データ長, 優先度(数字が小さいほど高い）
    { s0_bin, SAMPLING_S0, 40 }, // 噴射
    { s1_bin, SAMPLING_S1, 60 }, // ブラボー
    { s2_bin, SAMPLING_S2, 5 }, // 爆発
    { s3_bin, SAMPLING_S3, 10 }, // ゲームオーバー
    { s4_bin, SAMPLING_S4, 30}, // レベルアップ
    { s5_bin, SAMPLING_S5, 20 }, // アイテム
    { s6_bin, SAMPLING_S6, 15 }, // 衝突
  };

//*************************************************************
// 曲(A) 停止
//*************************************************************
// DirectSoundAの再生を止める
void
StopMusic ()
{
  DSmusic.ds_Playing = 0;  // 真っ先にOFFにすべきフラグ。割り込み内処理を無効化。

  // DirectSound設定
  if (save_REG_SGCNT0_H & SOUND_DIRECTB_OUTPUT_100)	// Bが鳴っているときです
  {
    save_REG_SGCNT0_H = 		//0;
        //Aは止めますが、音楽（B）を鳴らし続けるため設定を残します
        (
        //SOUND_DIRECTB_FIFO_RESET |	// 鳴らし続けるため、リセットしません
        SOUND_DIRECTB_USE_TIMER_1 |		// 効果音はタイマー1を使用
//				SOUND_DIRECTA_RIGHT_OUTPUT |	// DirectSoundAの右出力ON
//				SOUND_DIRECTA_LEFT_OUTPUT |		// DirectSoundAの左出力ON
            SOUND_DIRECTB_RIGHT_OUTPUT |	// DirectSoundBの右出力ON
            SOUND_DIRECTB_LEFT_OUTPUT |		// DirectSoundBの左出力ON
//				SOUND_DIRECTA_OUTPUT_100 |		// DirectSoundA出力レート
            SOUND_DIRECTB_OUTPUT_100 //|	// DirectSoundB出力レート
        //SOUND_ALL_OUTPUT_RATIO_100	// SOUND1～4出力レート
        );
  }
  else
  {
    save_REG_SGCNT0_H = 0;
  }
  REG_SGCNT0_H = save_REG_SGCNT0_H;
  REG_DM1CNT_H = 0;
  REG_TM0CNT_H = 0;
  DSmusic.ds_Count = 0;
  DSmusic.ds_Playing = 0;
  // DSmusic.ds_snd_no = 0;		//ループ再生のためこの項目はクリアしません
}
//*************************************************************
// 効果音(B) 停止
//*************************************************************
void
StopSound ()
{
  DSeffect.ds_Playing = 0;  // 真っ先にOFFにすべきフラグ。割り込み内処理を無効化。

  if (save_REG_SGCNT0_H & SOUND_DIRECTA_OUTPUT_100)	// A が鳴っているときです
  {
    save_REG_SGCNT0_H = // 効果音(B)は止めますが、音楽(A)を鳴らし続けるため設定を残します
        (
        //SOUND_DIRECTA_FIFO_RESET | 	// リセットはしません
        SOUND_DIRECTA_USE_TIMER_0 |		// 楽曲はタイマー0を使用
            SOUND_DIRECTA_RIGHT_OUTPUT |	// DirectSoundAの右出力ON
            SOUND_DIRECTA_LEFT_OUTPUT |		// DirectSoundAの左出力ON
//			SOUND_DIRECTB_RIGHT_OUTPUT |	// DirectSoundBの右出力ON
//			SOUND_DIRECTB_LEFT_OUTPUT |		// DirectSoundBの左出力ON
            SOUND_DIRECTA_OUTPUT_100 //|		// DirectSoundA出力レート
//			SOUND_DIRECTB_OUTPUT_100 //|	// DirectSoundB出力レート
            //SOUND_ALL_OUTPUT_RATIO_100 	// SOUND1～4出力レート
        );
  }
  else
  {
    save_REG_SGCNT0_H = 0; // 音楽(A)が鳴っていなければ0クリアするだけ
  }
  REG_SGCNT0_H = save_REG_SGCNT0_H;
  REG_DM2CNT_H = 0;
  REG_TM1CNT_H = 0;
  DSeffect.ds_Count = 0;
  DSeffect.ds_Playing = 0;
}

//*************************************************************
// 曲(A) 再生
// 引数：サウンドデータの配列の最初のアドレス
//*************************************************************
void
StartMusic (u32* sound_src)
{
  DSOUND_FIFOA = 0;

  if (DSmusic.ds_Playing == 1)
  {
    StopMusic ();	// 曲 停止
  }

  REG_SGCNT1 = ALL_SOUND_ENABLE;		// サウンドの使用

  // DirectSound設定
  save_REG_SGCNT0_H |= (					//「|=」なのでSOUNDBの設定はそのままです
      SOUND_DIRECTA_FIFO_RESET |		// 最初から鳴らすためリセットします
          SOUND_DIRECTA_USE_TIMER_0 |		// 楽曲はタイマー0を使用
          SOUND_DIRECTA_RIGHT_OUTPUT |	// DirectSoundAの右出力ON
          SOUND_DIRECTA_LEFT_OUTPUT |		// DirectSoundAの左出力ON
          SOUND_DIRECTB_RIGHT_OUTPUT |	// DirectSoundBの右出力ON
          SOUND_DIRECTB_LEFT_OUTPUT |		// DirectSoundBの左出力ON
          SOUND_DIRECTA_OUTPUT_100 |		// DirectSoundA出力レート
          SOUND_DIRECTB_OUTPUT_100 //|	// DirectSoundB出力レート
      //SOUND_ALL_OUTPUT_RATIO_100	// SOUND1～4出力レート
      );
  REG_SGCNT0_H = save_REG_SGCNT0_H;
  REG_DM1SAD = (u32) sound_src;  // 転送するWave（曲）データのアドレス
  REG_DM1DAD = 0x040000a0; // REG_SGFIF0A // DMA転送先アドレス

  // タイマー0の値を設定 u16の－値がサンプリングレート(周波数)になる
  // 16777216/(-x)
  //REG_TM0D = (u16)(-2048);
  // タイマー0の値を設定 u16の－値がサンプリングレート(周波数)になる
  // 44KHzのWAVEデータの場合 -380
  // 22KHzの場合 -760
  // 11KHzの場合 -1520
  //*(u16*)0x4000100 = (u16)(-1520);
  REG_TM0D = (u16) (-1520);

  // DMA転送設定
  REG_DM1CNT_H = (DMA_TRANSFER_ON |	// 転送開始
      DMA_INTR_OFF |		// 割り込み不使用
      DMA_TIMING_FIFO | 	// (DMA1 2にしか設定できません)
      DMA_SIZE_32 |		// 一度の転送サイズ
      DMA_REPEAT_ON |		// リピート設定
      DMA_SAD_INC |		// 転送元++
      DMA_DAD_FIX			// 転送先固定
  );

  // タイマー0動作開始。周期は１クロックで１進む
  REG_TM0CNT_H = TM_FREQ_PER_1 | TM_USEIRQ | TM_ENABLE;

  DSmusic.ds_Playing = 1;
}

//*************************************************************
// 効果音(B) 再生
// 引数：サウンドデータの配列の最初のアドレス
//*************************************************************
void
StartSound (u32* sound_src)
{
  DSOUND_FIFOB = 0;

  // あまり割りこみをさせないと良くないのでコメントにします。
  //REG_IME      =  IRQ_MASTER_OFF;  // 割り込み有効/無効の切り替えを行います。0:無効 1:有効
  if (DSeffect.ds_Playing == 1) // すでに音を再生中のときは停止します
  {
    StopSound ();
  }

  // Sound Enable
  REG_SGCNT1 = ALL_SOUND_ENABLE; //サウンドの使用

  save_REG_SGCNT0_H |= 	//「|=」により、DIRECTAの設定は残します
      (SOUND_DIRECTB_FIFO_RESET |		// 効果音の発音をリセットします
          SOUND_DIRECTB_USE_TIMER_1 |		// 効果音はタイマー1を使用
          SOUND_DIRECTA_RIGHT_OUTPUT |	// DirectSoundAの右出力ON
          SOUND_DIRECTA_LEFT_OUTPUT |		// DirectSoundAの左出力ON
          SOUND_DIRECTB_RIGHT_OUTPUT |	// DirectSoundBの右出力ON
          SOUND_DIRECTB_LEFT_OUTPUT |		// DirectSoundBの左出力ON
          SOUND_DIRECTA_OUTPUT_100 |		// DirectSoundA出力レート
          SOUND_DIRECTB_OUTPUT_100 //|	// DirectSoundB出力レート
      //SOUND_ALL_OUTPUT_RATIO_100		// SOUND1～4出力レート
      );
  REG_SGCNT0_H = save_REG_SGCNT0_H;
  REG_DM2SAD = (u32) sound_src;  // 転送するWaveデータのアドレス
  REG_DM2DAD = 0x040000a4; // REG_SGFIF0B // DMA転送先アドレス

  // タイマー1の値を設定 u16の－値がサンプリングレート(周波数)になる
  //REG_TM0D = (u16)(-1520);
  REG_TM1D = (u16) (-1520);

  // DMA転送設定
  // REG_DM2CNT_L = 0;		//「すずめ愛好会」より「DMAの転送サイズの値は無視されるので...」
  // ※つまり、転送終了の割り込みをかけられないということです。
  REG_DM2CNT_H = (DMA_TRANSFER_ON |	// 転送開始
      DMA_INTR_OFF | 		// 割り込み不使用
      DMA_TIMING_FIFO |	// (DMA1 2にしか設定できません)
      //DMA_TIMING_VBLANK |
      DMA_SIZE_32 |		// 一度の転送サイズ
      DMA_REPEAT_ON |		// リピート設定
      DMA_SAD_INC |		// 転送元++
      DMA_DAD_FIX			// 転送先固定
  );

  // タイマー1動作開始。周期は１クロックで１進む
  REG_TM1CNT_H = TM_FREQ_PER_1 | TM_USEIRQ | TM_ENABLE;

  DSeffect.ds_Playing = 1;

  // REG_IME		=	IRQ_MASTER_ON;  // 割り込み有効/無効の切り替えを行います。0:無効 1:有効
}
//*************************************************************
// サウンドのワークエリア変数すべての初期設定
//*************************************************************
void
DirectSoundInitialize ()
{
  save_REG_SGCNT0_H = 0;

  DSmusic.ds_Playing = 0;
  DSmusic.ds_Count = 0;
  DSeffect.ds_Playing = 0;
  DSeffect.ds_Count = 0;
  DSmusic.ds_snd_no = 0;
  DSeffect.ds_snd_no = 0;
  DSmusic.loopflg = 0;
  DSeffect.loopflg = 0;
}
//*************************************************************
// 楽曲の再生  引数= 曲番号、ループ
//*************************************************************
void
PlayMusic (int _no, int loop)
{
  if (_no >= MAX_MUSIC)
    return;
  StartMusic ((u32*) MusicList[_no].dataPtr);
  DSmusic.ds_snd_no = _no;  // 現在再生中の楽曲の番号
  DSmusic.loopflg = loop; // ループ演奏 0:ループしません 1:ループ
}

//*************************************************************
// 効果音の再生  引数=効果音番号
//*************************************************************
void
PlaySound (int _no)
{
  if (_no >= MAX_SOUND)
    return;

  // 優先度
  if (DSeffect.ds_Playing == 1
      && SoundList[DSeffect.ds_snd_no].priority < SoundList[_no].priority)
    return;

  StartSound ((u32*) SoundList[_no].dataPtr);
  DSeffect.ds_snd_no = _no; // 現在再生中の効果音の番号
  DSeffect.loopflg = 0;   // ループ演奏 0:ループしません 1:ループ
}
//*************************************************************
//IRQハンドラ処理
//*************************************************************
void
IRQ_Music ()
{
  if (DSmusic.ds_Playing == 1)     // 楽曲再生中フラグ 0:停止 1:再生中
  {
    DSmusic.ds_Count++;			// データ（BYTE）カウンタ++

    //-20は、処理が遅れたときにプチノイズがのるので有る程度防止するため
    if (DSmusic.ds_Count >= (MusicList[DSmusic.ds_snd_no].dataMax - 20))
    {
      StopMusic ();
    }

    //上のifで曲を停止したとき、ループ再生が必要なら再生
    if (DSmusic.ds_Playing == 0)
    {
      if (DSmusic.loopflg == 1)
      {
        PlayMusic (DSmusic.ds_snd_no, PLAY_LOOP_ON); // ループ再生
      }
    }
  }
}

//*************************************************************
//IRQハンドラ処理
//*************************************************************
void
IRQ_Sound ()
{
  if (DSeffect.ds_Playing == 1)     // 効果音再生中フラグ 0:停止 1:再生中
  {
    DSeffect.ds_Count++;
    //-20は、処理が遅れたときにプチノイズがのるので有る程度防止するため
    if (DSeffect.ds_Count >= (SoundList[DSeffect.ds_snd_no].dataMax - 20))
    {
      StopSound ();
    }
  }
}
