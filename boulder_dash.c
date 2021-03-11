#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>

// SIZE: 40X22

void must_init(bool test, const char *description)
{
  if(test) return;

  printf("couldn't initialize %s\n", description);
  exit(1);
}

#define BUFFER_W 480
#define BUFFER_H 264

#define DISP_SCALE 3
#define DISP_W (BUFFER_W * DISP_SCALE)
#define DISP_H (BUFFER_H * DISP_SCALE)

#define OBJECT_W 12
#define OBJECT_H 12

#define N_OBJ_W (BUFFER_W/OBJECT_W)
#define N_OBJ_H (BUFFER_H/OBJECT_H)

ALLEGRO_DISPLAY* disp;
ALLEGRO_BITMAP* buffer;

void disp_init()
{
  al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
  al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

  disp = al_create_display(DISP_W, DISP_H);
  must_init(disp, "display");

  buffer = al_create_bitmap(BUFFER_W, BUFFER_H);
  must_init(buffer, "bitmap buffer");
}

void disp_deinit()
{
  al_destroy_bitmap(buffer);
  al_destroy_display(disp);
}

void disp_pre_draw()
{
  al_set_target_bitmap(buffer);
}

void disp_post_draw()
{
  al_set_target_backbuffer(disp);
  al_draw_scaled_bitmap(buffer, 0, 0, BUFFER_W, BUFFER_H, 0, 0, DISP_W, DISP_H, 0);

  al_flip_display();
}

ALLEGRO_SAMPLE* coin;
ALLEGRO_SAMPLE* dig;
ALLEGRO_SAMPLE* boulder;
ALLEGRO_SAMPLE* coin_fall;

void audio_init()
{
  al_install_audio();
  al_init_acodec_addon();
  al_reserve_samples(128);

  coin = al_load_sample("coin.wav");
  dig = al_load_sample("dig.wav");
  boulder = al_load_sample("boulder.wav");
  coin_fall = al_load_sample("coin-fall.wav");

  must_init(coin, "coin sample");
  must_init(dig, "dig sample");
  must_init(boulder, "boulder sample");
  must_init(coin_fall, "coin fall sample");
}

void audio_deinit()
{
  al_destroy_sample(coin);
  al_destroy_sample(dig);
  al_destroy_sample(boulder);
  al_destroy_sample(coin_fall);

}

typedef struct SPRITES
{
  ALLEGRO_BITMAP* _sheet;

  ALLEGRO_BITMAP* border;
  ALLEGRO_BITMAP* boulder;
  ALLEGRO_BITMAP* earth;
  ALLEGRO_BITMAP* wall;
  ALLEGRO_BITMAP* jewel;
  ALLEGRO_BITMAP* hero[6][3];
  ALLEGRO_BITMAP* door;
} SPRITES;
SPRITES sprites;

ALLEGRO_BITMAP* sprite_grab(int x, int y, int w, int h)
{
  ALLEGRO_BITMAP* sprite = al_create_sub_bitmap(sprites._sheet, x, y, w, h);
  must_init(sprite, "sprite grab");
  return sprite;
}

void sprites_init()
{
  sprites._sheet = al_load_bitmap("tileset.png");
  must_init(sprites._sheet, "spritesheet");

  sprites.border = sprite_grab(228, 36, OBJECT_W, OBJECT_H);
  sprites.earth = sprite_grab(36, 36, OBJECT_W, OBJECT_H);
  sprites.boulder = sprite_grab(0, 204, OBJECT_W, OBJECT_H);
  sprites.wall = sprite_grab(0, 48, OBJECT_W, OBJECT_H);
  sprites.jewel = sprite_grab(108, 48, OBJECT_W, OBJECT_H);
  sprites.door = sprite_grab(0, 84, OBJECT_W, OBJECT_H);

  sprites.hero[0][0] = sprite_grab(24, 144, OBJECT_W, OBJECT_H);
  sprites.hero[1][0] = sprite_grab(24, 144, OBJECT_W, OBJECT_H);
  sprites.hero[2][0] = sprite_grab(24, 144, OBJECT_W, OBJECT_H);
  sprites.hero[3][0] = sprite_grab(24, 144, OBJECT_W, OBJECT_H);
  sprites.hero[4][0] = sprite_grab(24, 144, OBJECT_W, OBJECT_H);
  sprites.hero[5][0] = sprite_grab(24, 144, OBJECT_W, OBJECT_H);

  sprites.hero[0][1] = sprite_grab(0, 156, OBJECT_W, OBJECT_H);
  sprites.hero[1][1] = sprite_grab(12, 156, OBJECT_W, OBJECT_H);
  sprites.hero[2][1] = sprite_grab(24, 156, OBJECT_W, OBJECT_H);
  sprites.hero[3][1] = sprite_grab(36, 156, OBJECT_W, OBJECT_H);
  sprites.hero[4][1] = sprite_grab(48, 156, OBJECT_W, OBJECT_H);
  sprites.hero[5][1] = sprite_grab(60, 156, OBJECT_W, OBJECT_H);

  sprites.hero[5][2] = sprite_grab(72, 156, OBJECT_W, OBJECT_H);
  sprites.hero[4][2] = sprite_grab(84, 156, OBJECT_W, OBJECT_H);
  sprites.hero[3][2] = sprite_grab(96, 156, OBJECT_W, OBJECT_H);
  sprites.hero[2][2] = sprite_grab(108, 156, OBJECT_W, OBJECT_H);
  sprites.hero[1][2] = sprite_grab(120, 156, OBJECT_W, OBJECT_H);
  sprites.hero[0][2] = sprite_grab(132, 156, OBJECT_W, OBJECT_H);
}

