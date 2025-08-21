    #ifndef UTILIDADES_H
    #define UTILIDADES_H

    #include <allegro5/allegro.h>
    #include <allegro5/allegro_primitives.h>

    #define SCREEN_W 1536  
    #define SCREEN_H 864 
    #define GRASS_H 100  

    #define NAVE_W  108  
    #define NAVE_H  80 

    #define ALIEN_W 35  
    #define ALIEN_H 25 

    static const int NUM_LINHAS = 3; 
    static const int NUM_COLUNAS = 3; 
    static const float FPS = 60;  

    #define ALIENS_TOTAL (NUM_LINHAS * NUM_COLUNAS)  
    #define START_X 20  
    #define START_Y 20  
    #define ALIENS_SPACE 25  
    #define ALIEN_VEL 3  
    #define ALIEN_ESCALA_BASE 0.05f

    #define MAX_TIRO 1
    #define TIRO_W 10
    #define TIRO_H 5

    #define ESCALA_NAVE 0.33


    void draw_cenario();

    #endif