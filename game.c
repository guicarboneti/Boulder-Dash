// Made by Guilherme Carbonari Boneti, GRR20196478, 02/2021.

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "game.h"

void must_init(bool test, const char *description)
{
  if(test) return;

  fprintf(stderr ,"couldn't initialize %s\n", description);
  exit(1);
}

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

void audio_init()
{
  al_install_audio();
  al_init_acodec_addon();
  al_reserve_samples(128);

  coin = al_load_sample("./resources/coin.wav");
  dig = al_load_sample("./resources/dig.flac");
  boulder = al_load_sample("./resources/boulder.wav");
  coin_fall = al_load_sample("./resources/coin-fall.wav");
  boom = al_load_sample("./resources/boom.wav");
  lose = al_load_sample("./resources/lose.wav");
  score = al_load_sample("./resources/score.wav");
  menu = al_load_sample("./resources/menu.wav");
  menu_select = al_load_sample("./resources/menu_select.wav");
  cheat = al_load_sample("./resources/cheat.ogg");
  win = al_load_sample("./resources/win.flac");

  must_init(coin, "coin sample");
  must_init(dig, "dig sample");
  must_init(boulder, "boulder sample");
  must_init(coin_fall, "coin fall sample");
  must_init(boom, "boom sample");
  must_init(lose, "lose sample");
  must_init(score, "score sample");
  must_init(menu, "menu sample");
  must_init(menu_select, "menu select sample");
  must_init(cheat, "cheat sample");
  must_init(win, "win sample");
}

void audio_deinit()
{
  al_destroy_sample(coin);
  al_destroy_sample(dig);
  al_destroy_sample(boulder);
  al_destroy_sample(coin_fall);
  al_destroy_sample(boom);
  al_destroy_sample(lose);
  al_destroy_sample(score);
  al_destroy_sample(menu);
  al_destroy_sample(menu_select);
  al_destroy_sample(cheat);
  al_destroy_sample(win);
}

ALLEGRO_BITMAP* sprite_grab(int x, int y, int w, int h)
{
  ALLEGRO_BITMAP* sprite = al_create_sub_bitmap(sprites._sheet, x, y, w, h);
  must_init(sprite, "sprite grab");
  return sprite;
}
ALLEGRO_BITMAP* sprite_grab_smoke(int x, int y, int w, int h)
{
  ALLEGRO_BITMAP* sprite = al_create_sub_bitmap(sprites.smoke_sheet, x, y, w, h);
  must_init(sprite, "smoke sprite grab");
  return sprite;
}

void image_init() {
  infinite = al_load_bitmap("./resources/infinite.png");
  must_init(infinite, "infinite");
}
void image_deinit()
{
  al_destroy_bitmap(infinite);
}

void sprites_init()
{
  sprites._sheet = al_load_bitmap("./resources/tileset.png");
  must_init(sprites._sheet, "spritesheet");

  sprites.border = sprite_grab(228, 36, OBJECT_W, OBJECT_H);
  sprites.earth = sprite_grab(36, 36, OBJECT_W, OBJECT_H);
  sprites.boulder = sprite_grab(0, 204, OBJECT_W, OBJECT_H);
  sprites.wall = sprite_grab(0, 48, OBJECT_W, OBJECT_H);
  sprites.jewel = sprite_grab(108, 48, OBJECT_W, OBJECT_H);
  sprites.door = sprite_grab(0, 84, OBJECT_W, OBJECT_H);

  sprites.hero[0][0] = sprite_grab(24, 144, OBJECT_W, OBJECT_H);
  sprites.hero[1][0] = sprite_grab(0, 144, OBJECT_W, OBJECT_H);
  sprites.hero[2][0] = sprite_grab(12, 144, OBJECT_W, OBJECT_H);
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

  sprites.smoke_sheet = al_load_bitmap("./resources/smoke.png");
  must_init(sprites._sheet, "spritesheet");

  sprites.smoke[0] = sprite_grab_smoke(0, 0, 16, 16);
  sprites.smoke[1] = sprite_grab_smoke(17, 0, 16, 16);
  sprites.smoke[2] = sprite_grab_smoke(34, 0, 16, 16);
  sprites.smoke[3] = sprite_grab_smoke(51, 0, 16, 16);
  sprites.smoke[4] = sprite_grab_smoke(0, 17, 16, 16);
  sprites.smoke[5] = sprite_grab_smoke(17, 17, 16, 16);
  sprites.smoke[6] = sprite_grab_smoke(34, 17, 16, 16);
  sprites.smoke[7] = sprite_grab_smoke(51, 17, 16, 16);

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
  al_destroy_bitmap(sprites.smoke_sheet);

  al_destroy_bitmap(sprites.smoke[0]);
  al_destroy_bitmap(sprites.smoke[1]);
  al_destroy_bitmap(sprites.smoke[2]);
  al_destroy_bitmap(sprites.smoke[3]);
  al_destroy_bitmap(sprites.smoke[4]);
  al_destroy_bitmap(sprites.smoke[5]);
  al_destroy_bitmap(sprites.smoke[6]);
  al_destroy_bitmap(sprites.smoke[7]);
}

