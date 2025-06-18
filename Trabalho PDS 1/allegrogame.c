#include <stdio.h>
#include <allegro5/allegro.h>

const int SCREEN_W = 960;
const int SCREEN_H = 540;

const int GRASS_H = 60;

const float FPS = 1;

int main () {

    ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;

    	//----------------------- rotinas de inicializacao ---------------------------------------
    
	//inicializa o Allegro
      if(!al_init_primitives_addon()){
		fprintf(stderr, "failed to initialize primitives!\n");
        return -1;
    }

    //cria uma tela com as dimensoes SCREEN_W, SCREEN_H pixels
    display = al_create_display(SCREEN_W, SCREEN_H);
    if(!display) {
        fprintf(stderr, "failed to create display!\n");
        return -1;
    }
}