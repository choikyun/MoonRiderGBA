/**
 * @file game.c
 * @brief ゲーム本体
 * @date  2019.04.20 作成
 * @author Choi Gyun
 */

/***************************************************
 * Moon Rider
 * ver 1.0.0
 * 2019.04.19
 * Choi Gyun
 *****************************************************/

#define GLOBAL_VALUE_DEFINE

#include <gba.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "bg.h"
#include "game.h"
#include "graph.h"
#include "music.h"
#include "score.h"
#include "sprite.h"

void init_game();

static void disp_title();
static void init_ship();
static void init_fire();
static void init_blocks();
static void init_star(int, int, int);
static void init_guide();
static void init_boundary();
static void move_ship();
static void disp_ship();
static void disp_blocks();
static void disp_fire();
static void disp_guide();
static void draw_bg();
static void init_sprite_setting();
static void init_stage();
static void move_blocks();
static void check_stage_boundary();
static void create_new_blocks();
static void copy_affine(int, int);
static void copy_sp_attr(int, int);
static void init_lines();
static void create_new_line();
static void move_lines();
static void draw_lines();
static void draw_line(int);
static void draw_energy();
static VectorType trans_device_coord(SpriteCharType*);
static void disp_boundary();
static void flash();
static void shock();
static bool hits_block();
static void update_energy(int);
static void init_ring_icon();
static void set_ring_icon();
static void disp_ring_icon();
static void update_ring();
static void update_lv();
static void disp_num(int, int, u16);
static void update_score(int, int, int);
static void level_up();
static void reset_message();
static void disp_booster();
static void check_booster();
static void set_level_param(int);
static void disp_booster_icon();
static void init_booster_icon_anime();
static void disp_warning();
static void init_bomb(VectorType*, int);
static void set_new_bomb();
static void disp_bomb();
static void disp_gameover();
static void check_gameover();
static void pause();
static void disp_pause();
static void add_score(int);
static void add_bonus(int);
static void disp_bravo_icon();
static void set_bravo_icon();
static void init_bravo_icon();

//debug
void vbaPrint(char* s);

/**********************************************/ /**
 * @brief メインループ
 ***********************************************/
void game()
{
    game_state.key = keysDown();
    game_state.keyr = keysDownRepeat();

    seed++;
    stage.frame++;

    switch (game_state.scene) {
    case GAME_TITLE:
        disp_title();
        //select_mode();
        break;

    case GAME_DEMO:
        break;

    case GAME_READY:
        disp_ship();
        disp_fire();
        create_new_line();
        move_lines();
        draw_bg();
        draw_lines();
        disp_warning();
        break;

    case GAME_MAIN:
        move_ship();
        move_blocks();

        disp_ship();
        disp_fire();
        disp_bomb();
        disp_booster();
        disp_blocks();
        disp_guide();
        disp_ring_icon();
        disp_booster_icon();
        disp_bravo_icon();
        /*disp_boundary();*/

        create_new_blocks();
        create_new_line();
        move_lines();

        draw_bg();
        draw_lines();
        draw_energy();
        update_ring();
        update_lv();
        update_score(score, SCORE_X, SCORE_Y);
        level_up();
        check_booster();

        add_score(NORMAL_SCORE);
        check_gameover();

        pause();
        break;

    case GAME_PAUSE:
        pause();
        disp_ship();
        draw_bg();
        draw_energy();
        draw_lines();
        disp_pause();
        break;

    case GAME_OVER:
        disp_bomb();

        move_blocks();
        disp_blocks();

        //create_new_blocks();
        create_new_line();
        move_lines();

        draw_bg();
        draw_energy();
        draw_lines();
        update_ring();
        update_lv();
        disp_gameover();
        update_score(score, SCORE_X, SCORE_Y);
        break;
    }
}

/**********************************************/ /**
 * @brief ゲーム初期化
 ***********************************************/
void init_game()
{
    // ゲームステート初期化
    game_state.scene = GAME_TITLE;
    stage.mode = 0;
    stage.frame = 0;

    score = 0;

    // FLASH初期化
    for (int i = 0; i < 32; i++) {
        stage.flash.color[i] = FLASH_COLOR;
    }

    // ハイスコアのロード
    //hiscore = load_hiscore();

    // スプライト初期化
    init_sprite_setting();

    // 自機初期化
    init_ship();

    // 炎初期化
    init_fire();

    // ブロック初期化
    init_blocks();

    // 地平線
    init_lines();

    // ガイド
    init_guide();

    // 境界線
    init_boundary();

    // リングアイコン
    init_ring_icon();

    // ブラボーアイコン
    init_bravo_icon();

    // 逆噴射アイコンアニメ
    init_booster_icon_anime();

    // ステージ初期化
    init_stage();
}

/**********************************************/ /**
 * @brief ブロック作成
 ***********************************************/
static void
create_new_blocks()
{
    // 一定間隔で出現
    if (MAX_Z - (blocks.list[blocks.num - 1].vec.z >> FIX) >= blocks.interval) {

        for (int i = 0; i < APPER_MAX_BLOCKS; i++) {
            if (blocks.num < blocks.max_blocks) {
                init_star(
                    blocks.num,
                    RND(0, BLOCK_X_STEP_NUM) * BLOCK_X_STEP + BLOCK_W / 2 - SCREEN_CENTER - BLOCK_X_STEP_BLANK,
                    BLOCK_Y_TARGET);
                blocks.num++;
            }
        }
    }
}

/**********************************************/ /**
 * @brief ブロック移動
 ***********************************************/