void sprites_deinit()
{
  al_destroy_bitmap(sprites.border);
  al_destroy_bitmap(sprites.boulder);
  al_destroy_bitmap(sprites.earth);
  al_destroy_bitmap(sprites.wall);
  al_destroy_bitmap(sprites.jewel);
  al_destroy_bitmap(sprites.door);

  al_destroy_bitmap(sprites.hero[0][0]);
  al_destroy_bitmap(sprites.hero[1][0]);
  al_destroy_bitmap(sprites.hero[2][0]);
  al_destroy_bitmap(sprites.hero[3][0]);
  al_destroy_bitmap(sprites.hero[4][0]);
  al_destroy_bitmap(sprites.hero[5][0]);

  al_destroy_bitmap(sprites.hero[0][1]);
  al_destroy_bitmap(sprites.hero[1][1]);
  al_destroy_bitmap(sprites.hero[2][1]);
  al_destroy_bitmap(sprites.hero[3][1]);
  al_destroy_bitmap(sprites.hero[4][1]);
  al_destroy_bitmap(sprites.hero[5][1]);

  al_destroy_bitmap(sprites.hero[0][2]);
  al_destroy_bitmap(sprites.hero[1][2]);
  al_destroy_bitmap(sprites.hero[2][2]);
  al_destroy_bitmap(sprites.hero[3][2]);
  al_destroy_bitmap(sprites.hero[4][2]);
  al_destroy_bitmap(sprites.hero[5][2]);

  al_destroy_bitmap(sprites._sheet);
}

typedef enum {
  BOULDER,
  WALL,
  EARTH,
  BORDER,
  JEWEL,
  HERO,
  DOOR
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
  long score;
  int coins;
  bool done;
  bool restart;
} GAME;

void objects_init(MAP *map, OBJECT *boulders, COORDINATE *hero, OBJECT *jewels, GAME *game)
{
  game->frames = 0;
  game->score = 0;
  game->coins = 0;
  game->restart = false;
  game->done = false;

  for (int i=0;i<N_OBJ_W;i++) {
    for (int j=0;j<N_OBJ_H;j++) {
      map->el[i][j].show = false;
    }
  }

  // initialize borders
  for (int j=0; j<N_OBJ_H; j+=N_OBJ_H-1) {
    for (int i=0; i<N_OBJ_W; i++) {
      map->el[i][j].type = BORDER;
      map->el[i][j].show = true;
    }
  }
  for (int i=0; i<N_OBJ_W; i+=N_OBJ_W-1) {
    for (int j=0; j<N_OBJ_H; j++) {
      map->el[i][j].type = BORDER;
      map->el[i][j].show = true;
    }
  }

  FILE *fp;
  int c;

  fp = fopen("map_coordinates.txt", "r");
    if (!fp) {
      fprintf(stderr, "Nao foi possivel abrir o arquivo '%s'\n", "map_coordinates.txt");
      exit(1);
    }

    //check for comments
    c = getc(fp);
    while (c == '#') {
      while (getc(fp) != '\n') ;
      c = getc(fp);
    }
    ungetc(c, fp);

    int x;
    boulders->size=0;
    jewels->size=0;
    hero->left=0;
    hero->right=0;
    for (int j=1; j<N_OBJ_H-1; j++) {
      for (int i=1; i<N_OBJ_W-1; i++) {
        fscanf(fp, "%d", &x);
        switch (x)
        {
        case 0:
          map->el[i][j].show = false;
          break;
        case 1:
          map->el[i][j].type = HERO;
          map->el[i][j].show = true;
          hero->x = i;
          hero->y = j;
          break;
        case 2:
          map->el[i][j].type = DOOR;
          map->el[i][j].show = true;
          break;
        case 3:
          map->el[i][j].type = BOULDER;
          map->el[i][j].show = true;
          boulders->el[boulders->size].x = i;
          boulders->el[boulders->size].y = j;
          boulders->el[boulders->size].falling = false;
          boulders->size++;
          break;
        case 4:
          map->el[i][j].type = WALL;
          map->el[i][j].show = true;
          break;
        case 5:
          map->el[i][j].type = JEWEL;
          map->el[i][j].show = true;
          jewels->el[jewels->size].x = i;
          jewels->el[jewels->size].y = j;
          jewels->el[jewels->size].falling = false;
          jewels->size++;
          break;
        case 6:
          map->el[i][j].type = EARTH;
          map->el[i][j].show = true;
          break;
        
        default:
          break;
        }
      }
    }

  fclose(fp);

}

