#include <allegro5/allegro.h>
#include <stdio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdlib.h>
#include "utilidades.h" 

#define RECORD_FILE "recorde.txt"

int carregar_record(const char *arquivo) {
    FILE *fp = fopen(arquivo, "r");
    if (!fp) {
        return 0;  
    }
    int valor = 0;
    if (fscanf(fp, "%d", &valor) != 1) {
        fclose(fp);
        return 0;
    }
    fclose(fp);
    return valor;
    
}

void salvar_record(const char *arquivo, int record) {
    if (record < 0) {
        return;
    }
    FILE *fp = fopen(arquivo, "w");
    if (!fp) {
        return;
    }
    if (fprintf(fp, "%d", record) < 0) {
    }
    fclose(fp);
}

void draw_pontuacao (int score, int record, ALLEGRO_FONT *font) {
    char buffer[100];
    
    sprintf(buffer, "Pontos: %d", score);
    al_draw_text(font, al_map_rgb(255, 255, 255), 10, 10, 0, buffer);
    
    sprintf(buffer, "Recorde: %d", record);
    al_draw_text(font, al_map_rgb(255, 255, 0), 10, 40, 0, buffer);
}