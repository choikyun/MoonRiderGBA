
#ifndef SOUND_H
#define SOUND_H


#define REG_SGCNT0     *(u32*)0x4000080
#define REG_SGCNT0_L   *(u16*)0x4000080
#define REG_SGCNT0_H   *(u16*)0x4000082

#define REG_SGCNT1     *(u16*)0x4000084

#define REG_DM1CNT     *(u32*)0x40000C4
#define REG_DM1CNT_L   *(u16*)0x40000C4
#define REG_DM1CNT_H   *(u16*)0x40000C6

#define REG_DM1SAD     *(u32*)0x40000BC
#define REG_DM1SAD_L   *(u16*)0x40000BC
#define REG_DM1SAD_H   *(u16*)0x40000BE
#define REG_DM1DAD     *(u32*)0x40000C0
#define REG_DM1DAD_L   *(u16*)0x40000C0
#define REG_DM1DAD_H   *(u16*)0x40000C2

#define REG_DM2CNT     *(u32*)0x40000D0
#define REG_DM2CNT_L   *(u16*)0x40000D0
#define REG_DM2CNT_H   *(u16*)0x40000D2

#define REG_DM2SAD     *(u32*)0x40000C8
#define REG_DM2SAD_L   *(u16*)0x40000C8
#define REG_DM2SAD_H   *(u16*)0x40000CA
#define REG_DM2DAD     *(u32*)0x40000CC
#define REG_DM2DAD_L   *(u16*)0x40000CC
#define REG_DM2DAD_H   *(u16*)0x40000CE

#define REG_TM0D       *(u16*)0x4000100
#define REG_TM1D       *(u16*)0x4000104




// サウンドの設定

// REG_SGCNT0_L

#define DMG_LEFT_VOLUME(x)	((x)<<0) // 0-7
#define DMG_RIGHT_VOLUME(x)	((x)<<4) // 0-7

#define SOUND1_LEFT_OUTPUT	(1<<8)
#define SOUND2_LEFT_OUTPUT	(1<<9)
#define SOUND3_LEFT_OUTPUT	(1<<10)
#define SOUND4_LEFT_OUTPUT	(1<<11)
#define SOUND1_RIGHT_OUTPUT	(1<<12)
#define SOUND2_RIGHT_OUTPUT	(1<<13)
#define SOUND3_RIGHT_OUTPUT	(1<<14)
#define SOUND4_RIGHT_OUTPUT	(1<<15)


// REG_SGCNT0_H   0x82

// サウンド出力レシオ 0-2 (0=25%,1=50%,2=100%)
#define SOUND_ALL_OUTPUT_RATIO_25	(0)
#define SOUND_ALL_OUTPUT_RATIO_50	(1)
#define SOUND_ALL_OUTPUT_RATIO_100	(2)
// DirectSound 出力レシオ
#define SOUND_DIRECTA_OUTPUT_50		(0<<2)
#define SOUND_DIRECTA_OUTPUT_100	(1<<2)
#define SOUND_DIRECTB_OUTPUT_50		(0<<3)
#define SOUND_DIRECTB_OUTPUT_100	(1<<3)
// DirectSoundA を左右スピーカーに出力するか
#define SOUND_DIRECTA_RIGHT_OUTPUT	(1<<8)
#define SOUND_DIRECTA_LEFT_OUTPUT	(1<<9)
// タイマー0と１のどちらを使用するか
#define SOUND_DIRECTA_USE_TIMER_0		(0<<10)
#define SOUND_DIRECTA_USE_TIMER_1		(1<<10)
// DirectSoundA FIFO リセット
#define SOUND_DIRECTA_FIFO_RESET		(1<<11)
// DirectSoundA を左右スピーカーに出力するか
#define SOUND_DIRECTB_RIGHT_OUTPUT	(1<<12)
#define SOUND_DIRECTB_LEFT_OUTPUT	(1<<13)
// タイマー0と１のどちらを使用するか
#define SOUND_DIRECTB_USE_TIMER_0		(0<<14)
#define SOUND_DIRECTB_USE_TIMER_1		(1<<14)
// DirectSoundB FIFO リセット
#define SOUND_DIRECTB_FIFO_RESET		(1<<15)



// REG_SGCNT1

#define SOUND1_STATUS	(1<<0) // サウンドの状態
#define SOUND2_STATUS	(1<<1) // サウンドの状態
#define SOUND3_STATUS	(1<<2) // サウンドの状態
#define SOUND4_STATUS	(1<<3) // サウンドの状態
#define ALL_SOUND_ENABLE	(1<<7) // 全サウンド使用



// サウンド1
// スィープのあるエンベロープ音源

// REG_SG10_L

#define SOUND1_SWEEP_SHIFT(x)	((x)<<0) // スィープシフト 0-7
#define SOUND1_SWEEP_INC		(0<<3) // スィープ増加
#define SOUND1_SWEEP_DEC		(1<<3) // スィープ現象
#define SOUND1_SWEEP_TIME(x)	((x)<<4)	// スィープ時間 0-7

// REG_SG10_H

#define SOUND1_LENGTH(x)	((x)<<0) // サウンドの長さ 0-63
#define SOUND1_DUTY_CYCLE(x)	((x)<<6) // デューティサイクル 0-3
#define SOUND1_ENVELOP_STEP(x)	((x)<<8) // エンベロープステップ時間 0-7
#define SOUND1_ENVELOP_DEC		((0)<<11) // エンベロープ減少
#define SOUND1_ENVELOP_INC		((1)<<11) // エンベロープ増加
#define SOUND1_ENVELOP_VALUE(x)	((x)<<12) // エンベロープ初期値 0-15


