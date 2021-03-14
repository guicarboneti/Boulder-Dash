# Made by Guilherme Carbonari Boneti, GRR20196478, 02/2021.

CFLAGS = -Wall -std=c99 # compilation flag
LDFLAGS = -std=c99 `pkg-config allegro-5 allegro_font-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 allegro_image-5 --libs --cflags`

# object-file
objects = game.o

boulder_dash: boulder_dash.c $(objects)
	gcc boulder_dash.c -o boulder_dash $(objects) $(CFLAGS) $(LDFLAGS)
game.o: game.c game.h
	gcc -c game.c $(CFLAGS) $(LDFLAGS)
purge: clean
	-rm -f boulder_dash
clean:
	-rm -f $(objects)
