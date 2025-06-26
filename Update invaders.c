#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "utilidades.h"
#include "nave2.h"
#include "alien2.h"
#include "tiros.h"
#include "colisoes.h"
 

const float FPS = 60;  
 
int main(int argc, char **argv) {
	
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
   
	//----------------------- rotinas de inicializacao ---------------------------------------
    
	//inicializa o Allegro
	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}
	
    //inicializa o módulo de primitivas do Allegro
    if(!al_init_primitives_addon()){
		fprintf(stderr, "failed to initialize primitives!\n");
        return -1;
    }	
	
	//inicializa o modulo que permite carregar imagens no jogo
	if(!al_init_image_addon()){
		fprintf(stderr, "failed to initialize image module!\n");
		return -1;
	}
   
	//cria um temporizador que incrementa uma unidade a cada 1.0/FPS segundos
    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}
 
	//cria uma tela com dimensoes de SCREEN_W, SCREEN_H pixels
	display = al_create_display(SCREEN_W, SCREEN_H);
	if(!display) {
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}

	//instala o teclado
	if(!al_install_keyboard()) {
		fprintf(stderr, "failed to install keyboard!\n");
		return -1;
	}
	
	//instala o mouse
	if(!al_install_mouse()) {
		fprintf(stderr, "failed to initialize mouse!\n");
		return -1;
	}

	//inicializa o modulo allegro que carrega as fontes
	al_init_font_addon();

	//inicializa o modulo allegro que entende arquivos tff de fontes
	if(!al_init_ttf_addon()) {
		fprintf(stderr, "failed to load tff font module!\n");
		return -1;
	}
	
	//carrega o arquivo arial.ttf da fonte Arial e define que sera usado o tamanho 32 (segundo parametro)
    ALLEGRO_FONT *texto = al_load_font("fontes/arial.ttf", 18, 0); 
	ALLEGRO_FONT *font = al_load_font("fontes/arial.ttf", 36, 0);
	
	event_queue = al_create_event_queue();

	//registra na fila os eventos de tela (ex: clicar no X na janela)
	al_register_event_source(event_queue, al_get_display_event_source(display));
	//registra na fila os eventos de tempo: quando o tempo altera de t para t+1
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	//registra na fila os eventos de teclado (ex: pressionar uma tecla)
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	//registra na fila os eventos de mouse (ex: clicar em um botao do mouse)
	al_register_event_source(event_queue, al_get_mouse_event_source());  
    
    srand(time(NULL));
    
    float bloco_x = 0;
    float bloco_y = 0;
    int bloco_direcao = 1;
	int game_over = 0;
	int win = 0;

    Alien aliens[NUM_LINHAS][NUM_COLUNAS];
    for (int linha = 0; linha < NUM_LINHAS; linha++) {
        for (int coluna = 0; coluna < NUM_COLUNAS; coluna++) {
            initAlien(&aliens[linha][coluna], linha, coluna);
       }
    } 

    Nave nave;
    initNave(&nave);

    init_tiros();
	al_start_timer(timer);
	
	int playing = 1;
	while(playing) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);	
 
		//se o tipo de evento for um evento do temporizador, ou seja, se o tempo passou de t para t+1
		if(ev.type == ALLEGRO_EVENT_TIMER) {
            if(!game_over) {
				update_nave(&nave);
            	update_all_aliens(aliens, &bloco_x, &bloco_y, &bloco_direcao);
            	update_tiros(nave.x, nave.y, aliens, bloco_x, bloco_y);  

				if (colisao_alien_solo(aliens, bloco_y)) {
            		game_over = 1;
            		printf("Game Over! Aliens atingiram a grama!\n");
        		}

				if (colisao_alien_nave(aliens, bloco_x, bloco_y, &nave)) {
            		game_over = 1;
        		}
            
				al_clear_to_color(al_map_rgb(0, 0, 0)); 
            	draw_cenario();
            	draw_nave(nave);
            	for (int linha = 0; linha < NUM_LINHAS; linha++) {
                	for (int coluna = 0; coluna < NUM_COLUNAS; coluna++) {
                    	draw_alien_com_bloco(aliens[linha][coluna], bloco_x, bloco_y);
                	}
            	} 	
			}
            draw_tiros();
			al_flip_display();

			if(verifica_aliens(aliens)){
            	game_over = 1;
            	win = 1; 

				al_clear_to_color(al_map_rgb(0, 0, 0));
    			al_draw_text(font, al_map_rgb(0, 255, 0), SCREEN_W/2, SCREEN_H/2 - 30, 
                ALLEGRO_ALIGN_CENTER, "VITORIA! Parabens!");
   	 			al_draw_text(texto, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2 + 40, 
                ALLEGRO_ALIGN_CENTER, "Pressione R para reiniciar");
    			al_flip_display();
        	}

			if(game_over) {
            	if(font){
					if(win) {
						al_clear_to_color(al_map_rgb(0, 0, 0));
						al_draw_text(font, al_map_rgb(0, 255, 0), SCREEN_W/2, SCREEN_H/2 - 30, 
                        ALLEGRO_ALIGN_CENTER, "VITORIA! Parabens!");
						al_draw_text(texto, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2 + 40, 
                    	ALLEGRO_ALIGN_CENTER, "Pressione R para reiniciar");
					} else {	
						al_clear_to_color(al_map_rgb(0, 0, 0));
                		al_draw_text(font, al_map_rgb(255, 0, 0), SCREEN_W/2, SCREEN_H/2 - 30, 
                    	ALLEGRO_ALIGN_CENTER, "GAME OVER!!!");
						al_draw_text(texto, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2 + 40, 
                    	ALLEGRO_ALIGN_CENTER, "Pressione R para reiniciar");
            		}
        		}
				al_flip_display();
			}
		}
		//se o tipo de evento for o fechamento da tela (clique no x da janela)
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
         	playing = 0;
		}
		
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
        	switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_A:
				nave.esq = 1;
				update_nave(&nave);
				break;

			case ALLEGRO_KEY_D:
				nave.dir = 1;
				update_nave(&nave);
				break;

			case ALLEGRO_KEY_SPACE:
            	if (!game_over) {
            		cria_tiro(nave.x, nave.y);
            	}
            	break;

			case ALLEGRO_KEY_R:
            	if (game_over) {
            		game_over = 0;
                	// Reinicializa os aliens
                	for (int linha = 0; linha < NUM_LINHAS; linha++) {
                    	for (int coluna = 0; coluna < NUM_COLUNAS; coluna++) {
                    		initAlien(&aliens[linha][coluna], linha, coluna);
                    	}
               		}
                	//Reinicializa a nave
                	initNave(&nave);
                	// Reinicializa a posição do bloco
                	bloco_x = 0;
                	bloco_y = 0;
               		bloco_direcao = 1;
                	// Limpa os tiros
                	init_tiros();
            	}
            	break;
			}
		}

		else if(ev.type == ALLEGRO_EVENT_KEY_UP){
        	switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_A:
				nave.esq = 0;
				break;

			case ALLEGRO_KEY_D:
				nave.dir = 0;
				break;
			}
		}
	}
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_font(font);
	al_destroy_event_queue(event_queue);

	return 0;
}
