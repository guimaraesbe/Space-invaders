CC = gcc
CFLAGS = -Wall -g `pkg-config --cflags allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5 allegro_ttf-5 allegro_main-5`
LDFLAGS = `pkg-config --libs allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5 allegro_ttf-5 allegro_main-5` -lm

EXEC = invaders
SRC = invaders2.c alien2.c nave2.c utilidades.c tiros.c colisoes.c score.c

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $(EXEC) $(SRC) $(LDFLAGS)

clean:
	rm -f $(EXEC)