static void
move_blocks()
{
    RectangleType m, e;

    if (!blocks.num) {
        return;
    }

    // ブロックの加速
    blocks.acc += DEF_BLOCK_ACC;
    if (blocks.acc < blocks.speed) {
        blocks.acc = blocks.speed;
    }

    for (int i = 0; i < blocks.num; i++) {
        blocks.list[i].vec.z += blocks.acc;
    }

    // 先頭のスプライトデータの消去
    if (blocks.list[0].vec.z >> FIX < MIN_Z) {
        // 当たり判定
        m.x = (ship.sprite.vec.x >> FIX) + SHIP_MOVE_MAX_X - ship.sprite.center.x;
        m.w = ship.sprite.hit.w;
        e.x = blocks.list[0].target.x + SHIP_MOVE_MAX_X - blocks.list[0].center.x + (stage.center.x >> FIX);
        e.w = blocks.list[0].hit.w;

        if (hits_block(&m, &e) && game_state.scene == GAME_MAIN) {
            
            // ブロック or リング
            if (blocks.list[0].type == NORMAL) {
                flash();
                shock();
                update_energy(DAMAGE_ENERGY);
            } else {
                stage.ring++;
                set_ring_icon();
                ship.sprite.show = true;
                update_energy(RECOVERY_ENERGY);
            }

        } else if(game_state.scene == GAME_MAIN){
        
            // ボーナススコア
            // ブロックをギリギリでかわせばボーナス
            if (blocks.list[0].type == NORMAL
                && abs((m.x + m.w / 2) - (e.x + e.w / 2)) <= BELOW_BLOCK_BONUS) {
                add_bonus(BLOCK_BONUS);
                set_bravo_icon();
            }
        
        }

        // エネルギー消費
        update_energy(-abs(stage.center.x >> FIX));

        blocks.num--;
        CpuSet(
            &blocks.list[1],
            &blocks.list[0],
            (COPY32 | (sizeof(SpriteCharType) / 4) * blocks.num));

        // スプライト消去
        erase_sprite(SPRITE_BLOCK + blocks.num);
    }
}

/**********************************************/ /**
 * @brief スプライト属性のコピー
 *
 * @param sorc 元
 * @param dest 先
 ***********************************************/
static void
copy_sp_attr(int sorc, int dest)
{
    OBJATTR* s = OAM + sorc;
    OBJATTR* d = OAM + dest;

    d->attr0 = s->attr0;
    d->attr1 = s->attr1;
    d->attr2 = s->attr2;
}

/**********************************************/ /**
 * @brief アフィンパラメータのコピー
 *
 * @param sorc 元
 * @param dest 先
 ***********************************************/
static void
copy_affine(int sorc, int dest)
{
    OBJAFFINE* s = (OBJAFFINE*)OAM + sorc;
    OBJAFFINE* d = (OBJAFFINE*)OAM + dest;

    d->pa = s->pa;
    d->pb = s->pb;
    d->pc = s->pc;
    d->pd = s->pd;
}

/**********************************************/ /**
 * @brief スプライト初期化
 ***********************************************/
static void
init_sprite_setting()
{
    //// 自機 32*32 dot
    set_sprite_form(SPRITE_SHIP, OBJ_SIZE(2), OBJ_SQUARE, OBJ_256_COLOR);
    set_sprite_tile(SPRITE_SHIP, TILE_SHIP1);

    //// 炎 16*16 dot
    set_sprite_form(SPRITE_FIRE, OBJ_SIZE(1), OBJ_SQUARE, OBJ_256_COLOR);
    set_sprite_tile(SPRITE_FIRE, TILE_FIRE1);

    /// 逆噴射 16*16 dot
    set_sprite_form(SPRITE_BOOSTER, OBJ_SIZE(1), OBJ_SQUARE, OBJ_256_COLOR);
    set_sprite_tile(SPRITE_BOOSTER, TILE_BOOSTER1);

    /// 逆噴射アイコン 8*8 dot
    set_sprite_form(SPRITE_BOOSTERICON, OBJ_SIZE(0), OBJ_SQUARE, OBJ_256_COLOR);
    set_sprite_tile(SPRITE_BOOSTERICON, TILE_BOOSTERICON1);

    //// ガイド 8*8 dot
    set_sprite_form(SPRITE_GUIDE, OBJ_SIZE(0), OBJ_SQUARE, OBJ_256_COLOR);
    set_sprite_tile(SPRITE_GUIDE, TILE_GUIDE1);

    //// 境界線 8*32 dot
    set_sprite_form(SPRITE_BOUNDARY_L, OBJ_SIZE(1), OBJ_TALL, OBJ_256_COLOR);
    set_sprite_tile(SPRITE_BOUNDARY_L, TILE_BOUNDARY1);

    set_sprite_form(SPRITE_BOUNDARY_R, OBJ_SIZE(1), OBJ_TALL, OBJ_256_COLOR);
    set_sprite_tile(SPRITE_BOUNDARY_R, TILE_BOUNDARY1);

    //// リングアイコン 16*16 dot
    set_sprite_form(SPRITE_RINGICON, OBJ_SIZE(1), OBJ_SQUARE, OBJ_256_COLOR);
    set_sprite_tile(SPRITE_RINGICON, TILE_RINGICON1);

    /// 爆風 16*16 dot
    set_sprite_form(SPRITE_BOMB, OBJ_SIZE(1), OBJ_SQUARE, OBJ_256_COLOR);

    /// ブラボー 8*32 dot
    set_sprite_form(SPRITE_BRAVOICON, OBJ_SIZE(2), OBJ_WIDE, OBJ_256_COLOR);
    set_sprite_tile(SPRITE_BRAVOICON, TILE_BRAVO1);

    //// ブロック 64*64 dot
    for (int i = 0; i < MAX_BLOCKS; i++) {
        set_sprite_form(SPRITE_BLOCK + i, OBJ_SIZE(Sprite_64x64), OBJ_SQUARE, OBJ_256_COLOR);
        set_sprite_tile(SPRITE_BLOCK + i, TILE_BLOCK1);
    }
}

/**********************************************/ /**
 * @brief リスタート
 ***********************************************/
static void
restart()
{
    game_state.scene = GAME_READY;

    // ランダマイズ
    srand(REG_TM0CNT + seed);
    // seedをSRAMに保存
    SRAMWrite32(SRAM_SEED, seed);

    // メッセージ初期化
    reset_message(&mes);
    reset_message(&lv_mes);

    // ステージ初期化
    init_stage();
}

/**********************************************/ /**
 * @brief ステージ初期化
 ***********************************************/
static void
init_stage()
{
    // ステージの座標
    stage.center.x = 0;
    stage.center.y = 0;

    // 獲得リング数
    stage.ring = 0;

    // レベル初期化
    set_level_param(0);
    stage.lv = 1;
    stage.next_lv = NEXT_LEVEL;
    blocks.max_blocks = MAX_BLOCKS;
}

