CC = gcc
CFLAGS = -Wall -g `pkg-config --cflags allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5 allegro_ttf-5 allegro_main-5`
LDFLAGS = `pkg-config --libs allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5 allegro_ttf-5 allegro_main-5`

all: invaders

invaders: invaders.c
	$(CC) $(CFLAGS) -o invaders invaders.c $(LDFLAGS)

clean:
	rm -f invaders