void objects_init(MAP *map, OBJECT *boulders, COORDINATE *hero, OBJECT *jewels, GAME *game)
{
  game->frames = 0;
  game->coins = 0;
  game->smoke_index = 0;
  game->restart = false;
  game->finish = false;
  game->die = false;
  game->done = false;
  game->pause = false;
  
  for (int i=0;i<2;i++)
    for (int j=0;j<3;j++)
      game->pause_set[i][j] = 0;

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

  // read map info
  FILE *fp;
  int c;

  if (game->map == 1) {
    fp = fopen("./resources/map_coordinates.txt", "r");
    must_init(fp, "map.coordinates.txt");
  }
  else if (game->map == 2) {
    fp = fopen("./resources/map2_coordinates.txt", "r");
    must_init(fp, "map2.coordinates.txt");
  }


    // check for comments
    c = getc(fp);
    while (c == '#') {
      while (getc(fp) != '\n') ;
      c = getc(fp);
    }
    ungetc(c, fp);

    fscanf(fp, "%d %d %d %d", &game->door_coins, &game->pts_per_coin, &game->pts_gain, &game->time);

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
          al_draw_bitmap(sprites.boulder, i*OBJECT_W, (j*OBJECT_H)+OBJECT_H, 0);
          break;
        case WALL:
          al_draw_bitmap(sprites.wall, i*OBJECT_W, (j*OBJECT_H)+OBJECT_H, 0);
          break;
        case EARTH:
          al_draw_bitmap(sprites.earth, i*OBJECT_W, (j*OBJECT_H)+OBJECT_H, 0);
          break;
        case BORDER:
          al_draw_bitmap(sprites.border, i*OBJECT_W, (j*OBJECT_H)+OBJECT_H, 0);
          break;
        case JEWEL:
          al_draw_bitmap(sprites.jewel, i*OBJECT_W, (j*OBJECT_H)+OBJECT_H, 0);
          break;
        case HERO:
          if (game->finish) {
            if (game->frames % 4 == 0)
              al_draw_bitmap(sprites.hero[1][0], i*OBJECT_W, (j*OBJECT_H)+OBJECT_H, 0);
            else al_draw_bitmap(sprites.hero[2][0], i*OBJECT_W, (j*OBJECT_H)+OBJECT_H, 0);
          }
          else if (hero->left>0) {
            al_draw_bitmap(sprites.hero[hero->left-1][1], i*OBJECT_W, (j*OBJECT_H)+OBJECT_H, 0);
          }
          else if (hero->right>0) {
            al_draw_bitmap(sprites.hero[hero->right-1][2], i*OBJECT_W, (j*OBJECT_H)+OBJECT_H, 0);
          }
          else al_draw_bitmap(sprites.hero[0][0], i*OBJECT_W, (j*OBJECT_H)+OBJECT_H, 0);
          break;
        case DOOR:
          if (game->coins >= game->door_coins)
            al_draw_bitmap(sprites.door, i*OBJECT_W, (j*OBJECT_H)+OBJECT_H, 0);
          else
            al_draw_bitmap(sprites.border, i*OBJECT_W, (j*OBJECT_H)+OBJECT_H, 0);

          break;
        case SMOKE:
          al_draw_scaled_bitmap(sprites.smoke[game->smoke_index-1], 0, 0, 16, 16, i*OBJECT_W, (j*OBJECT_H)+OBJECT_H, 12, 12, 0);

        default:
          break;
        }
      }
    }
  }

  // draw pause menu
  if (game->pause) {
    al_draw_filled_rectangle(0, 0, BUFFER_W, BUFFER_H, al_map_rgba_f(0.15,0,0.15,0.9));
    al_draw_text(
      font,
      al_map_rgb_f(1,1,1),
      BUFFER_W / 2, (BUFFER_H/4)-(2*OBJECT_H),
      ALLEGRO_ALIGN_CENTER,
      "P A U S E D"
    );
    if (!game->pause_set[1][1]) {
      if (game->pause_set[0][0])
        al_draw_filled_rectangle((BUFFER_W/4)+OBJECT_W-3 , (BUFFER_H/4)+OBJECT_H-3, (3*(BUFFER_W/4))-OBJECT_W+3, (BUFFER_H/4)+ 4*OBJECT_H+3, al_map_rgb(140,140,140));
      else al_draw_filled_rectangle((BUFFER_W/4)+OBJECT_W , (BUFFER_H/4)+OBJECT_H, (3*(BUFFER_W/4))-OBJECT_W, (BUFFER_H/4)+ 4*OBJECT_H, al_map_rgb(102,102,102));
      al_draw_text(font, al_map_rgb_f(0,0,0), (BUFFER_W/4)+10*OBJECT_W, (BUFFER_H/4)+(2.3*OBJECT_H), ALLEGRO_ALIGN_CENTER, "R E S U M E");

      if (game->pause_set[0][1])
        al_draw_filled_rectangle((BUFFER_W/4)+OBJECT_W-3 , ((BUFFER_H/4)+(5*OBJECT_H)-(OBJECT_H/2)-3), (3*(BUFFER_W/4))-OBJECT_W+3, ((BUFFER_H/4)+(8*OBJECT_H)-(OBJECT_H/2))+3, al_map_rgb(140,140,140));
      else al_draw_filled_rectangle((BUFFER_W/4)+OBJECT_W , ((BUFFER_H/4)+(5*OBJECT_H)-(OBJECT_H/2)), (3*(BUFFER_W/4))-OBJECT_W, ((BUFFER_H/4)+(8*OBJECT_H)-(OBJECT_H/2)), al_map_rgb(102,102,102));
      al_draw_text(font, al_map_rgb_f(0,0,0), (BUFFER_W/4)+10*OBJECT_W, ((BUFFER_H/4)+(5.7*OBJECT_H)), ALLEGRO_ALIGN_CENTER, "I N S T R U C T I O N S");

      if (game->pause_set[0][2])
        al_draw_filled_rectangle((BUFFER_W/4)+OBJECT_W-3 , ((BUFFER_H/4)+(8*OBJECT_H))-3, (3*(BUFFER_W/4))-OBJECT_W+3, ((BUFFER_H/4)+(11*OBJECT_H))+3, al_map_rgb(140,140,140));
      else al_draw_filled_rectangle((BUFFER_W/4)+OBJECT_W , ((BUFFER_H/4)+(8*OBJECT_H)), (3*(BUFFER_W/4))-OBJECT_W, ((BUFFER_H/4)+(11*OBJECT_H)), al_map_rgb(102,102,102));
      al_draw_text(font, al_map_rgb_f(0,0,0), (BUFFER_W/4)+10*OBJECT_W, ((BUFFER_H/4)+(9.3*OBJECT_H)), ALLEGRO_ALIGN_CENTER, "E X I T  G A M E");
    
      al_draw_text(font, al_map_rgb_f(1,1,1), BUFFER_W-5*OBJECT_W, 2*OBJECT_H, ALLEGRO_ALIGN_CENTER, "BEST SCORE");
      al_draw_textf(font, al_map_rgb_f(1,1,1), BUFFER_W-5*OBJECT_W, 3*OBJECT_H, ALLEGRO_ALIGN_CENTER, "%d POINTS", game->current_score);
    }

    // instructions page
    else {
      if (game->frames % 100 != 0)
        al_draw_text(font, al_map_rgb_f(1,1,1), 2*OBJECT_W, 2*OBJECT_H, 0, "ESC TO RETURN");

      al_draw_text(font, al_map_rgb_f(1,1,1), (BUFFER_W/4)+10*OBJECT_W, (BUFFER_H/4)+(1.3*OBJECT_H), ALLEGRO_ALIGN_CENTER, "MOVE ROCKFORD WITH ARROW KEYS OR WASD");
      al_draw_text(font, al_map_rgb_f(1,1,1), (BUFFER_W/4)+10*OBJECT_W, (BUFFER_H/4)+(2.3*OBJECT_H), ALLEGRO_ALIGN_CENTER, "AND TRY TO REACH THE DOOR.");
      al_draw_text(font, al_map_rgb_f(1,1,1), (BUFFER_W/4)+10*OBJECT_W, (BUFFER_H/4)+(4.3*OBJECT_H), ALLEGRO_ALIGN_CENTER, "PICK UP ALL THE JEWELS YOU CAN");
      al_draw_text(font, al_map_rgb_f(1,1,1), (BUFFER_W/4)+10*OBJECT_W, (BUFFER_H/4)+(5.3*OBJECT_H), ALLEGRO_ALIGN_CENTER, "AND AVOID GETTING HIT BY THE ROCKS!");
      al_draw_text(font, al_map_rgb_f(1,1,1), (BUFFER_W/4)+10*OBJECT_W, (BUFFER_H/4)+(7.3*OBJECT_H), ALLEGRO_ALIGN_CENTER, "THE TIME IS YOUR ENEMY, GOOD LUCK!");
      al_draw_text(font, al_map_rgb_f(1,1,1), (BUFFER_W/4)+10*OBJECT_W, (BUFFER_H/4)+(10.3*OBJECT_H), ALLEGRO_ALIGN_CENTER, "GAME DEVELOPED BY GUILHERME CARBONARI BONETI");
      al_draw_text(font, al_map_rgb_f(1,1,1), (BUFFER_W/4)+10*OBJECT_W, (BUFFER_H/4)+(11.3*OBJECT_H), ALLEGRO_ALIGN_CENTER, "MARCH, 2021");
    }
  }

  // win page
  if (game->finish && game->map==MAPS_N && game->time==0) {
    al_draw_filled_rectangle(0, 0, BUFFER_W, BUFFER_H, al_map_rgba_f(0,0.15,0.15,0.9));
    al_draw_text(
      font,
      al_map_rgb_f(1,1,1),
      BUFFER_W / 2, (BUFFER_H/2)-(4*OBJECT_H),
      ALLEGRO_ALIGN_CENTER,
      "C O N G R A T U L A T I O N S ,"
    );
    al_draw_text(
      font,
      al_map_rgb_f(1,1,1),
      BUFFER_W / 2, (BUFFER_H/2)-(2*OBJECT_H),
      ALLEGRO_ALIGN_CENTER,
      "Y O U  F I N I S H E D  T H E  G A M E !"
    );

    long sum=0;
    for (int i=game->map-1; i>=0; i--)
      sum = sum + game->score[i];
    al_draw_textf(
      font,
      al_map_rgb_f(1,1,1),
      BUFFER_W / 2, (BUFFER_H/2)+OBJECT_H,
      ALLEGRO_ALIGN_CENTER,
      "YOUR SCORE WAS %ld POINTS",
      sum
    );

    // play sample only once
    if (!game->infinite_time) {
      al_play_sample(win, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
      game->infinite_time=true;
    }
  }
}

