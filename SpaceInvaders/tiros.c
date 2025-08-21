#include <allegro5/allegro.h>
#include <math.h>
#include "utilidades.h"
#include "tiros.h"
#include "alien2.h"
#include "colisoes.h"

Tiro tiros[MAX_TIRO];
int pode_atirar = 1;

void init_tiros() {
    for (int i = 0; i < MAX_TIRO; i++) {
        tiros[i].ativo = 0;
    }
    pode_atirar = 1;
}

void cria_tiro(float nave_x, float nave_y) {
    if (!pode_atirar) return;   
    
    for (int i = 0; i < MAX_TIRO; i++) {
        if (!tiros[i].ativo) {
            tiros[i].x = nave_x - (TIRO_W / 2);  
            tiros[i].y = nave_y - NAVE_H;
            tiros[i].vel_y = -20;  
            tiros[i].ativo = 1;
            pode_atirar = 0;  
            break;
        }
    }
}

int update_tiros(float nave_x, float nave_y, Alien aliens[NUM_LINHAS][NUM_COLUNAS], float bloco_x, float bloco_y) {
    int tiro_ativo = 0;
    int pontos = 0;
    
    for (int i = 0; i < MAX_TIRO; i++) {
        if (tiros[i].ativo) {
            tiro_ativo = 1;
            tiros[i].y += tiros[i].vel_y;
            
            if (tiros[i].y < 0) {
                tiros[i].ativo = 0;
                pode_atirar = 1;
            } else {
                for (int linha = 0; linha < NUM_LINHAS; linha++) {
                    for (int col = 0; col < NUM_COLUNAS; col++) {
                        Alien *alien = &aliens[linha][col];
                        if (alien->ativo && colisao_tiro_alien(tiros[i].x, tiros[i].y, TIRO_W, TIRO_H, alien->x + bloco_x, alien->y + bloco_y, ALIEN_W, ALIEN_H)) {
                            float dx = (alien->x + bloco_x) - nave_x;
                            float dy = (alien->y + bloco_y) - nave_y;
                            float distancia = sqrt(dx*dx + dy*dy);
                            
                            int pontos_alien = 50 + (int)(distancia / 10);
                            
                            alien->ativo = 0;
                            tiros[i].ativo = 0;
                            pontos += pontos_alien;
                            pode_atirar = 1;
                            break;
                        }
                    }
                    if (!tiros[i].ativo) break;
                }
            }
        }
    }
    
    if (!tiro_ativo) {
        pode_atirar = 1;
    }
    
    return pontos;
}


void draw_tiros() {
    for (int i = 0; i < MAX_TIRO; i++) {
        if (tiros[i].ativo) {
            float raio = TIRO_W / 2.0;  
            al_draw_filled_circle(tiros[i].x + raio, tiros[i].y + raio, raio, al_map_rgb(255, 255, 0));
        }
    }
}