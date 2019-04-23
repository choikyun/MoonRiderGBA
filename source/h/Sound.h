
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




// �T�E���h�̐ݒ�

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

// �T�E���h�o�̓��V�I 0-2 (0=25%,1=50%,2=100%)
#define SOUND_ALL_OUTPUT_RATIO_25	(0)
#define SOUND_ALL_OUTPUT_RATIO_50	(1)
#define SOUND_ALL_OUTPUT_RATIO_100	(2)
// DirectSound �o�̓��V�I
#define SOUND_DIRECTA_OUTPUT_50		(0<<2)
#define SOUND_DIRECTA_OUTPUT_100	(1<<2)
#define SOUND_DIRECTB_OUTPUT_50		(0<<3)
#define SOUND_DIRECTB_OUTPUT_100	(1<<3)
// DirectSoundA �����E�X�s�[�J�[�ɏo�͂��邩
#define SOUND_DIRECTA_RIGHT_OUTPUT	(1<<8)
#define SOUND_DIRECTA_LEFT_OUTPUT	(1<<9)
// �^�C�}�[0�ƂP�̂ǂ�����g�p���邩
#define SOUND_DIRECTA_USE_TIMER_0		(0<<10)
#define SOUND_DIRECTA_USE_TIMER_1		(1<<10)
// DirectSoundA FIFO ���Z�b�g
#define SOUND_DIRECTA_FIFO_RESET		(1<<11)
// DirectSoundA �����E�X�s�[�J�[�ɏo�͂��邩
#define SOUND_DIRECTB_RIGHT_OUTPUT	(1<<12)
#define SOUND_DIRECTB_LEFT_OUTPUT	(1<<13)
// �^�C�}�[0�ƂP�̂ǂ�����g�p���邩
#define SOUND_DIRECTB_USE_TIMER_0		(0<<14)
#define SOUND_DIRECTB_USE_TIMER_1		(1<<14)
// DirectSoundB FIFO ���Z�b�g
#define SOUND_DIRECTB_FIFO_RESET		(1<<15)



// REG_SGCNT1

#define SOUND1_STATUS	(1<<0) // �T�E���h�̏��
#define SOUND2_STATUS	(1<<1) // �T�E���h�̏��
#define SOUND3_STATUS	(1<<2) // �T�E���h�̏��
#define SOUND4_STATUS	(1<<3) // �T�E���h�̏��
#define ALL_SOUND_ENABLE	(1<<7) // �S�T�E���h�g�p



// �T�E���h1
// �X�B�[�v�̂���G���x���[�v����

// REG_SG10_L

#define SOUND1_SWEEP_SHIFT(x)	((x)<<0) // �X�B�[�v�V�t�g 0-7
#define SOUND1_SWEEP_INC		(0<<3) // �X�B�[�v����
#define SOUND1_SWEEP_DEC		(1<<3) // �X�B�[�v����
#define SOUND1_SWEEP_TIME(x)	((x)<<4)	// �X�B�[�v���� 0-7

// REG_SG10_H

#define SOUND1_LENGTH(x)	((x)<<0) // �T�E���h�̒��� 0-63
#define SOUND1_DUTY_CYCLE(x)	((x)<<6) // �f���[�e�B�T�C�N�� 0-3
#define SOUND1_ENVELOP_STEP(x)	((x)<<8) // �G���x���[�v�X�e�b�v���� 0-7
#define SOUND1_ENVELOP_DEC		((0)<<11) // �G���x���[�v����
#define SOUND1_ENVELOP_INC		((1)<<11) // �G���x���[�v����
#define SOUND1_ENVELOP_VALUE(x)	((x)<<12) // �G���x���[�v�����l 0-15


// REG_SG11

#define SOUND1_FREQ(x)	((x)<<0) // ���� 0-2048
#define SOUND1_CONTINUE	(0<<14) // �p��
#define SOUND1_TIMED	(1<<14) // ���Ԑ���
#define SOUND1_RESET	(1<<15) // �T�E���h1���Z�b�g


// �T�E���h�Q�@�X�B�[�v�̂Ȃ��G���x���[�v����
// �T�E���h1��REG_SG10_H,REG_SG11�Ɠ���

// REG_SG20
#define SOUND2_LENGTH(x)	((x)<<0) // �T�E���h�̒��� 0-63
#define SOUND2_DUTY_CYCLE(x)	((x)<<6) // �f���[�e�B�T�C�N�� 0-3
#define SOUND2_ENVELOP_STEP(x)	((x)<<8) // �G���x���[�v�X�e�b�v���� 0-7
#define SOUND2_ENVELOP_DEC		((0)<<11) // �G���x���[�v����
#define SOUND2_ENVELOP_INC		((1)<<11) // �G���x���[�v����
#define SOUND2_ENVELOP_VALUE(x)	((x)<<12) // �G���x���[�v�����l 0-15

