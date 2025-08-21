#ifndef COLISOES_H
#define COLISOES_H

#include <allegro5/allegro.h>
#include "alien2.h"
#include "utilidades.h"
#include "nave2.h"

int colisao_tiro_alien(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2);
int colisao_alien_solo(Alien aliens[NUM_LINHAS][NUM_COLUNAS], float bloco_y);
int colisao_alien_nave(Alien aliens[NUM_LINHAS][NUM_COLUNAS], float bloco_x, float bloco_y, Nave *nave);
float calcular_distancia(float x1, float y1, float x2, float y2);

#endif