void draw_map(MAP *map, COORDINATE *hero, GAME *game)
{
  for(int i=0; i<N_OBJ_W; i++) {
    for (int j=0; j<N_OBJ_H; j++) {
      if (map->el[i][j].show) {
        switch (map->el[i][j].type)
        {
        case BOULDER:
          al_draw_bitmap(sprites.boulder, i*OBJECT_W, j*OBJECT_H, 0);
          break;
        case WALL:
          al_draw_bitmap(sprites.wall, i*OBJECT_W, j*OBJECT_H, 0);
          break;
        case EARTH:
          al_draw_bitmap(sprites.earth, i*OBJECT_W, j*OBJECT_H, 0);
          break;
        case BORDER:
          al_draw_bitmap(sprites.border, i*OBJECT_W, j*OBJECT_H, 0);
          break;
        case JEWEL:
          al_draw_bitmap(sprites.jewel, i*OBJECT_W, j*OBJECT_H, 0);
          break;
        case HERO:
          if (hero->left>0) {
            al_draw_bitmap(sprites.hero[hero->left-1][1], i*OBJECT_W, j*OBJECT_H, 0);
          }
          else if (hero->right>0) {
            al_draw_bitmap(sprites.hero[hero->right-1][2], i*OBJECT_W, j*OBJECT_H, 0);
          }
          else al_draw_bitmap(sprites.hero[0][0], i*OBJECT_W, j*OBJECT_H, 0);
          break;
        case DOOR:
          if (game->coins > 11)
            al_draw_bitmap(sprites.door, i*OBJECT_W, j*OBJECT_H, 0);
          else
            al_draw_bitmap(sprites.border, i*OBJECT_W, j*OBJECT_H, 0);
          break;
        
        default:
          break;
        }
      }
    }
  }
}

