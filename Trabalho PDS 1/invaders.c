#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdlib.h>
#include <time.h>

const float FPS = 100;  
const int SCREEN_W = 960;
const int SCREEN_H = 540;

const int GRASS_H = 60;

const int NAVE_W = 100;
const int NAVE_H = 50;

const int ALIEN_W = 50;
const int ALIEN_H = 25;

const int NUM_LINHAS = 4;
const int NUM_COLUNAS = 5;
const int ALIENS_SPACE = 30;
const int ALIENS_TOTAL = NUM_LINHAS * NUM_COLUNAS;

typedef struct Nave {
     float x;
	 float vel;
	 int dir, esq;
	 ALLEGRO_COLOR cor;
} Nave;

typedef struct Alien {
    float x, y;
    float x_vel, y_vel;
    ALLEGRO_COLOR cor;
} Alien;

void initNave(Nave *nave) {
	nave->x =  SCREEN_W / 2;
	nave->vel = 1;
	nave->dir = 0;
	nave->esq = 0;
	nave-> cor = al_map_rgb(0, 0, 255);
}

void initAlien (Alien *alien, int col, int linha) {
    alien->x = col * (ALIEN_W + ALIENS_SPACE);
    alien->y = linha * (ALIEN_H + ALIENS_SPACE);
    alien->x_vel = 1;
    alien->y_vel = ALIEN_H;
    alien->cor = al_map_rgb(rand() % 256, rand() % 256, rand() % 256);
}

void draw_alien (Alien alien) {

        al_draw_filled_rectangle(alien.x, alien.y, alien.x + ALIEN_W, alien.y + ALIEN_H, alien.cor);
}

void update_alien (Alien *alien) {

       alien->x += alien->x_vel;
}

int colisao_alien_solo (Alien alien) {

    if (alien.y + ALIEN_H > SCREEN_H - GRASS_H) {
        return 1;
    } else {
        return 0;
    }
}

void draw_scenario() {
	
	al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_filled_rectangle(0, SCREEN_H - GRASS_H, SCREEN_W, SCREEN_H, al_map_rgb(0, 245, 0));
}

void draw_nave (Nave nave) {

	float y_base = SCREEN_H - GRASS_H / 2;
	al_draw_filled_triangle(nave.x, y_base - NAVE_H, nave.x - NAVE_W / 2, y_base, nave.x + NAVE_W / 2, y_base, nave.cor);
}

void update_nave (Nave *nave) {
    if (nave->dir && (nave->x + NAVE_W / 2 + nave->vel) <= SCREEN_W) {
       nave->x += nave->vel;
	}
	if (nave->esq && (nave->x - NAVE_W / 2 - nave->vel) >= 0) {
		nave->x -= nave->vel;
	}
}

void update_all_aliens (Alien aliens [], int total_aliens, int screen_w, int alien_w) {
    int deve_descer = 0;
    for (int i = 0; i < total_aliens; i++) {
        if ((aliens[i].x + alien_w + aliens[i].x_vel > screen_w && aliens[i].x_vel > 0) || (aliens[i].x + aliens[i].x_vel < 0 && aliens[i].x_vel < 0)) {
            deve_descer = 1;
            break;
        }
    }
    for (int i = 0; i < total_aliens; i++) {
        if (deve_descer) {
            aliens[i].y += aliens[i].y_vel; // Faz o alien descer
            aliens[i].x_vel *= -1;         // Inverte a direção horizontal
        }
        update_alien(&aliens[i]);
    }
}
 
int main(int argc, char **argv){
	
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
    ALLEGRO_FONT *size_32 = al_load_font("arial.ttf", 32, 1);   
	if(size_32 == NULL) {
		fprintf(stderr, "font file does not exist or cannot be accessed!\n");
	}


 	//cria a fila de eventos
	event_queue = al_create_event_queue();
	if(!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		return -1;
	}
   




	//registra na fila os eventos de tela (ex: clicar no X na janela)
	al_register_event_source(event_queue, al_get_display_event_source(display));
	//registra na fila os eventos de tempo: quando o tempo altera de t para t+1
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	//registra na fila os eventos de teclado (ex: pressionar uma tecla)
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	//registra na fila os eventos de mouse (ex: clicar em um botao do mouse)
	al_register_event_source(event_queue, al_get_mouse_event_source());  	


	Nave nave;
	initNave(&nave);

    srand(time(NULL));
    Alien aliens[ALIENS_TOTAL];
       for (int l = 0; l < NUM_LINHAS; l++) {
           for (int c = 0; c < NUM_COLUNAS; c++) {
               initAlien(&aliens[l * NUM_COLUNAS + c], c, l);
           }
       }


	//inicia o temporizador
	al_start_timer(timer);
	
	int playing = 1;
	while(playing) {
		ALLEGRO_EVENT ev;
		//espera por um evento e o armazena na variavel de evento ev
		al_wait_for_event(event_queue, &ev);


		//se o tipo de evento for um evento do temporizador, ou seja, se o tempo passou de t para t+1
		if(ev.type == ALLEGRO_EVENT_TIMER) {


			draw_scenario();

			update_nave(&nave);

            update_all_aliens(aliens, ALIENS_TOTAL, SCREEN_W, ALIEN_W);

            for (int i = 0; i < ALIENS_TOTAL; i++) {    //desenha todos os aliens
               draw_alien(aliens[i]);
            }
            
			draw_nave(nave);

            for (int i = 0; i < ALIENS_TOTAL; i++) {
                if (colisao_alien_solo(aliens[i])) {
                    playing = 0;
                    break;
                }
            }


			//atualiza a tela (quando houver algo para mostrar)
			al_flip_display();
			
			if(al_get_timer_count(timer)%(int)FPS == 0)
				printf("\n%d segundos se passaram...", (int)(al_get_timer_count(timer)/FPS));
		}
		//se o tipo de evento for o fechamento da tela (clique no x da janela)
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			playing = 0;
		}
		
		//se o tipo de evento for um pressionar de uma tecla
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			//imprime qual tecla foi
			printf("\ncodigo tecla: %d", ev.keyboard.keycode);

			switch(ev.keyboard.keycode) {
				
				case ALLEGRO_KEY_A:
				    nave.esq = 1;
					break;

					case ALLEGRO_KEY_D:
				    nave.dir = 1;
					break;
			}
		}

		//se o tipo de evento for um pressionar de uma tecla
		else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
			//imprime qual tecla foi
			printf("\ncodigo tecla: %d", ev.keyboard.keycode);

			switch(ev.keyboard.keycode) {
				
				case ALLEGRO_KEY_A:
				    nave.esq = 0;
					break;

					case ALLEGRO_KEY_D:
				    nave.dir = 0;
					break;
			}
		
		}

	} //fim do while
     
	//procedimentos de fim de jogo (fecha a tela, limpa a memoria, etc)
	
 
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
   
 
	return 0;
}