/**********************************************/ /**
 * @brief 自機初期化
 ***********************************************/
static void
init_ship()
{
    // 加速度
    ship.sprite.acc.x = ship.sprite.acc.y = 0;

    // 自機スプライト
    ship.sprite.show = true;
    ship.sprite.chr = SPRITE_SHIP;
    ship.sprite.vec.x = SHIP_X << FIX;
    ship.sprite.vec.y = SHIP_Y << FIX;
    ship.sprite.vec.z = SHIP_Z << FIX;
    ship.sprite.center.x = SHIP_W / 2;
    ship.sprite.center.y = SHIP_H / 2;
    ship.sprite.target.x = SHIP_X;
    ship.sprite.target.y = SHIP_Y;
    ship.sprite.rect.w = SHIP_W;
    ship.sprite.rect.h = SHIP_H;
    ship.sprite.hit.w = SHIP_W - 1;
    ship.sprite.hit.h = SHIP_H - 1;
    ship.sprite.fix.x = 0;
    ship.sprite.fix.y = 0;

    // 振動
    ship.shock.range = SHOCK_RANGE;
    ship.shock.direc = 1;
    ship.shock.interval = SHOCK_INTERVAL;
    ship.shock.duration = 0;

    // エネルギー
    ship.energy = (MAX_ENERGY + MAX_ENERGY_BLANK) << E_FIX;
    ship.booster = 0;
}

/**********************************************/ /**
 * @brief 炎初期化
 ***********************************************/
static void
init_fire()
{
    fire.sprite.chr = SPRITE_FIRE;
    fire.sprite.vec.x = FIRE_X << FIX;
    fire.sprite.vec.y = FIRE_Y << FIX;
    fire.sprite.vec.z = FIRE_Z << FIX;
    fire.sprite.center.x = FIRE_W / 2;
    fire.sprite.center.y = FIRE_H / 2;
    fire.sprite.target.x = FIRE_X;
    fire.sprite.target.y = FIRE_Y;
    fire.sprite.rect.w = FIRE_W;
    fire.sprite.rect.h = FIRE_H;
    fire.sprite.fix.x = 0;
    fire.sprite.fix.y = 0;

    // アニメーション
    fire.anime.frame = 0;
    fire.anime.max_frame = 2;
    fire.anime.interval = fire.anime.interval_rel = FIRE_INTERVAL;
    fire.anime.is_start = true;

    // 逆噴射
    booster = fire;
    booster.sprite.chr = SPRITE_BOOSTER;
}

/**********************************************/ /**
 * @brief 逆噴射アイコンアニメ初期化
 ***********************************************/
static void
init_booster_icon_anime()
{
    booster_icon_anime.is_start = true;
    booster_icon_anime.frame = 0;
    booster_icon_anime.max_frame = 2;
    booster_icon_anime.interval = booster_icon_anime.interval_rel = BOOSTER_ICON_INTERVAL;
}

/**********************************************/ /**
 * @brief すべてのブロック初期化
 ***********************************************/
static void
init_blocks()
{
    for (int i = 0; i < MAX_BLOCKS; i++) {
        // スプライトの割当
        blocks.list[i].chr = SPRITE_BLOCK;
        blocks.list[i].center.x = BLOCK_SP_W / 2;
        blocks.list[i].center.y = BLOCK_SP_H / 2;
        blocks.list[i].rect.w = BLOCK_W;
        blocks.list[i].rect.h = BLOCK_H;
        blocks.list[i].hit.w = BLOCK_W - 1;
        blocks.list[i].hit.h = BLOCK_H - 1;
        blocks.list[i].fix.x = 0;
        blocks.list[i].fix.y = 0;
    }

    //blocks.interval = 3 * 60;
    blocks.num = 0;
}

/**********************************************/ /**
 * @brief ブロック初期化
 ***********************************************/
static void
init_star(int num, int x, int y)
{
    // 加速度
    //blocks.list[num].acc.z = BLOCK_SPEED;
    blocks.acc = blocks.speed;

    // スプライト
    blocks.list[num].target.x = x; // X座標目標値
    blocks.list[num].target.y = y; // Y座標目標値
    blocks.list[num].vec.z = MAX_Z << FIX;

    set_affine_setting(SPRITE_BLOCK + num, num, 0);

    // ブロック or リング
    blocks.list[num].type = NORMAL;
    if (!RND(0, blocks.ring_chance)) {
        blocks.list[num].type = RING;
    }
}

/**********************************************/ /**
 * @brief 地平線初期化
 ***********************************************/
static void
init_lines()
{
    lines.num = 0;
    lines.z = LINE_Z << FIX;
    for (int i = 0; i < MAX_LINES; i++) {
        lines.list[i].vec.x = 0;
        lines.list[i].vec.y = 0;
        lines.list[i].vec.z = MAX_Z << FIX;
        lines.list[i].target.x = 0;
        lines.list[i].target.y = LINE_Y_TARGET;
        lines.list[i].center.x = 0;
        lines.list[i].center.y = 0;
        lines.list[i].fix.y = 0;
        lines.list[i].fix.y = 0;
        lines.list[i].rect.w = 0;
        lines.list[i].rect.h = 0;
    }
}

/**********************************************/ /**
 * @brief リングアイコン初期化
 ***********************************************/
static void
init_ring_icon()
{
    ring_icon.life = 0;

    ring_icon.sprite.chr = SPRITE_RINGICON;
    ring_icon.sprite.vec.z = MIN_Z << FIX;
    ring_icon.sprite.center.x = ICON_W / 2;
    ring_icon.sprite.center.y = ICON_H / 2;
    ring_icon.sprite.fix.y = 0;
    ring_icon.sprite.fix.y = 0;
    ring_icon.sprite.rect.w = ICON_W;
    ring_icon.sprite.rect.h = ICON_H;
    ring_icon.sprite.acc.y = ICON_Y_ACC;
}

/**********************************************/ /**
 * @brief ブラボーアイコン初期化
 ***********************************************/