void hero_update(ALLEGRO_EVENT* event, MAP *map, COORDINATE *hero, OBJECT *boulders, OBJECT *jewels, GAME *game) {

  if (map->el[hero->x][hero->y-1].type == BOULDER && map->el[hero->x][hero->y-1].show) {
    for (int i=0; i<boulders->size; i++) {
      if (hero->x == boulders->el[i].x && hero->y == boulders->el[i].y+1) {
        if (boulders->el[i].falling)
          game->restart = true;
        break;
      }
    }
  }
  else if (map->el[hero->x][hero->y-1].type == JEWEL && map->el[hero->x][hero->y-1].show) {
    for (int i=0; i<jewels->size; i++) {
      if (hero->x == jewels->el[i].x && hero->y == jewels->el[i].y+1) {
        if (jewels->el[i].falling)
          game->restart = true;
        break;
      }
    }
  }

  if(event->keyboard.keycode == ALLEGRO_KEY_UP) {
    hero->left=0;
    hero->right++;
    if (hero->right > 6)
      hero->right=1;

    if (hero->y-1 != 0) {
      if (map->el[hero->x][hero->y-1].show == false) {
        map->el[hero->x][hero->y].show = false;
        map->el[hero->x][hero->y-1].type = HERO;
        map->el[hero->x][hero->y-1].show = true;
        hero->y--;
        al_play_sample(dig, 0.5, 0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
      }
      else {
        switch (map->el[hero->x][hero->y-1].type)
        {
        case EARTH:
          map->el[hero->x][hero->y].show = false;
          map->el[hero->x][hero->y-1].type = HERO;
          hero->y--;
          al_play_sample(dig, 0.5, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
          break;
        case JEWEL:
          game->coins++;
          map->el[hero->x][hero->y].show = false;
          map->el[hero->x][hero->y-1].type = HERO;
          hero->y--;
          al_play_sample(coin, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
          break;
        case DOOR:
          if (game->coins > 11)
            game->done = true;

        default:
          break;
        }
      }
    }
  }
  if(event->keyboard.keycode == ALLEGRO_KEY_DOWN) {
    hero->right=0;
    hero->left++;
    if (hero->left > 6)
      hero->left=1;

    if (hero->y+1 != N_OBJ_H-1) {
      if (map->el[hero->x][hero->y+1].show == false) {
        map->el[hero->x][hero->y].show = false;
        map->el[hero->x][hero->y+1].type = HERO;
        map->el[hero->x][hero->y+1].show = true;
        hero->y++;
        al_play_sample(dig, 0.5, 0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
      }
      else {
        switch (map->el[hero->x][hero->y+1].type)
        {
        case EARTH:
          map->el[hero->x][hero->y].show = false;
          map->el[hero->x][hero->y+1].type = HERO;
          hero->y++;
          al_play_sample(dig, 0.5, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
          break;
        case JEWEL:
          game->coins++;
          al_play_sample(coin, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
          map->el[hero->x][hero->y].show = false;
          map->el[hero->x][hero->y+1].type = HERO;
          hero->y++;
          break;
        case DOOR:
          if (game->coins > 11)
            game->done = true;   

        default:
          break;
        }
      }
    }
  }
  if(event->keyboard.keycode == ALLEGRO_KEY_LEFT) {
    hero->right=0;
    hero->left++;
    if (hero->left > 6)
      hero->left=0;
    if (hero->x-1 != 0) {
      if (map->el[hero->x-1][hero->y].show == false) {
        map->el[hero->x][hero->y].show = false;
        map->el[hero->x-1][hero->y].type = HERO;
        map->el[hero->x-1][hero->y].show = true;
        hero->x--;
        al_play_sample(dig, 0.5, 0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
      }
      else {
        switch (map->el[hero->x-1][hero->y].type)
        {
        case EARTH:
          map->el[hero->x][hero->y].show = false;
          map->el[hero->x-1][hero->y].type = HERO;
          hero->x--;
          al_play_sample(dig, 0.5, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
          break;
        case JEWEL:
          game->coins++;
          al_play_sample(coin, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
          map->el[hero->x][hero->y].show = false;
          map->el[hero->x-1][hero->y].type = HERO;
          hero->x--;
          break;
        case BOULDER:
          if (map->el[hero->x-2][hero->y].show == false && game->frames % 40 == 0) {
            map->el[hero->x][hero->y].show = false;
            map->el[hero->x-1][hero->y].type = HERO;
            hero->x--;
            for (int i=0; i<boulders->size; i++) {
              if (boulders->el[i].x == hero->x && boulders->el[i].y == hero->y) {
                boulders->el[i].x--;
                break;
              }
            }
            map->el[hero->x-1][hero->y].type = BOULDER;
            map->el[hero->x-1][hero->y].show = true;
          }
          break;
        case DOOR:
          if (game->coins > 11)
            game->done = true;
        
        default:
          break;
        }
      }
    }
  }
  if(event->keyboard.keycode == ALLEGRO_KEY_RIGHT) {
    hero->left=0;
    hero->right++;
    if (hero->right > 6)
      hero->right=0;
    if (hero->x+1 != N_OBJ_W-1) {
      if (map->el[hero->x+1][hero->y].show == false) {
        map->el[hero->x][hero->y].show = false;
        map->el[hero->x+1][hero->y].type = HERO;
        map->el[hero->x+1][hero->y].show = true;
        hero->x++;
        al_play_sample(dig, 0.5, 0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
      }
      else {
        switch (map->el[hero->x+1][hero->y].type)
        {
        case EARTH:
          map->el[hero->x][hero->y].show = false;
          map->el[hero->x+1][hero->y].type = HERO;
          hero->x++;
          al_play_sample(dig, 0.5, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
          break;
        case JEWEL:
          game->coins++;
          al_play_sample(coin, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
          map->el[hero->x][hero->y].show = false;
          map->el[hero->x+1][hero->y].type = HERO;
          hero->x++;
          break;
        case BOULDER:
          if (map->el[hero->x+2][hero->y].show == false &&  game->frames % 40 == 0) {
            map->el[hero->x][hero->y].show = false;
            map->el[hero->x+1][hero->y].type = HERO;
            hero->x++;
            for (int i=0; i<boulders->size; i++) {
              if (boulders->el[i].x == hero->x && boulders->el[i].y == hero->y) {
                boulders->el[i].x++;
                break;
              }
            }
            map->el[hero->x+1][hero->y].type = BOULDER;
            map->el[hero->x+1][hero->y].show = true;
          }
          break;
        case DOOR:
          if (game->coins > 11)
            game->done = true;

        default:
          break;
        }
      }
    }
  }
}

void boulders_update(MAP *map, OBJECT *boulders, GAME *game) {
  for (int i=0; i<boulders->size; i++) {
    if ((game->frames % 10) == 0) {

      if (boulders->el[i].falling && map->el[boulders->el[i].x][boulders->el[i].y+1].show == true) {
        boulders->el[i].falling = false;
        al_play_sample(boulder, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
      }

      if (map->el[boulders->el[i].x][boulders->el[i].y+1].show == false) {
        map->el[boulders->el[i].x][boulders->el[i].y].show = false;
        map->el[boulders->el[i].x][boulders->el[i].y+1].type = BOULDER;
        map->el[boulders->el[i].x][boulders->el[i].y+1].show = true;
        boulders->el[i].falling = true;
        boulders->el[i].y++;
      }

      else if ((((map->el[boulders->el[i].x][boulders->el[i].y+1].type == BOULDER) || (map->el[boulders->el[i].x][boulders->el[i].y+1].type == WALL) 
      || (map->el[boulders->el[i].x][boulders->el[i].y+1].type == JEWEL)) && map->el[boulders->el[i].x][boulders->el[i].y+1].show == true) &&
      !(((map->el[boulders->el[i].x][boulders->el[i].y-1].type == BOULDER) || (map->el[boulders->el[i].x][boulders->el[i].y-1].type == JEWEL)) 
      && (map->el[boulders->el[i].x][boulders->el[i].y-1].show == true))) {


        if (map->el[boulders->el[i].x-1][boulders->el[i].y].show == false && map->el[boulders->el[i].x-1][boulders->el[i].y+1].show == false) {
          map->el[boulders->el[i].x][boulders->el[i].y].show = false;
          map->el[boulders->el[i].x-1][boulders->el[i].y].type = BOULDER;
          map->el[boulders->el[i].x-1][boulders->el[i].y].show = true;
          boulders->el[i].x--;
        }
        if (map->el[boulders->el[i].x+1][boulders->el[i].y].show == false && map->el[boulders->el[i].x+1][boulders->el[i].y+1].show == false) {
          map->el[boulders->el[i].x][boulders->el[i].y].show = false;
          map->el[boulders->el[i].x+1][boulders->el[i].y].type = BOULDER;
          map->el[boulders->el[i].x+1][boulders->el[i].y].show = true;
          boulders->el[i].x++;
        }
      }


    }
  }
}

void jewels_update(MAP *map, OBJECT *jewels, GAME *game) {
  for (int i=0; i<jewels->size; i++) {
    if ((game->frames % 10 == 0) && map->el[jewels->el[i].x][jewels->el[i].y].show == true && map->el[jewels->el[i].x][jewels->el[i].y].type == JEWEL) {

      if (jewels->el[i].falling && map->el[jewels->el[i].x][jewels->el[i].y+1].show == true) {
        jewels->el[i].falling = false;
        al_play_sample(coin_fall, 0.7, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
      }

      if (map->el[jewels->el[i].x][jewels->el[i].y+1].show == false) {
        map->el[jewels->el[i].x][jewels->el[i].y].show = false;
        map->el[jewels->el[i].x][jewels->el[i].y+1].type = JEWEL;
        map->el[jewels->el[i].x][jewels->el[i].y+1].show = true;
        jewels->el[i].falling = true;
        jewels->el[i].y++;
      }

      else if ((((map->el[jewels->el[i].x][jewels->el[i].y+1].type == BOULDER) || (map->el[jewels->el[i].x][jewels->el[i].y+1].type == WALL) 
      || (map->el[jewels->el[i].x][jewels->el[i].y+1].type == JEWEL)) && map->el[jewels->el[i].x][jewels->el[i].y+1].show == true) &&
      !(((map->el[jewels->el[i].x][jewels->el[i].y-1].type == BOULDER) || (map->el[jewels->el[i].x][jewels->el[i].y-1].type == JEWEL))
      && (map->el[jewels->el[i].x][jewels->el[i].y-1].show == true))) {

        if (map->el[jewels->el[i].x-1][jewels->el[i].y].show == false && map->el[jewels->el[i].x-1][jewels->el[i].y+1].show == false) {
          map->el[jewels->el[i].x][jewels->el[i].y].show = false;
          map->el[jewels->el[i].x-1][jewels->el[i].y].type = JEWEL;
          map->el[jewels->el[i].x-1][jewels->el[i].y].show = true;
          jewels->el[i].x--;
        }
        if (map->el[jewels->el[i].x+1][jewels->el[i].y].show == false && map->el[jewels->el[i].x+1][jewels->el[i].y+1].show == false) {
          map->el[jewels->el[i].x][jewels->el[i].y].show = false;
          map->el[jewels->el[i].x+1][jewels->el[i].y].type = JEWEL;
          map->el[jewels->el[i].x+1][jewels->el[i].y].show = true;
          jewels->el[i].x++;
        }
      }
    }
  }
}

void restart(MAP *map, COORDINATE *hero, GAME *game) {
  for (int i=hero->x-1; i<hero->x+2; i++) {
    for (int j=hero->y-1; j<hero->y+2; j++) {
      map->el[i][j].show = false;
    }
  }
}

int main()
{
  must_init(al_init(), "allegro");
  must_init(al_install_keyboard(), "keyboard");

  ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
  must_init(timer, "timer");

  ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
  must_init(queue, "queue");

  disp_init();

  audio_init();

  must_init(al_init_image_addon(), "image");
  sprites_init();

  // hud_init();

  must_init(al_init_primitives_addon(), "primitives");

  must_init(al_install_audio(), "audio");
  must_init(al_init_acodec_addon(), "audio codecs");
  must_init(al_reserve_samples(16), "reserve samples");

  al_register_event_source(queue, al_get_keyboard_event_source());
  al_register_event_source(queue, al_get_display_event_source(disp));
  al_register_event_source(queue, al_get_timer_event_source(timer));

  COORDINATE hero;
  OBJECT boulders;
  OBJECT jewels;
  MAP map;
  GAME game;

  objects_init(&map, &boulders, &hero, &jewels, &game);

  bool redraw = true;
  ALLEGRO_EVENT event;

  al_start_timer(timer);

  while(1)
  {
    al_wait_for_event(queue, &event);

    switch(event.type)
    {
      case ALLEGRO_EVENT_TIMER:
        if (game.restart) {
          restart(&map, &hero, &game);
          disp_pre_draw();
          al_clear_to_color(al_map_rgb(0,0,0));
          draw_map(&map, &hero, &game);
          disp_post_draw();
          redraw = false;
          usleep(2000000);
          objects_init(&map, &boulders, &hero, &jewels, &game);
        }
          if (game.frames % 20 == 0) {
          hero.left=0;
          hero.right=0;
        }
        // boulders_update(&map, &boulders, &game);
        // jewels_update(&map, &jewels, &game);
      case ALLEGRO_EVENT_KEY_CHAR:
        // earth_update();
        // hud_update();
        hero_update(&event, &map, &hero, &boulders, &jewels, &game);
        boulders_update(&map, &boulders, &game);
        jewels_update(&map, &jewels, &game);

        redraw = true;
        game.frames++;
        if(event.keyboard.keycode != ALLEGRO_KEY_ESCAPE)
          break;

      case ALLEGRO_EVENT_DISPLAY_CLOSE:
        game.done = true;
        break;
    }

    if(game.done)
      break;


    if(redraw && al_is_event_queue_empty(queue))
    {
      disp_pre_draw();
      al_clear_to_color(al_map_rgb(0,0,0));

      draw_map(&map, &hero, &game);
      // stars_draw();
      // aliens_draw();
      // shots_draw();
      // fx_draw();
      // ship_draw();

      // hud_draw();

      disp_post_draw();
      redraw = false;
    }
  }

  sprites_deinit();
  // hud_deinit();
  audio_deinit();
  disp_deinit();
  al_destroy_timer(timer);
  al_destroy_event_queue(queue);

  return 0;
}