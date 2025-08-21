#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdlib.h>
#include "utilidades.h"
#include "alien2.h"

void draw_alien_com_bloco(Alien alien, float dx, float dy) {
    if (!alien.ativo) return;

    float largura = al_get_bitmap_width(alien.sprite) * alien.escala;
    float altura = al_get_bitmap_height(alien.sprite) * alien.escala;

    al_draw_scaled_bitmap(
        alien.sprite,
        0, 0, al_get_bitmap_width(alien.sprite), al_get_bitmap_height(alien.sprite),
        alien.x + dx, alien.y + dy,
        largura, altura,
        0
    );
}

void initAlien(ALLEGRO_BITMAP *PNGalien, const int NUM_LINHAS, const int NUM_COLUNAS, Alien aliens[][NUM_COLUNAS]) {
    float sprite_width = al_get_bitmap_width(PNGalien);
    float sprite_height = al_get_bitmap_height(PNGalien);
    
    float escala = (SCREEN_W * ALIEN_ESCALA_BASE) / sprite_width;
    float largura_escala = sprite_width * escala;
    float altura_escala = sprite_height * escala;

    float espacamento_x = largura_escala * 1.5;
    float espacamento_y = altura_escala * 1.5;

    for (int linha = 0; linha < NUM_LINHAS; linha++) {
        for (int coluna = 0; coluna < NUM_COLUNAS; coluna++) {
            aliens[linha][coluna].x = coluna * espacamento_x;
            aliens[linha][coluna].y = linha * espacamento_y;
            aliens[linha][coluna].ativo = 1;
            aliens[linha][coluna].escala = escala * 0.8;
            aliens[linha][coluna].sprite = PNGalien;
        }
    }
}

void update_all_aliens(Alien aliens[NUM_LINHAS][NUM_COLUNAS], float *bloco_x, float *bloco_y, int *bloco_direcao) {
    int deve_descer = 0;
    float largura_alien = 0;
    float altura_alien = 0;

    for (int linha = 0; linha < NUM_LINHAS; linha++) {
        for (int coluna = 0; coluna < NUM_COLUNAS; coluna++) {
            if (aliens[linha][coluna].ativo) {
                largura_alien = al_get_bitmap_width(aliens[linha][coluna].sprite) * aliens[linha][coluna].escala;
                altura_alien = al_get_bitmap_height(aliens[linha][coluna].sprite) * aliens[linha][coluna].escala;
                linha = NUM_LINHAS;  
                break;
            }
        }
    }

    for (int linha = 0; linha < NUM_LINHAS; linha++) {
        for (int coluna = 0; coluna < NUM_COLUNAS; coluna++) {
            if (aliens[linha][coluna].ativo) {
                float alien_x = *bloco_x + aliens[linha][coluna].x;

                if ((*bloco_direcao == 1 && (alien_x + largura_alien >= SCREEN_W)) ||
                    (*bloco_direcao == -1 && (alien_x <= 0))) {
                    deve_descer = 1;
                    break;
                }
            }
        }
        if (deve_descer) break;
    }

    if (deve_descer) {
        *bloco_y += altura_alien;
        *bloco_direcao *= -1;
    } else {
        *bloco_x += (*bloco_direcao) * ALIEN_VEL;
    }
}

int verifica_aliens(Alien aliens[NUM_LINHAS][NUM_COLUNAS]) {
    for(int i = 0; i < NUM_LINHAS; i++) {
        for(int j = 0; j < NUM_COLUNAS; j++) {
            if(aliens[i][j].ativo) {
                return 0;
            }
        }
    }
    return 1;
}
