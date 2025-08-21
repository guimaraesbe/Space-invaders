#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "nave2.h"
#include "utilidades.h"


void draw_nave(Nave *nave) {
    float largura = al_get_bitmap_width(nave->sprite) * ESCALA_NAVE;
    float altura = al_get_bitmap_height(nave->sprite) * ESCALA_NAVE;

    al_draw_scaled_bitmap(
        nave->sprite,
        0, 0,
        al_get_bitmap_width(nave->sprite), al_get_bitmap_height(nave->sprite),
        nave->x - largura / 2,  
        nave->y - altura / 2,   
        largura,
        altura,
        0
    );
}


void initNave(Nave *nave, ALLEGRO_BITMAP *PNGnave) {
    nave->x = SCREEN_W / 2; 
    nave->y = SCREEN_H - (al_get_bitmap_height(PNGnave) * ESCALA_NAVE) / 2 - 10;
    nave->vel = 7; 
    nave->dir = 0;
    nave->esq = 0; 
    nave->sprite = PNGnave;
}

void update_nave(Nave *nave) {
    float largura = al_get_bitmap_width(nave->sprite) * ESCALA_NAVE;

    if (nave->dir && (nave->x + largura / 2 + nave->vel <= SCREEN_W)) {
        nave->x += nave->vel;
    }
    if (nave->esq && (nave->x - largura / 2 - nave->vel >= 0)) {
        nave->x -= nave->vel;
    }
}
