#include <allegro5/allegro.h>
#include <math.h>
#include "utilidades.h"
#include "colisoes.h"
#include "nave2.h"

int colisao_tiro_alien (float x1, float y1, float w1, float h1,
                       float x2, float y2, float w2, float h2) {
    return (x1 < x2 + w2) && (x1 + w1 > x2) && (y1 < y2 + h2) && (y1 + h1 > y2);
}

int colisao_alien_solo(Alien aliens[NUM_LINHAS][NUM_COLUNAS], float bloco_y){
       float limite_grass = SCREEN_H - GRASS_H;

       for (int linha = 0; linha < NUM_LINHAS; linha++) {
              for (int coluna = 0; coluna < NUM_COLUNAS; coluna++) {
                     if (aliens[linha][coluna].ativo) {
                            float alien_base_y = aliens[linha][coluna].y + bloco_y + ALIEN_H;
                
                            if (alien_base_y >= limite_grass) {
                                   return 1; 
                            }
                     }
              }
       }
       return 0; 
}

int colisao_alien_nave(Alien aliens[NUM_LINHAS][NUM_COLUNAS], float bloco_x, float bloco_y, Nave *nave) {
    float largura_nave = al_get_bitmap_width(nave->sprite) * ESCALA_NAVE;
    float altura_nave = al_get_bitmap_height(nave->sprite) * ESCALA_NAVE;

    float nave_left = nave->x - largura_nave / 2;
    float nave_right = nave->x + largura_nave / 2;
    float nave_top = nave->y - altura_nave / 2;
    float nave_bottom = nave->y + altura_nave / 2;

    for (int linha = 0; linha < NUM_LINHAS; linha++) {
        for (int coluna = 0; coluna < NUM_COLUNAS; coluna++) {
            if (aliens[linha][coluna].ativo) {
                float alien_x = aliens[linha][coluna].x + bloco_x;
                float alien_y = aliens[linha][coluna].y + bloco_y;

                float alien_largura = al_get_bitmap_width(aliens[linha][coluna].sprite) * aliens[linha][coluna].escala;
                float alien_altura = al_get_bitmap_height(aliens[linha][coluna].sprite) * aliens[linha][coluna].escala;

                float alien_left = alien_x;
                float alien_right = alien_x + alien_largura;
                float alien_top = alien_y;
                float alien_bottom = alien_y + alien_altura;

                int colisao_horizontal = (alien_right > nave_left) && (alien_left < nave_right);
                int colisao_vertical = (alien_bottom > nave_top) && (alien_top < nave_bottom);

                if (colisao_horizontal && colisao_vertical) {
                    return 1; // colisão detectada
                }
            }
        }
    }
    return 0; // sem colisão
}

float calcular_distancia(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    return sqrt(dx*dx + dy*dy);
}