static void
init_bravo_icon()
{
    bravo_icon.life = 0;

    bravo_icon.sprite.chr = SPRITE_BRAVOICON;
    bravo_icon.sprite.vec.z = MIN_Z << FIX;
    bravo_icon.sprite.center.x = BRAVO_W / 2;
    bravo_icon.sprite.center.y = BRAVO_H / 2;
    bravo_icon.sprite.fix.y = 0;
    bravo_icon.sprite.fix.y = 0;
    bravo_icon.sprite.rect.w = BRAVO_W;
    bravo_icon.sprite.rect.h = BRAVO_H;
}

/**********************************************/ /**
 * @brief ガイド初期化
 ***********************************************/
static void
init_guide()
{
    guide.chr = SPRITE_GUIDE;
    guide.vec.x = 0;
    guide.vec.y = GUIDE_Y << FIX;
    guide.vec.z = MIN_Z << FIX;
    guide.target.x = 0;
    guide.target.y = GUIDE_Y;
    guide.center.x = GUIDE_W / 2;
    guide.center.y = GUIDE_H / 2;
    guide.fix.y = 0;
    guide.fix.y = 0;
    guide.rect.w = GUIDE_W;
    guide.rect.h = GUIDE_H;
}

/**********************************************/ /**
 * @brief 境界線初期化
 ***********************************************/
static void
init_boundary()
{
    boundary_l.sprite.chr = SPRITE_BOUNDARY_L;
    boundary_l.sprite.vec.x = 0;
    boundary_l.sprite.vec.y = 0;
    boundary_l.sprite.vec.z = MIN_Z << FIX;
    boundary_l.sprite.target.x = 0;
    boundary_l.sprite.target.y = 0;
    boundary_l.sprite.center.x = BOUNDARY_W / 2;
    boundary_l.sprite.center.y = BOUNDARY_H / 2;
    boundary_l.sprite.fix.y = 0;
    boundary_l.sprite.fix.y = 0;
    boundary_l.sprite.rect.w = BOUNDARY_W;
    boundary_l.sprite.rect.h = BOUNDARY_H;

    boundary_r.sprite.chr = SPRITE_BOUNDARY_R;
    boundary_r.sprite.vec.x = 0;
    boundary_r.sprite.vec.y = 0;
    boundary_r.sprite.vec.z = MIN_Z << FIX;
    boundary_r.sprite.target.x = 0;
    boundary_r.sprite.target.y = 0;
    boundary_r.sprite.center.x = BOUNDARY_W / 2;
    boundary_r.sprite.center.y = BOUNDARY_H / 2;
    boundary_r.sprite.fix.y = 0;
    boundary_r.sprite.fix.y = 0;
    boundary_r.sprite.rect.w = BOUNDARY_W;
    boundary_r.sprite.rect.h = BOUNDARY_H;
}

/**********************************************/ /**
 * @brief 当たり判定
 * 
 * @param m 座標
 * @param e 座標
 * @return 当たっているか
 ***********************************************/
static bool hits_block(RectangleType* m, RectangleType* e)
{
    // MX - EX + A = A + B + 1
    if ((u16)(m->x - e->x + m->w) < m->w + e->w + 1) {
        return true;
    }

    return false;
}

/**********************************************/ /**
 * @brief 爆風初期化
 * 
 * @param v 爆風発生源
 * @param max 発生回数
 ***********************************************/
static void
init_bomb(VectorType* v, int max)
{
    bomb.base.x = (v->x >> FIX) + FIX_STAGE_X - BOMB_W / 2;
    bomb.base.y = (v->y >> FIX) + FIX_STAGE_Y - BOMB_H / 2;

    bomb.num = 0;
    bomb.max = max;
    set_new_bomb();
}

/**********************************************/ /**
 * @brief 新しい爆風をセット
 ***********************************************/
static void
set_new_bomb()
{
    if (bomb.num < bomb.max) {
        // アニメ初期化
        bomb.anime.is_start = true;
        bomb.anime.frame = 0;
        bomb.anime.max_frame = 4;
        bomb.anime.interval = bomb.anime.interval_rel = BOMB_INTERVAL;

        // スプライト初期化
        set_sprite_tile(SPRITE_BOMB, TILE_BOMB1);
        bomb.sprite.vec.x = bomb.base.x + RND(0, BOMB_RANGE) - BOMB_RANGE / 2;
        bomb.sprite.vec.y = bomb.base.y + RND(0, BOMB_RANGE) - BOMB_RANGE / 2;

        bomb.num++;
    } else {
        bomb.num = bomb.max = 0;
    }
}

/**********************************************/ /**
 * @brief 爆風を表示
 ***********************************************/
static void
disp_bomb()
{
    if (!bomb.max) {
        return;
    }

    // アニメ
    if (!--bomb.anime.interval) {
        bomb.anime.interval = bomb.anime.interval_rel;
        bomb.anime.frame = (bomb.anime.frame + 1) % bomb.anime.max_frame;

        if (!bomb.anime.frame) {
            // 次の爆風をセット
            set_new_bomb();
            erase_sprite(SPRITE_BOMB);
            return;
        }

        // タイル変更
        set_sprite_tile(SPRITE_BOMB, TILE_BOMB1 + bomb.anime.frame * TILE_SIZE_16);
    }

    move_sprite(SPRITE_BOMB, bomb.sprite.vec.x, bomb.sprite.vec.y);
}

/**********************************************/ /**
 * @brief 新規ライン作成
 ***********************************************/
static void
create_new_line()
{
    lines.z += LINE_SPEED;
    if ((lines.z >> FIX) % LINE_INTERVAL != 0) {
        return;
    }
    lines.z = LINE_Z << FIX;

    if (lines.num < MAX_LINES) {
        lines.list[lines.num].vec.y = 0;
        lines.list[lines.num].vec.z = MAX_Z << FIX;
        lines.num++;
    }
}

/**********************************************/ /**
 * @brief ライン移動
 ***********************************************/
static void
move_lines()
{
    if (!lines.num) {
        return;
    }

    for (int i = 0; i < lines.num; i++) {
        lines.list[i].vec.z += LINE_SPEED /*blocks.acc*/;
    }

    // 削除
    if (lines.list[0].vec.z >> FIX < MIN_Z) {
        lines.num--;
        CpuSet(
            &lines.list[1],
            &lines.list[0],
            (COPY32 | (sizeof(SpriteCharType) / 4) * lines.num));
    }
}