void hero_update(ALLEGRO_EVENT* event, MAP *map, COORDINATE *hero, OBJECT *boulders, OBJECT *jewels, GAME *game) {

  // restart if something falls on hero's head
  if (map->el[hero->x][hero->y-1].type == BOULDER && map->el[hero->x][hero->y-1].show) {
    for (int i=0; i<boulders->size; i++) {
      if (hero->x == boulders->el[i].x && hero->y == boulders->el[i].y+1) {
        if (boulders->el[i].falling) {
          al_play_sample(boom, 0.5, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
          game->restart = true;
        }
        break;
      }
    }
  }
  else if (map->el[hero->x][hero->y-1].type == JEWEL && map->el[hero->x][hero->y-1].show) {
    for (int i=0; i<jewels->size; i++) {
      if (hero->x == jewels->el[i].x && hero->y == jewels->el[i].y+1) {
        if (jewels->el[i].falling) {
          al_play_sample(boom, 0.5, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
          game->restart = true;
        }
        break;
      }
    }
  }

  // moving around
  if(event->keyboard.keycode == ALLEGRO_KEY_UP || event->keyboard.keycode == ALLEGRO_KEY_W) {
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
          if (game->coins < game->door_coins)
            game->score[game->map-1] = game->score[game->map-1] + game->pts_per_coin;
          else
            game->score[game->map-1] = game->score[game->map-1] + game->pts_per_coin + game->pts_gain;
          game->coins++;
          map->el[hero->x][hero->y].show = false;
          map->el[hero->x][hero->y-1].type = HERO;
          hero->y--;
          al_play_sample(coin, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
          break;
        case DOOR:
          if (game->coins >= game->door_coins) {
            map->el[hero->x][hero->y].show = false;
            map->el[hero->x][hero->y-1].type = HERO;
            map->el[hero->x][hero->y-1].show = true;
            hero->y--;
            game->finish = true;
          }

        default:
          break;
        }
      }
    }
  }
  if(event->keyboard.keycode == ALLEGRO_KEY_DOWN || event->keyboard.keycode == ALLEGRO_KEY_S) {
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
          if (game->coins < game->door_coins)
            game->score[game->map-1] = game->score[game->map-1] + game->pts_per_coin;
          else
            game->score[game->map-1] = game->score[game->map-1] + game->pts_per_coin + game->pts_gain;
          game->coins++;
          al_play_sample(coin, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
          map->el[hero->x][hero->y].show = false;
          map->el[hero->x][hero->y+1].type = HERO;
          hero->y++;
          break;
        case DOOR:
          if (game->coins >= game->door_coins) {
            map->el[hero->x][hero->y].show = false;
            map->el[hero->x][hero->y+1].type = HERO;
            map->el[hero->x][hero->y+1].show = true;
            hero->y++;
            game->finish = true;
          } 

        default:
          break;
        }
      }
    }
  }
  if(event->keyboard.keycode == ALLEGRO_KEY_LEFT || event->keyboard.keycode == ALLEGRO_KEY_A) {
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
          if (game->coins < game->door_coins)
            game->score[game->map-1] = game->score[game->map-1] + game->pts_per_coin;
          else
            game->score[game->map-1] = game->score[game->map-1] + game->pts_per_coin + game->pts_gain;
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
          if (game->coins >= game->door_coins) {
            map->el[hero->x][hero->y].show = false;
            map->el[hero->x-1][hero->y].type = HERO;
            map->el[hero->x-1][hero->y].show = true;
            hero->x--;
            game->finish = true;
          }
        
        default:
          break;
        }
      }
    }
  }
  if(event->keyboard.keycode == ALLEGRO_KEY_RIGHT || event->keyboard.keycode == ALLEGRO_KEY_D) {
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
          if (game->coins < game->door_coins)
            game->score[game->map-1] = game->score[game->map-1] + game->pts_per_coin;
          else
            game->score[game->map-1] = game->score[game->map-1] + game->pts_per_coin + game->pts_gain;
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
          if (game->coins >= game->door_coins) {
            map->el[hero->x][hero->y].show = false;
            map->el[hero->x+1][hero->y].type = HERO;
            map->el[hero->x+1][hero->y].show = true;
            hero->x++;
            game->finish = true;
          }

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

      // hit something, stop falling
      if (boulders->el[i].falling && map->el[boulders->el[i].x][boulders->el[i].y+1].show == true) {
        boulders->el[i].falling = false;
        al_play_sample(boulder, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
      }

      // that's gravity
      if (map->el[boulders->el[i].x][boulders->el[i].y+1].show == false) {
        map->el[boulders->el[i].x][boulders->el[i].y].show = false;
        map->el[boulders->el[i].x][boulders->el[i].y+1].type = BOULDER;
        map->el[boulders->el[i].x][boulders->el[i].y+1].show = true;
        boulders->el[i].falling = true;
        boulders->el[i].y++;
      }

      // rocks don't stay on each other, neither on jewels, nor on walls
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

      // hit something, stop falling
      if (jewels->el[i].falling && map->el[jewels->el[i].x][jewels->el[i].y+1].show == true) {
        jewels->el[i].falling = false;
        al_play_sample(coin_fall, 0.7, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
      }

      // also gravity
      if (map->el[jewels->el[i].x][jewels->el[i].y+1].show == false) {
        map->el[jewels->el[i].x][jewels->el[i].y].show = false;
        map->el[jewels->el[i].x][jewels->el[i].y+1].type = JEWEL;
        map->el[jewels->el[i].x][jewels->el[i].y+1].show = true;
        jewels->el[i].falling = true;
        jewels->el[i].y++;
      }

      // jewels also don't stay on each other, neither on rocks, nor on walls
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
      map->el[i][j].type = SMOKE;
      map->el[i][j].show = true;
    }
  }
}

