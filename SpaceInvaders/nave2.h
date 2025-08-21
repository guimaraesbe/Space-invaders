#ifndef NAVE2_H
#define NAVE2_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "utilidades.h"

typedef struct {
    float x, y;
    float vel;
    int dir, esq;
    ALLEGRO_COLOR cor;
    int score;
    ALLEGRO_BITMAP *sprite;
} Nave;

void draw_nave(Nave *nave);
void initNave(Nave *nave, ALLEGRO_BITMAP *PNGnave);
void update_nave(Nave *nave);

#endif