/**********************************************/ /**
 * @brief ライン描画
 ***********************************************/
static void
draw_lines()
{
    VectorType v;

    for (int i = 0; i < lines.num; i++) {
        v = trans_device_coord(&lines.list[i]);
        draw_line(v.y);
    }
}

/**********************************************/ /**
 * @brief ライン描画 サブ
 ***********************************************/
static void
draw_line(int y)
{
    u32* dst = (u32*)current_frame + (y * SCREEN_WIDTH) / 4;

    for (int i = 0; i < LINE_W / 4; i++) {
        *(dst + i) = LINE_COLOR;
    }
}

/**********************************************/ /**
 * @brief 3D座標からデバイス座標に変換
 * 
 * @param *sp スプライト
 * @return 3D座標
 ***********************************************/
static VectorType
trans_device_coord(SpriteCharType* sp)
{
    VectorType v;
    int z = sp->vec.z >> FIX; // Z軸整数値のみ
    int tx = sp->target.x + sp->fix.x; // 目標値の補正
    int ty = sp->target.y + sp->fix.y;

    // sc1 - sc0 でスケールを求める
    float sc0 = ((float)tx / z) * MIN_Z + 0.5f;
    float sc1 = (((float)tx + sp->rect.w) / z) * MIN_Z + 0.5f;

    v.x = sc0 + FIX_STAGE_X - sp->center.x;
    v.y = ((float)ty / z) * MIN_Z + FIX_STAGE_Y - sp->center.y + 0.5f;

    v.scale = ((sc1 - sc0) / sp->rect.w) * 100 + 0.5f;
    if (v.scale < 1) {
        v.scale = 1;
    }

    return v;
}

/**********************************************/ /**
 * @brief 自機移動
 ***********************************************/
static void
move_ship()
{
    u16 key = game_state.keyr;

    // デフォルトタイル
    set_sprite_tile(SPRITE_SHIP, TILE_SHIP1);

    // 移動
    if (key & KEY_LEFT) {
        ship.sprite.acc.x -= SHIP_SPEED;
        if (ship.sprite.acc.x < -MAX_SHIP_ACC) {
            ship.sprite.acc.x = -MAX_SHIP_ACC;
        }
        set_sprite_tile(SPRITE_SHIP, TILE_SHIP3);
        fire.sprite.vec.x = FIRE_X_LEFT << FIX;
    } else if (key & KEY_RIGHT) {
        ship.sprite.acc.x += SHIP_SPEED;
        if (ship.sprite.acc.x > MAX_SHIP_ACC) {
            ship.sprite.acc.x = MAX_SHIP_ACC;
        }
        set_sprite_tile(SPRITE_SHIP, TILE_SHIP2);
        fire.sprite.vec.x = FIRE_X_RIGHT << FIX;
    }

    // 逆噴射
    if (ship.booster) {
        ship.booster--;
    }
    if (key & KEY_B && !ship.booster && ship.allows_booster) {
        blocks.acc = BOOSTER_ACC;
        ship.energy += BOOTER_ENERGY;
        ship.booster = BOOST_TIME;
    }

    // 自然減速
    if (!(key & (KEY_UP | KEY_DOWN | KEY_LEFT | KEY_RIGHT))) {
        ship.sprite.acc.x += -ship.sprite.acc.x * SHIP_FRIC;
    }

    // 自機とは逆方向にステージを移動
    stage.center.x -= ship.sprite.acc.x;

    // 境界判定
    check_stage_boundary();
}

/**********************************************/ /**
 * @brief ステージ境界チェック 両端は繋がっている
 ***********************************************/
static void
check_stage_boundary()
{
    int x = stage.center.x >> FIX;

    if (x < SHIP_MOVE_MIN_X) {
        stage.center.x = SHIP_MOVE_MAX_X << FIX;
    } else if (x > SHIP_MOVE_MAX_X) {
        stage.center.x = SHIP_MOVE_MIN_X << FIX;
    }
}

/**********************************************/ /**
 * @brief 自機表示
 ***********************************************/
static void
disp_ship()
{
    ship.sprite.target.x = ship.sprite.vec.x >> FIX;
    ship.sprite.target.y = ship.sprite.vec.y >> FIX;

    // 座標変換
    VectorType v = trans_device_coord(&ship.sprite);

    // エネルギー警告
    if (ship.energy >> E_FIX <= MAX_ENERGY / 4)
    {
        if (!--ship.flash.interval)
        {
            ship.flash.interval = SHIP_CAUTION_INTERVAL;
            ship.sprite.show ^= 1;
        }
    } else {
        ship.flash.interval = SHIP_CAUTION_INTERVAL;
    }
    
    // 振動
    int range = 0;
    if (ship.shock.duration) {
        ship.shock.duration--;

        if (!--ship.shock.interval) {
            ship.shock.direc *= -1;
            ship.shock.interval = SHOCK_INTERVAL;
        }
        range = ship.shock.direc * ship.shock.range;
    }
    
    if (ship.sprite.show) {
        move_sprite(ship.sprite.chr, v.x + range, v.y);
    } else {
        erase_sprite(ship.sprite.chr);
    }
}

/**********************************************/ /**
 * @brief 炎表示
 ***********************************************/
static void
disp_fire()
{
    if (ship.booster > 0) {
        erase_sprite(fire.sprite.chr);
    }

    // アニメ
    if (fire.anime.is_start && --fire.anime.interval < 0) {
        fire.anime.interval = fire.anime.interval_rel;
        fire.anime.frame = (fire.anime.frame + 1) % fire.anime.max_frame;
        // タイル切り替え
        set_sprite_tile(SPRITE_FIRE, TILE_FIRE1 + (fire.anime.frame * TILE_SIZE_16));
    }

    fire.sprite.target.x = ship.sprite.vec.x >> FIX;
    fire.sprite.target.y = FIRE_Y + (ship.sprite.vec.y >> FIX);

    // 座標変換
    VectorType v = trans_device_coord(&fire.sprite);

    move_sprite(fire.sprite.chr, v.x, v.y);
}

/**********************************************/ /**
 * @brief 逆噴射表示
 ***********************************************/