void hud_init(GAME *game)
{
  font = al_create_builtin_font();
  must_init(font, "font");
}

void hud_deinit()
{
  al_destroy_font(font);
}

void hud_update(GAME *game)
{
  // calculate score after finishing map
  if(game->finish && game->frames % 2 == 0 && game->time > 0) {
    game->score[game->map-1]++;
    game->time--;
    al_play_sample(score, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
  }
}

void hud_draw(GAME *game)
{
  al_draw_textf(
    font,
    al_map_rgb(255,255,1),
    43, 2,
    0,
    "%02d",
    game->door_coins
  );

  al_draw_bitmap(sprites.jewel, 59, 0, 0);
  
  if (game->coins < game->door_coins) {
    al_draw_textf(
      font,
      al_map_rgb_f(1,1,1),
      72, 2,
      0,
      "%02d",
      game->pts_per_coin
    );
  }
  else {
    al_draw_textf(
      font,
      al_map_rgb_f(1,1,1),
      72, 2,
      0,
      "%02d",
      game->pts_per_coin+game->pts_gain
    );
  }

  al_draw_textf(
    font,
    al_map_rgb(255,255,1),
    155, 2,
    0,
    "%02d",
    game->coins
  );

  if (!game->infinite_time) {
    al_draw_textf(
      font,
      al_map_rgb_f(1,1,1),
      311, 2,
      0,
      "%03d",
      game->time
    );
  }
  else {
    al_draw_tinted_scaled_bitmap(infinite, al_map_rgb(26, 198, 255), 0, 0, 600, 279, 311, 2, 20, 8, 0);
  }

  long sum=0;
  for (int i=game->map-1;i>=0;i--)
    sum = sum + game->score[i];
  al_draw_textf(
    font,
    al_map_rgb_f(1,1,1),
    383, 3,
    0,
    "%06ld",
    sum
  );
}