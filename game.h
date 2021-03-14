// Made by Guilherme Carbonari Boneti, GRR20196478, 02/2021.

#ifndef __GAME__
#define __GAME__

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>

#define BUFFER_W 480
#define BUFFER_H 276

#define DISP_SCALE 3
#define DISP_W (BUFFER_W * DISP_SCALE)
#define DISP_H (BUFFER_H * DISP_SCALE)

#define OBJECT_W 12
#define OBJECT_H 12

#define N_OBJ_W (BUFFER_W/OBJECT_W)
#define N_OBJ_H ((BUFFER_H-OBJECT_H)/OBJECT_H)

#define MAPS_N 2

ALLEGRO_DISPLAY* disp;
ALLEGRO_BITMAP* buffer;

ALLEGRO_FONT* font;

ALLEGRO_SAMPLE* coin;
ALLEGRO_SAMPLE* dig;
ALLEGRO_SAMPLE* boulder;
ALLEGRO_SAMPLE* coin_fall;
ALLEGRO_SAMPLE* boom;
ALLEGRO_SAMPLE* lose;
ALLEGRO_SAMPLE* score;
ALLEGRO_SAMPLE* menu;
ALLEGRO_SAMPLE* menu_select;
ALLEGRO_SAMPLE* cheat;
ALLEGRO_SAMPLE* win;

void must_init(bool test, const char *description);

void disp_init();

void disp_deinit();

void disp_pre_draw();

void disp_post_draw();

void audio_init();

void audio_deinit();

typedef struct SPRITES
{
  ALLEGRO_BITMAP* _sheet;
  ALLEGRO_BITMAP* smoke_sheet;

  ALLEGRO_BITMAP* border;
  ALLEGRO_BITMAP* boulder;
  ALLEGRO_BITMAP* earth;
  ALLEGRO_BITMAP* wall;
  ALLEGRO_BITMAP* jewel;
  ALLEGRO_BITMAP* hero[6][3];
  ALLEGRO_BITMAP* door;
  ALLEGRO_BITMAP* smoke[8];
} SPRITES;
SPRITES sprites;

ALLEGRO_BITMAP* sprite_grab(int x, int y, int w, int h);

ALLEGRO_BITMAP* sprite_grab_smoke(int x, int y, int w, int h);

ALLEGRO_BITMAP* infinite;

void image_init();
void image_deinit();

void sprites_init();
void sprites_deinit();

typedef enum {
  BOULDER,
  WALL,
  EARTH,
  BORDER,
  JEWEL,
  HERO,
  DOOR,
  SMOKE
} object_t ;

typedef struct CELL
{
  object_t type;
  int show;
} CELL;

typedef struct COORDINATE
{
  int x,y;
  bool falling;
  int left, right;
} COORDINATE;

typedef struct OBJECT {
  int size;
  COORDINATE el[200];
} OBJECT;

typedef struct MAP
{
  CELL el[N_OBJ_W][N_OBJ_H];
} MAP;

typedef struct GAME
{
  long frames;
  long score[2];
  int coins, door_coins, pts_per_coin, pts_gain;
  bool done, die, finish, restart, pause, infinite_time;
  int smoke_index, time, current_score;
  int pause_set[2][3];
  int map;
  int cheat_index;
} GAME;

void objects_init(MAP *map, OBJECT *boulders, COORDINATE *hero, OBJECT *jewels, GAME *game);

void draw_map(MAP *map, COORDINATE *hero, GAME *game);

void hero_update(ALLEGRO_EVENT* event, MAP *map, COORDINATE *hero, OBJECT *boulders, OBJECT *jewels, GAME *game);

void boulders_update(MAP *map, OBJECT *boulders, GAME *game);

void jewels_update(MAP *map, OBJECT *jewels, GAME *game);

void restart(MAP *map, COORDINATE *hero, GAME *game);

void hud_init(GAME *game);

void hud_deinit();

void hud_update(GAME *game);

void hud_draw(GAME *game);

#endif