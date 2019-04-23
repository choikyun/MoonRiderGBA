
// DMAの設定

#ifndef DMA_H
#define DMA_H


// DMA0転送
#define DMA0TRANSFER(sad,dad,size,mode)	\
	REG_DM0SAD = sad;	\
	REG_DM0DAD = dad;	\
	REG_DM0CNT_L = size;	\
	REG_DM0CNT_H = mode

// DMA1転送
#define DMA1TRANSFER(sad,dad,size,mode)	\
	REG_DM1SAD = sad;	\
	REG_DM1DAD = dad;	\
	REG_DM1CNT_L = size;	\
	REG_DM1CNT_H = mode

// DMA2転送
#define DMA2TRANSFER(sad,dad,size,mode)	\
	REG_DM2SAD = sad;	\
	REG_DM2DAD = dad;	\
	REG_DM2CNT_L = size;	\
	REG_DM2CNT_H = mode

// DMA3転送
#define DMA3TRANSFER(sad,dad,size,mode)	\
	REG_DM3SAD = sad;	\
	REG_DM3DAD = dad;	\
	REG_DM3CNT_L = size;	\
	REG_DM3CNT_H = mode




// 転送開始・中止
#define DMA_TRANSFER_ON		(1<<15)
#define DMA_TRANSFER_OFF	(0<<15)

// 割り込み使用・不使用
#define DMA_INTR_ON		(1<<14)
#define DMA_INTR_OFF	(0<<14)

// 転送タイミング
#define DMA_TIMING_NOW		(0<<12) // すぐに開始
#define DMA_TIMING_VBLANK	(1<<12) // Vblankに開始
#define DMA_TIMING_HBLANK	(2<<12) // Hblankに開始
#define DMA_TIMING_FIFO		(3<<12) // FIFOリクエストで開始 DMA1CNT_H のみ
#define DMA_TIMING_DRAWLINE	(3<<12) // ライン描画時に開始 DMA3CNT_H のみ

#define DMA_ROM_REQUEST		(1<<11) // ROMからリクエストで開始 DMA3CNT_H のみ

// 一度の転送サイズ
#define DMA_SIZE_16		(0<<10) // 16bitずつ size/2
#define DMA_SIZE_32		(1<<10) // 32bitずつ size/4

// リピート設定
#define DMA_REPEAT_ON	(1<<9)
#define DMA_REPEAT_OFF	(0<<9)

// 転送元アドレスの増減設定
#define DMA_SAD_INC		(0<<7) // 転送ごとにインクリメント
#define DMA_SAD_DEC		(1<<7) // 転送ごとにデクリメント
#define DMA_SAD_FIX		(2<<7) // 固定

// 転送先アドレスの増減設定
#define DMA_DAD_INC		(0<<5) // 転送ごとにインクリメント
#define DMA_DAD_DEC		(1<<5) // 転送ごとにデクリメント
#define DMA_DAD_FIX		(2<<5) // 固定
#define DMA_DAD_RESET	(3<<5) // インクリメント、転送終了時にリセット



#endif