static void
disp_booster()
{
    if (!ship.booster) {
        erase_sprite(booster.sprite.chr);
        return;
    }

    erase_sprite(fire.sprite.chr);

    // アニメ
    if (booster.anime.is_start && --booster.anime.interval < 0) {
        booster.anime.interval = booster.anime.interval_rel;
        booster.anime.frame = (booster.anime.frame + 1) % booster.anime.max_frame;
        // タイル切り替え
        set_sprite_tile(SPRITE_BOOSTER, TILE_BOOSTER1 + (booster.anime.frame * TILE_SIZE_16));
    }

    // ターゲット座標 更新
    booster.sprite.target.x = ship.sprite.vec.x >> FIX;
    booster.sprite.target.y = (ship.sprite.vec.y >> FIX) + BOOSTER_FIXED_Y;

    // 座標変換
    VectorType v = trans_device_coord(&booster.sprite);

    move_sprite(booster.sprite.chr, v.x, v.y);
}

/**********************************************/ /**
 * @brief ブロック表示
 ***********************************************/
static void
disp_blocks()
{
    VectorType v;

    for (int i = 0; i < blocks.num; i++) {
        // ステージ側が移動するのでターゲット座標の補正
        //blocks.list[cur].fix.x = stage.center.x >> FIX; // 視点をを移動しない場合はこちら

        // 座標変換
        v = trans_device_coord(&blocks.list[i]);

        // アフィンパラメータ設定
        set_affine_setting(SPRITE_BLOCK + i, i, 0 /*OBJ_DOUBLE*/);

        // スケールを設定
        // ブロック or リング
        if (blocks.list[i].type == NORMAL) {
            set_scale(i, v.scale, v.scale /* * 1.2f*/);
            set_sprite_tile(SPRITE_BLOCK + i, TILE_BLOCK1);
        } else {
            set_scale(i, v.scale, v.scale);
            set_sprite_tile(SPRITE_BLOCK + i, TILE_RING1);
        }

        move_sprite(
            blocks.list[i].chr + i,
            // v.x,
            v.x + (stage.center.x >> FIX), // 視点を移動する場合はこちら
            v.y);
    }
}

/**********************************************/ /**
 * @brief ガイド表示 常に中央を指す
 ***********************************************/
static void
disp_guide()
{
    guide.target.x = 0;
    guide.target.y = GUIDE_Y;

    VectorType v = trans_device_coord(&guide);

    move_sprite(
        guide.chr,
        v.x + (stage.center.x >> FIX),
        v.y);
}

/**********************************************/ /**
 * @brief 逆噴射アイコン 逆噴射可能時にオン
 ***********************************************/
static void
disp_booster_icon()
{
    if (ship.allows_booster) {

        // アニメ
        if (booster_icon_anime.is_start && --booster_icon_anime.interval < 0) {
            booster_icon_anime.interval = booster_icon_anime.interval_rel;
            booster_icon_anime.frame = (booster_icon_anime.frame + 1) % booster_icon_anime.max_frame;
            // タイル切り替え
            set_sprite_tile(SPRITE_BOOSTERICON, TILE_BOOSTERICON1 + (booster_icon_anime.frame * TILE_SIZE_8));
        }

    } else {
        set_sprite_tile(SPRITE_BOOSTERICON, TILE_BOOSTERICON1);
    }
    move_sprite(SPRITE_BOOSTERICON, 232, 0);
}

/**********************************************/ /**
 * @brief 境界表示 未使用
 ***********************************************/
static void
disp_boundary()
{
    if (stage.center.x >> FIX < 0) {
        // 左の境界
        boundary_l.sprite.target.x = SHIP_MOVE_MIN_X;
        boundary_l.sprite.target.y = BOUNDARY_Y;

        VectorType v = trans_device_coord(&boundary_l.sprite);

        move_sprite(
            boundary_l.sprite.chr,
            v.x + (stage.center.x >> FIX),
            v.y);

        erase_sprite(boundary_r.sprite.chr);
    } else {
        // 右の境界
        boundary_r.sprite.target.x = SHIP_MOVE_MAX_X;
        boundary_r.sprite.target.y = BOUNDARY_Y;

        VectorType v = trans_device_coord(&boundary_r.sprite);

        move_sprite(
            boundary_r.sprite.chr,
            v.x + (stage.center.x >> FIX),
            v.y);

        erase_sprite(boundary_l.sprite.chr);
    }
}

/**********************************************/ /**
 * @brief リング獲得のアイコン
 ***********************************************/
static void
disp_ring_icon()
{
    if (!ring_icon.life) {
        return;
    }

    if (ring_icon.target.y != ring_icon.sprite.vec.y >> FIX) {
        ring_icon.sprite.vec.y += ring_icon.sprite.acc.y;
    }

    ring_icon.sprite.target.x = ring_icon.sprite.vec.x >> FIX;
    ring_icon.sprite.target.y = ring_icon.sprite.vec.y >> FIX;

    VectorType v = trans_device_coord(&ring_icon.sprite);

    move_sprite(
        ring_icon.sprite.chr,
        v.x,
        v.y);

    if (--ring_icon.life == 0) {
        erase_sprite(ring_icon.sprite.chr);
    }
}

/**********************************************/ /**
 * @brief ブロックをすれすれで通過すごいねアイコン
 ***********************************************/
static void
disp_bravo_icon()
{
    if (!bravo_icon.life) {
        return;
    }

    bravo_icon.sprite.target.x = bravo_icon.sprite.vec.x >> FIX;
    bravo_icon.sprite.target.y = bravo_icon.sprite.vec.y >> FIX;

    VectorType v = trans_device_coord(&bravo_icon.sprite);

    move_sprite(bravo_icon.sprite.chr, v.x, v.y);

    if (--bravo_icon.life == 0) {
        erase_sprite(bravo_icon.sprite.chr);
    }
}


/**********************************************/ /**
 * @brief フラッシュ
 ***********************************************/
static void
flash()
{
    stage.flash.interval = FLASH_INTERVAL;
}

/**********************************************/ /**
 * @brief 振動
 ***********************************************/
static void
shock()
{
    if (!ship.shock.duration) {
        ship.shock.duration = SHOCK_DURATION;
        ship.shock.interval = SHOCK_INTERVAL;
    }
}