// REG_SG21
#define SOUND2_FREQ(x)	((x)<<0) // ���� 0-2048
#define SOUND2_CONTINUE	(0<<14) // �p��
#define SOUND2_TIMED	(1<<14) // ���Ԑ���
#define SOUND2_RESET	(1<<15) // �T�E���h1���Z�b�g


// �T�E���h�R
// 

// REG_SG30_L

#define SOUND3_BANK_2x32	(0<<5) // 32bit�̃o���N���
#define SOUND3_BANK_1x64	(1<<5) // 64bit�̃o���N���

// �o���N��I�� �I�����Ă��Ȃ��ق����������߂�
#define SOUND3_BANK_SELECT(x)	((x)<<6) // 0-1

#define SOUND3_OUTPUT_ENABLE	(1<<7) // �T�E���h3�o��
#define SOUND3_OUTPUT_DISABLE	(0<<7) // �T�E���h3�o�͂��Ȃ�


// REG_SG30_H

#define SOUND3_LENGTH(x)	((x)<<0)	// �T�E���h�̒��� 0-255
#define SOUND3_VOLUME_RATIO(x)	((x)<<13)	// �{�����[���o�̓��V�I 0-4 1��100%


// REG_SG31

#define SOUND3_FREQ(x)	((x)<<0) // ���� 0-2048
#define SOUND3_CONTINUE	(0<<14) // �p��
#define SOUND3_TIMED	(1<<14) // ���Ԑ���
#define SOUND3_RESET	(1<<15) // �T�E���h1���Z�b�g


// REG_SGWR0
// REG_SGWR0_L
// REG_SGWR0_H
// �cREG_SGWR3


// �T�E���h4

// REG_SG40
#define SOUND4_LENGTH(x)	((x)<<0) // �T�E���h�̒��� 0-63
#define SOUND4_ENVELOP_STEP(x)	((x)<<8) // �G���x���[�v�X�e�b�v���� 0-7
#define SOUND4_ENVELOP_DEC		((0)<<11) // �G���x���[�v����
#define SOUND4_ENVELOP_INC		((1)<<11) // �G���x���[�v����
#define SOUND4_ENVELOP_VALUE(x)	((x)<<12) // �G���x���[�v�����l 0-15


// REG_SG41

#define SOUND4_DIVIDE_FREQ(x)	((x)<<0) // ���� 0-7
#define SOUND4_STAGE_15	(0<<3)	// �J�E���^�X�e�[�W
#define SOUND4_STAGE_7	(1<<3)	// 

#define SOUND4_STAGE_FREQ(x) ((x)<<4) // �X�e�b�v���Ƃ̎��� 0-13
#define SOUND4_CONTINUE	(0<<14) // �p��
#define SOUND4_TIMED	(1<<14) // ���Ԑ���
#define SOUND4_RESET	(1<<15) // �T�E���h1���Z�b�g



// REG_SGCNT_H

// GB4�`�����l���̏o�̓��V�I�@�Q�T���A�T�O���A�P�O�O��
#define SOUND_GB_RATIO_25	0
#define SOUND_GB_RATIO_50	1
#define SOUND_GB_RATIO_100	2

// DirectSound A�EB �o�̓��V�I�@�T�O���A�P�O�O��
#define SOUND_DSOUNDA_RATIO_50	0
#define SOUND_DSOUNDA_RATIO_100	(1<<2)
#define SOUND_DSOUNDB_RATIO_50	0
#define SOUND_DSOUNDB_RATIO_100	(1<<3)

// DirectSoundA�����E�̃X�s�[�J�[�ɏo�͂��邩
#define SOUND_DSOUNDA_OUTPUT_RIGHT	(1<<8)
#define SOUND_DSOUNDA_OUTPUT_LEFT	(1<<9)

// DirectSoundA�Ń^�C�}�[0���^�C�}�[�P�̂ǂ�����g�p���邩
#define SOUND_DSOUNDA_SAMP_TIMER0	0
#define SOUND_DSOUNDA_SAMP_TIMER1	(1<<10)

// DirectSoundA��FIFO�����Z�b�g����
#define SOUND_DSOUNDA_FIFO_RESET	(1<<11)


// DirectSoundB�����E�̃X�s�[�J�[�ɏo�͂��邩
#define SOUND_DSOUNDB_OUTPUT_RIGHT	(1<<12)
#define SOUND_DSOUNDB_OUTPUT_LEFT	(1<<13)

// DirectSoundB�Ń^�C�}�[0���^�C�}�[�P�̂ǂ�����g�p���邩
#define SOUND_DSOUNDB_SAMP_TIMER0	0
#define SOUND_DSOUNDB_SAMP_TIMER1	(1<<14)

// DirectSoundB��FIFO�����Z�b�g����
#define SOUND_DSOUNDB_FIFO_RESET	(1<<15)



#endif