// REG_SG11

#define SOUND1_FREQ(x)	((x)<<0) // 周期 0-2048
#define SOUND1_CONTINUE	(0<<14) // 継続
#define SOUND1_TIMED	(1<<14) // 時間制限
#define SOUND1_RESET	(1<<15) // サウンド1リセット


// サウンド２　スィープのないエンベロープ音源
// サウンド1のREG_SG10_H,REG_SG11と同じ

// REG_SG20
#define SOUND2_LENGTH(x)	((x)<<0) // サウンドの長さ 0-63
#define SOUND2_DUTY_CYCLE(x)	((x)<<6) // デューティサイクル 0-3
#define SOUND2_ENVELOP_STEP(x)	((x)<<8) // エンベロープステップ時間 0-7
#define SOUND2_ENVELOP_DEC		((0)<<11) // エンベロープ現象
#define SOUND2_ENVELOP_INC		((1)<<11) // エンベロープ増加
#define SOUND2_ENVELOP_VALUE(x)	((x)<<12) // エンベロープ初期値 0-15

// REG_SG21
#define SOUND2_FREQ(x)	((x)<<0) // 周期 0-2048
#define SOUND2_CONTINUE	(0<<14) // 継続
#define SOUND2_TIMED	(1<<14) // 時間制限
#define SOUND2_RESET	(1<<15) // サウンド1リセット


// サウンド３
// 

// REG_SG30_L

#define SOUND3_BANK_2x32	(0<<5) // 32bitのバンク二つ
#define SOUND3_BANK_1x64	(1<<5) // 64bitのバンク一つ

// バンクを選択 選択していないほうが書き込める
#define SOUND3_BANK_SELECT(x)	((x)<<6) // 0-1

#define SOUND3_OUTPUT_ENABLE	(1<<7) // サウンド3出力
#define SOUND3_OUTPUT_DISABLE	(0<<7) // サウンド3出力しない


// REG_SG30_H

#define SOUND3_LENGTH(x)	((x)<<0)	// サウンドの長さ 0-255
#define SOUND3_VOLUME_RATIO(x)	((x)<<13)	// ボリューム出力レシオ 0-4 1が100%


// REG_SG31

#define SOUND3_FREQ(x)	((x)<<0) // 周期 0-2048
#define SOUND3_CONTINUE	(0<<14) // 継続
#define SOUND3_TIMED	(1<<14) // 時間制限
#define SOUND3_RESET	(1<<15) // サウンド1リセット


// REG_SGWR0
// REG_SGWR0_L
// REG_SGWR0_H
// …REG_SGWR3


// サウンド4

// REG_SG40
#define SOUND4_LENGTH(x)	((x)<<0) // サウンドの長さ 0-63
#define SOUND4_ENVELOP_STEP(x)	((x)<<8) // エンベロープステップ時間 0-7
#define SOUND4_ENVELOP_DEC		((0)<<11) // エンベロープ現象
#define SOUND4_ENVELOP_INC		((1)<<11) // エンベロープ増加
#define SOUND4_ENVELOP_VALUE(x)	((x)<<12) // エンベロープ初期値 0-15


// REG_SG41

#define SOUND4_DIVIDE_FREQ(x)	((x)<<0) // 周期 0-7
#define SOUND4_STAGE_15	(0<<3)	// カウンタステージ
#define SOUND4_STAGE_7	(1<<3)	// 

#define SOUND4_STAGE_FREQ(x) ((x)<<4) // ステップごとの周期 0-13
#define SOUND4_CONTINUE	(0<<14) // 継続
#define SOUND4_TIMED	(1<<14) // 時間制限
#define SOUND4_RESET	(1<<15) // サウンド1リセット



// REG_SGCNT_H

// GB4チャンネルの出力レシオ　２５％、５０％、１００％
#define SOUND_GB_RATIO_25	0
#define SOUND_GB_RATIO_50	1
#define SOUND_GB_RATIO_100	2

// DirectSound A・B 出力レシオ　５０％、１００％
#define SOUND_DSOUNDA_RATIO_50	0
#define SOUND_DSOUNDA_RATIO_100	(1<<2)
#define SOUND_DSOUNDB_RATIO_50	0
#define SOUND_DSOUNDB_RATIO_100	(1<<3)

// DirectSoundAを左右のスピーカーに出力するか
#define SOUND_DSOUNDA_OUTPUT_RIGHT	(1<<8)
#define SOUND_DSOUNDA_OUTPUT_LEFT	(1<<9)

// DirectSoundAでタイマー0かタイマー１のどちらを使用するか
#define SOUND_DSOUNDA_SAMP_TIMER0	0
#define SOUND_DSOUNDA_SAMP_TIMER1	(1<<10)

// DirectSoundAでFIFOをリセットする
#define SOUND_DSOUNDA_FIFO_RESET	(1<<11)


// DirectSoundBを左右のスピーカーに出力するか
#define SOUND_DSOUNDB_OUTPUT_RIGHT	(1<<12)
#define SOUND_DSOUNDB_OUTPUT_LEFT	(1<<13)

// DirectSoundBでタイマー0かタイマー１のどちらを使用するか
#define SOUND_DSOUNDB_SAMP_TIMER0	0
#define SOUND_DSOUNDB_SAMP_TIMER1	(1<<14)

// DirectSoundBでFIFOをリセットする
#define SOUND_DSOUNDB_FIFO_RESET	(1<<15)



#endif