/**********************************************/ /**
 * @brief リングアイコンセット
 ***********************************************/
static void
set_ring_icon()
{
    ring_icon.life = ICON_LIFE;
    ring_icon.target.y = (ship.sprite.vec.y >> FIX) + ICON_TARGET_Y;
    ring_icon.sprite.vec.x = ship.sprite.vec.x;
    ring_icon.sprite.vec.y = ship.sprite.vec.y + (ICON_RING_Y << FIX);
}

/**********************************************/ /**
 * @brief ブラボーアイコンセット
 ***********************************************/
static void
set_bravo_icon()
{
    bravo_icon.life = BRAVO_LIFE;
    bravo_icon.target.y = (ship.sprite.vec.y >> FIX) + BRAVO_TARGET_Y;
    bravo_icon.sprite.vec.x = ship.sprite.vec.x;
    bravo_icon.sprite.vec.y = ship.sprite.vec.y + (BRAVO_TARGET_Y << FIX);
}

/**********************************************/ /**
 * @brief エネルギー更新
 ***********************************************/
static void
update_energy(int e)
{
    ship.energy += e;

    if (ship.energy < 0) {
        ship.energy = 0;
    } else if (ship.energy > (MAX_ENERGY + MAX_ENERGY_BLANK) << E_FIX) {
        ship.energy = (MAX_ENERGY + MAX_ENERGY_BLANK) << E_FIX;
    }
}

/**********************************************/ /**
 * @brief エネルギー表示
 ***********************************************/
static void
draw_energy()
{
    int max = ship.energy >> E_FIX;
    if (max > MAX_ENERGY) {
        max = MAX_ENERGY;
    }

    for (int i = 0; i < max; i++) {
        draw_bitmap_frame(ENERGY_X + (i * 2), ENERGY_Y, ENERGY_W, ENERGY_H, bmp_energy1Bitmap);
    }

    int x = ENERGY_X + max * 2;
    max = MAX_ENERGY - max;
    for (int j = 0; j < max; j++) {
        draw_bitmap_frame(x + (j * 2), ENERGY_Y, ENERGY_W, ENERGY_H, bmp_energy2Bitmap);
    }
}

/**********************************************/ /**
 * @brief ステージのBG描画
 ***********************************************/
static void
draw_bg()
{
    flip_frame();

    if (stage.flash.interval) {
        stage.flash.interval--;
        fill_frame_bitmap(stage.flash.color);
    } else {
        // 通常のBG
        load_frame_bitmap(DEF_BG_BITMAP);
    }
}

/**********************************************/ /**
 * @brief 逆噴射できるかチェックする
 ***********************************************/
static void
check_booster()
{
    // 経過時間とエネルギー残量を確認
    if (stage.frame > USE_BOOSTER_INTERVAL && (ship.energy >> E_FIX) > ALLOW_BOOSTER_ENERGY && !ship.booster) {
        ship.allows_booster = true;
    } else {
        ship.allows_booster = false;
    }
}

/**********************************************/ /**
 * @brief ポーズ
 ***********************************************/
static void
pause()
{
    u16 key = game_state.key;

    if (key & KEY_SELECT) {
        game_state.scene ^= GAME_PAUSE;
        if (game_state.scene & GAME_PAUSE) {
            //StopMusic();
            //PlaySound(SOUND_ITEM);
        } else {
            //PlayMusic(MUSIC_STAGE + stage_bgm, true);
        }
    }
}

/**********************************************/ /**
 * @brief リング数表示
 ***********************************************/
static void
update_ring()
{
    int i;
    int pos = RING_DIGIT * NUM_W - NUM_W;
    int r = stage.ring;

    for (i = 0; i < RING_DIGIT; i++) {
        disp_num(RING_X + pos, RING_Y, r % 10);
        r /= 10;
        pos -= (NUM_W);
    }
}

/**********************************************/ /**
 * @brief レベル表示
 ***********************************************/
static void
update_lv()
{
    // レベルアップ
    if (--stage.next_lv < 0 && stage.lv < MAX_LV) {
        stage.next_lv = NEXT_LEVEL;

        set_level_param(stage.lv);

        stage.lv++;
        reset_message(&lv_mes);
        lv_mes.is_start = true;
    }

    // レベル表示
    int pos = LV_DIGIT * NUM_W - NUM_W;
    int r = stage.lv;

    for (int i = 0; i < LV_DIGIT; i++) {
        disp_num(LV_X + pos, LV_Y, r % 10);
        r /= 10;
        pos -= (NUM_W);
    }
}

/**********************************************/ /**
 * @brief レベル毎のパラメータをセット
 * 
 * @param lv レベル
 ***********************************************/
static void
set_level_param(int lv)
{
    // ブロック速度, ブロック生成間隔, リング生成確率,
    static int param[MAX_LV][3] = {
        { -4096 * 20, 40, 2 },
        { -4096 * 21, 38, 2 },
        { -4096 * 22, 36, 2 },
        { -4096 * 23, 34, 3 },
        { -4096 * 24, 32, 3 },
        { -4096 * 25, 30, 3 },
        { -4096 * 26, 28, 4 },
        { -4096 * 27, 26, 4 }
    };

    // エネルギー回復
    ship.energy = (MAX_ENERGY + MAX_ENERGY_BLANK) << E_FIX;

    // ブロックスピード
    blocks.speed = param[lv][0];

    // ブロックの生成間隔 Z座標
    blocks.interval = param[lv][1];
    blocks.interval_rel = param[lv][1];

    // リング生成確率
    blocks.ring_chance = param[lv][2];
}

/**********************************************/ /**
 * @brief レベルアップ表示
 ***********************************************/
static void
level_up()
{
    if (!lv_mes.is_start) {
        return;
    }

    // 点滅
    if (!--lv_mes.wait) {
        lv_mes.wait = lv_mes.wait_rel;
        lv_mes.chr ^= 1;

        if (!--lv_mes.count) {
            reset_message(&lv_mes);

            // BGM切り替え
            //StopMusic();
            //stage_bgm = (stage_bgm + 1) & MAX_STAGE_BGM;
            //PlayMusic(MUSIC_STAGE + stage_bgm, true);
        }
    }

    if (lv_mes.chr) {
        draw_bitmap_frame(LV_MES_X, LV_MES_X, LV_MES_W, LV_MES_H, bmp_lv_black1Bitmap);
    }
}

