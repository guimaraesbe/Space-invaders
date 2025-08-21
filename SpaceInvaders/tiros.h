#ifndef TIROS_H
#define TIROS_H

#include <allegro5/allegro.h>
#include "utilidades.h"
#include "alien2.h"

typedef struct {
    float x, y;  
    int ativo;  
    float vel_y;  
} Tiro;

void init_tiros();
int update_tiros(float nave_x, float nave_y, Alien aliens[NUM_LINHAS][NUM_COLUNAS], float bloco_x, float bloco_y);
void draw_tiros();
void cria_tiro(float nave_x, float nave_y);

#endif