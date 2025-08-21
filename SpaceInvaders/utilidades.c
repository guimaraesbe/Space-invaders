#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include "utilidades.h"

void draw_cenario() {
    al_clear_to_color(al_map_rgb(0, 0, 0)); 
    al_draw_filled_rectangle(0, SCREEN_H - GRASS_H, SCREEN_W, SCREEN_H, al_map_rgb(128, 128, 128)); 
}