/**********************************************/ /**
 * @brief スコア加算
 ***********************************************/
static void
add_score(int s)
{
    score += s;
}

/**********************************************/ /**
 * @brief ボーナス加算
 ***********************************************/
static void
add_bonus(int b)
{
    bonus += b;
}

/**********************************************/ /**
 * @brief スコア表示
 ***********************************************/
static void
update_score(int score, int x, int y)
{
    int pos = SCORE_DIGIT * NUM_W - NUM_W;
    int sc = score;

    for (int i = 0; i < SCORE_DIGIT; i++) {
        disp_num(x + pos, y, sc % 10);
        sc /= 10;
        pos -= (NUM_W);
    }
}

/**********************************************/ /**
 * @brief ハイスコア表示
 ***********************************************/
void update_hiscore()
{
    /*
    int i;
    int pos = SCORE_DIGIT * NUM_W - NUM_W;
    int sc = hiscore;

    for (i = 0; i < SCORE_DIGIT; i++)
    {
        disp_num_title(HISCORE_X + pos, HISCORE_Y, sc % 10);
        sc /= 10;
        pos -= (NUM_W);
    }
    */
}

/**********************************************/ /**
 * @brief ワーニングメッセージ表示
 ***********************************************/
static void
disp_warning()
{
    if (stage.frame < WAIT_WARNING) {
        return;
    }

    if (!--mes.wait) {
        mes.wait = mes.wait_rel;
        mes.chr ^= 1;

        if (!--mes.count) {
            game_state.scene = GAME_MAIN;
            reset_message(&mes);
            stage.frame = 0;
            //stage_bgm = 0;
            //PlayMusic (MUSIC_STAGE, PLAY_LOOP_ON);
        }
    }

    if (mes.chr) {
        draw_bitmap_frame(MES_X, MES_Y, MES_W, MES_H, bmp_warningBitmap);
    }
}

/**********************************************/ /**
 * @brief ゲームオーバーメッセージ表示
 ***********************************************/
static void
disp_gameover()
{
    if (stage.frame == 1) {

        // 爆風
        init_bomb(&ship.sprite.vec, MAX_OVER_BOMBS);

    } else if (stage.frame > 2 * 60) {

        // ゲームオーバー
        draw_bitmap_frame(MES_X, MES_Y, MES_W, MES_H, bmp_overBitmap);

        if (stage.frame > 2.5 * 60) {
            // ボーナス
            draw_bitmap_frame(BONUS_MES_X, BONUS_MES_Y, BONUS_MES_W, BONUS_MES_H, bmp_bonusBitmap);
            // トータル
            draw_bitmap_frame(TOTAL_MES_X, TOTAL_MES_Y, TOTAL_MES_W, TOTAL_MES_H, bmp_totalBitmap);

            update_score(bonus, BONUS_X, BONUS_Y);
            update_score(total, TOTAL_X, TOTAL_Y);
        }

        // タイトル画面へ
        u16 key = game_state.key;
        if (key & KEY_START) {
            //save_hiscore(score);

            reset_frame();
            init_sprite();
            init_game();
            load_title();
            StopMusic();

            //PlayMusic(MUSIC_TITLE, PLAY_LOOP_ON);
        }
    }
}

/**********************************************/ /**
 * @brief ゲームオーバー判定表示
 ***********************************************/
static void
check_gameover()
{
    if (ship.energy <= 0) {
        game_state.scene = GAME_OVER;

        erase_sprite(ship.sprite.chr);
        erase_sprite(fire.sprite.chr);
        erase_sprite(ring_icon.sprite.chr);
        erase_sprite(bravo_icon.sprite.chr);

        // ブロック消去
        for (int i = 0; i < MAX_BLOCKS; i++) {
            erase_sprite(SPRITE_BLOCK + i);
        }

        // ボーナススコア
        bonus += stage.ring * RING_BONUS_UNIT;

        // トータルスコア
        total = score + bonus;

        // フレームリセット
        stage.frame = 0;
    }
}

/**********************************************/ /**
 * @brief ポーズメッセージ表示
 ***********************************************/
static void
disp_pause()
{
    draw_bitmap_frame(MES_X, MES_Y, MES_W, MES_H, bmp_pauseBitmap);
}

/**********************************************/ /**
 * @brief 点滅メッセージ用パラメータのリセット
 * @param *m メッセージパラメータポインタ
 ***********************************************/
static void
reset_message(BlinkMessageType* m)
{
    m->is_start = false;
    m->count = MES_COUNT;
    m->chr = 0;
    m->wait = m->wait_rel = MES_WAIT;
}

/**********************************************/ /**
 * @brief 点滅メッセージ用パラメータのリセット
 * @param *m メッセージパラメータポインタ
 ***********************************************/
static void
reset_message_fast(BlinkMessageType* m)
{
}

/**********************************************/ /**
 * @brief BGに数字表示
 * @param x X座標
 * @param y Y座標
 * @param num 表示する数値
 ***********************************************/
static void
disp_num(int x, int y, u16 num)
{
    draw_bitmap_frame(x, y, NUM_W, NUM_H, bmp_numBitmap + 32 * num);
}

/**********************************************/ /**
 *  タイトル ビットマップ転送
 ***********************************************/
void load_title()
{
    load_bg_bitmap_lz77(DEF_TITLE_BITMAP);
    //update_hiscore ();
}

/**********************************************/ /**
 * @brief タイトル
 ***********************************************/
static void
disp_title()
{
    u16 key = game_state.key;

    if (key & KEY_START) {
        restart();
        stage.demo = false;
        //  StopMusic ();
    } else if ((key & KEY_R) && (key & KEY_B)) {
        //  clear_hiscore ();
        //  update_hiscore ();
    }
}

//// デバグ用
//THUMB code
void vbaPrint(char* s)
{
    /*
    asm volatile("mov r0, %0;"
                 "swi 0xff;"
                 : // no ouput
                 : "r"(s)
                 : "r0");
    */
}
