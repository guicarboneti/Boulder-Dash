// Developed by Guilherme Carbonari Boneti, GRR20196478, 02/2021.

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "game.h"

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
  image_init();
  sprites_init();

  must_init(al_init_primitives_addon(), "primitives");

  must_init(al_install_audio(), "audio");
  must_init(al_init_acodec_addon(), "audio codecs");
  must_init(al_reserve_samples(16), "reserve samples");

  al_register_event_source(queue, al_get_keyboard_event_source());
  al_register_event_source(queue, al_get_display_event_source(disp));
  al_register_event_source(queue, al_get_timer_event_source(timer));

  COORDINATE *hero = (COORDINATE*)malloc(sizeof(COORDINATE));
  must_init(hero, "hero struct");

  OBJECT *boulders = (OBJECT*)malloc(sizeof(OBJECT));
  must_init(boulders, "boulders struct");

  OBJECT *jewels = (OBJECT*)malloc(sizeof(OBJECT));
  must_init(jewels, "jewels struct");

  MAP *map = (MAP*)malloc(sizeof(MAP));
  must_init(map, "map struct");

  GAME *game = (GAME*)malloc(sizeof(GAME));
  must_init(game, "game struct");

  game->map=1;
  for (int i=MAPS_N;i>=0;i--)
    game->score[i]=0;
  
  game->cheat_index=0;
  game->infinite_time = false;

  // read current best score
  FILE *score_file ;
  int c;
  score_file = fopen ("./resources/record.txt", "r") ;
  must_init(score_file, "record.txt");
    // check for comments
    c = getc(score_file);
    while (c == '#') {
      while (getc(score_file) != '\n') ;
      c = getc(score_file);
    }
    ungetc(c, score_file);

    fscanf(score_file, "%d", &game->current_score);
  fclose (score_file) ;

  hud_init(game);
  
  objects_init(map, boulders, hero, jewels, game);

  bool redraw = true;
  ALLEGRO_EVENT event;

  al_start_timer(timer);

  while(1)
  {
    al_wait_for_event(queue, &event);

    switch(event.type)
    {
      case ALLEGRO_EVENT_TIMER:

        // finished map
        if (game->time == 0 && game->finish && game->map<MAPS_N) {
          game->map++;
          if (game->map<=MAPS_N)
            game->infinite_time=false;
          game->finish = false;
          objects_init(map, boulders, hero, jewels, game);
        }

        // time is over
        if (game->time == 0 && !game->finish && !game->restart) {
          game->restart = true;
          game->score[game->map-1] = 0;
          al_play_sample(boom, 0.5, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        }
        else if (game->frames % 60 == 0 && !game->finish && !game->pause && !game->infinite_time)
          game->time--;

        // restart game
        if (game->restart && !game->pause) {
          restart(map, hero, game);
          if (game->frames % 20 == 0 || game->smoke_index == 0)
            game->smoke_index++;

          if (game->smoke_index > 8) {
            al_play_sample(lose, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            objects_init(map, boulders, hero, jewels, game);
          }
          game->score[game->map-1]=0;
          game->infinite_time=false;
        }

        // hero stopped, reposition
        if (game->frames % 20 == 0) {
          hero->left=0;
          hero->right=0;
        }

      case ALLEGRO_EVENT_KEY_CHAR:

        // game pause menu
        if(event.keyboard.keycode == ALLEGRO_KEY_H || event.keyboard.keycode == ALLEGRO_KEY_F1) {
          game->pause = !game->pause;
          game->pause_set[0][0] = 1;
        }

        // pause menu navigation
        if((event.keyboard.keycode == ALLEGRO_KEY_UP  || event.keyboard.keycode == ALLEGRO_KEY_W) && game->pause) {
          if (game->pause_set[0][1] == 1) {
            game->pause_set[0][1] = 0;
            game->pause_set[0][0] = 1;
          }
          else if (game->pause_set[0][2] == 1) {
            game->pause_set[0][2] = 0;
            game->pause_set[0][1] = 1;
          }
          al_play_sample(menu, 0.3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        }
        if((event.keyboard.keycode == ALLEGRO_KEY_DOWN || event.keyboard.keycode == ALLEGRO_KEY_S) && game->pause) {
          if (game->pause_set[0][0] == 1) {
            game->pause_set[0][0] = 0;
            game->pause_set[0][1] = 1;
          }
          else if (game->pause_set[0][1] == 1) {
            game->pause_set[0][1] = 0;
            game->pause_set[0][2] = 1;
          }
          al_play_sample(menu, 0.3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        }
        if (event.keyboard.keycode == ALLEGRO_KEY_ENTER && game->pause) {
          if (game->pause_set[0][0])
            game->pause = false;
          else if (game->pause_set[0][1])
            game->pause_set[1][1] = 1;
          else if (game->pause_set[0][2])
            game->done = true;
          al_play_sample(menu_select, 0.3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        }

        // exit pause menu
        if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE && game->pause && game->pause_set[1][1]) {
          game->pause_set[1][1] = false;
          al_play_sample(menu, 0.3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
          break;
        }

        // user may be cheating
        // code: up, right, down, left, left, right, left, up, left, down
        if (game->cheat_index==10) {
          game->infinite_time = true;
          game->cheat_index = 0;
          al_play_sample(cheat, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        }
        if (event.keyboard.keycode == ALLEGRO_KEY_UP && !game->pause && !game->infinite_time) {
          if (game->cheat_index==0 || game->cheat_index==7)
            game->cheat_index++;
          else game->cheat_index=0;
        }
        if (event.keyboard.keycode == ALLEGRO_KEY_DOWN && !game->pause && !game->infinite_time) {
          if (game->cheat_index==2 || game->cheat_index==9)
            game->cheat_index++;
          else game->cheat_index=0;
        }
        if (event.keyboard.keycode == ALLEGRO_KEY_LEFT && !game->pause && !game->infinite_time) {
          if (game->cheat_index==3 || game->cheat_index==4 || game->cheat_index==6 || game->cheat_index==8)
            game->cheat_index++;
          else game->cheat_index=0;
        }
        if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT && !game->pause && !game->infinite_time) {
          if (game->cheat_index==1 || game->cheat_index==5)
            game->cheat_index++;
          else game->cheat_index=0;
        }

        // if the game continues, update
        if (game->smoke_index == 0 && !game->finish && !game->pause) {
          hero_update(&event, map, hero, boulders, jewels, game);
          boulders_update(map, boulders, game);
          jewels_update(map, jewels, game);
        }
        hud_update(game);

        redraw = true;
        game->frames++;
        if((event.keyboard.keycode != ALLEGRO_KEY_ESCAPE) || game->pause_set[1][1])
          break;

      case ALLEGRO_EVENT_DISPLAY_CLOSE:
        game->done = true;
        break;
    }

    if(game->done)
      break;


    if(redraw && al_is_event_queue_empty(queue))
    {
      disp_pre_draw();
      al_clear_to_color(al_map_rgb(0,0,0));

      draw_map(map, hero, game);

      if (!game->pause && game->time>0)
        hud_draw(game);

      disp_post_draw();
      redraw = false;
    }
  }


  // update user record file
  long sum=0;
  for (int i=game->map-1; i>=0; i--)
    sum = sum + game->score[i];
  if (game->current_score < sum) {
    score_file = fopen ("./resources/record.txt", "w") ;
      fputs("# USER RECORD\n", score_file);
      fprintf (score_file, "%ld", sum);
    fclose (score_file) ;
  }

  free(hero);
  free(boulders);
  free(jewels);
  free(map);
  free(game);

  image_deinit();
  sprites_deinit();
  hud_deinit();
  audio_deinit();
  disp_deinit();
  al_destroy_timer(timer);
  al_destroy_event_queue(queue);

  return 0;
}