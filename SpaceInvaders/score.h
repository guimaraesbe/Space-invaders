#ifndef SCORE_H
#define score_h

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "utilidades.h"

int carregar_record(const char *arquivo);
void salvar_record(const char *arquivo, int recorde);
void draw_pontuacao (int score, int record, ALLEGRO_FONT *font);


#endif