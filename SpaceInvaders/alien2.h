#ifndef ALIEN2_H
#define ALIEN2_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "utilidades.h"

typedef struct {
    float x, y;   
    float x_vel, y_vel; 
    ALLEGRO_COLOR cor; 
    int ativo;  
    ALLEGRO_BITMAP *sprite;
    float escala;
} Alien;

void draw_alien_com_bloco(Alien alien, float dx, float dy);
void initAlien(ALLEGRO_BITMAP *PNGalien, const int NUM_LINHAS, const int NUM_COLUNAS, Alien aliens[][NUM_COLUNAS]);
void update_all_aliens(Alien aliens[NUM_LINHAS][NUM_COLUNAS], float *bloco_x, float *bloco_y, int *bloco_direcao);
int verifica_aliens(Alien aliens[NUM_LINHAS][NUM_COLUNAS]);

#endif