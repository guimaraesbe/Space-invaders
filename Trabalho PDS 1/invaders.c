#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


const float FPS = 100;  
const int SCREEN_W = 1500;
const int SCREEN_H = 790;

const int GRASS_H = 100;

const int NAVE_W = 130;
const int NAVE_H = 90;

const int ALIEN_W = 50;
const int ALIEN_H = 25;

const int NUM_LINHAS = 4;
const int NUM_COLUNAS = 5;
const int ALIENS_SPACE = 30;
const int ALIENS_TOTAL = NUM_LINHAS * NUM_COLUNAS;

const int NUM_TIROS = 1;
const float TIRO_VEL_Y = 5.0;
const float TIRO_RAIO = 5.0;

const float TIRO_POS_INICIAL_Y = SCREEN_H - GRASS_H / 2 - NAVE_H - TIRO_RAIO;
const int TIRO_INTERVALO_FPS = (int)(TIRO_POS_INICIAL_Y / TIRO_VEL_Y);

const char* RECORD_FILE = "recorde.txt";

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
    int ativo;
} Alien;

typedef struct Tiro {
    float x, y;
    float vel_y;
    float raio;
    int ativo;
    ALLEGRO_COLOR cor;
} Tiro;

void initNave(Nave *nave) {
	nave->x =  SCREEN_W / 2;
	nave->vel = 5;
	nave->dir = 0;
	nave->esq = 0;
	nave-> cor = al_map_rgb(0, 0, 255);
}

void initAlien (Alien *alien, int col, int linha) {
    alien->x = 100 + col * (ALIEN_W + ALIENS_SPACE);
    alien->y = linha * (ALIEN_H + ALIENS_SPACE);
    alien->x_vel = 5;
    alien->y_vel = ALIEN_H;
    alien->cor = al_map_rgb(rand() % 256, rand() % 256, rand() % 256);
    alien->ativo = 1;
}

void draw_alien (Alien alien) {

     if (alien.ativo) {
        al_draw_filled_rectangle(alien.x, alien.y, alien.x + ALIEN_W, alien.y + ALIEN_H, alien.cor);
     }
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

int colisao_alien_nave (Alien alien, Nave nave) {
   // dimensoes nave
    float nave_topo_y = SCREEN_H - GRASS_H / 2 - NAVE_H;
    float nave_base_y = SCREEN_H - GRASS_H /2;
    float nave_esq_x = nave.x - NAVE_W / 2;
    float nave_dir_x = nave.x + NAVE_W / 2;
    // dimensoes alien
    float alien_esq_x = alien.x;
    float alien_dir_x = alien.x + ALIEN_W;
    float alien_topo_y = alien.y;
    float alien_base_y = alien.y + ALIEN_H;

    int sobrepoe_x = (alien_esq_x < nave_dir_x && alien_dir_x > nave_esq_x);

    int sobrepoe_y = (alien_topo_y < nave_base_y && alien_base_y > nave_topo_y);

    return sobrepoe_x && sobrepoe_y;
}

int colisao_tiro_alien (Tiro tiro, Alien alien) {
    if (!tiro.ativo || !alien.ativo) {
        return 0;
    }
    //centros do tiro
    float circ_x = tiro.x;
    float circ_y = tiro.y;
    float circ_raio = tiro.raio;
    //dimensoes dos aliens
    float alien_x1 = alien.x;
    float alien_y1 = alien.y;
    float alien_x2 = alien.x + ALIEN_W;
    float alien_y2 = alien.y + ALIEN_H;
    //ponto do alien mais proximo do centro do circulo
    float test_x = circ_x;
    float test_y = circ_y;

    if (circ_x < alien_x1) {
        test_x = alien_x1;
    } else if (circ_x > alien_x2) {
        test_x = alien_x2;
    }
    if (circ_y < alien_y1) {
        test_y = alien_y1;
    } else if (circ_y > alien_y2) {
        test_y = alien_y2;
    }

        
    float dist_x = circ_x - test_x;
    float dist_y = circ_y - test_y;
    float distancia_ao_quadrado = (dist_x * dist_x) + (dist_y * dist_y);

	float raio_ao_quadrado = circ_raio * circ_raio;

    return (distancia_ao_quadrado <= raio_ao_quadrado);     //distancia menor ou igual ao raio  tem colisao
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
        if(aliens[i].ativo && (aliens[i].x + alien_w >= screen_w || aliens[i].x  <= 0)){
            deve_descer = 1;
            break;
        }
    }
    for (int i = 0; i < total_aliens; i++) {
        if (aliens[i].ativo) {
        if (deve_descer) {
            aliens[i].y += aliens[i].y_vel; // Faz o alien descer
            aliens[i].x_vel *= -1;         // Inverte a direção horizontal
        }
        update_alien(&aliens[i]);
        }
    }
}

void initTiros (Tiro tiros[]) {
    for (int i = 0; i < NUM_TIROS; i++) {
        tiros[i].ativo = 0;
    }
}
void draw_tiros (Tiro tiros []) {
    for (int i = 0; i < NUM_TIROS; i++) {
        if (tiros[i].ativo) {
            al_draw_filled_circle(tiros[i].x, tiros[i].y, tiros[i].raio, tiros[i].cor);
        }
    }
}

void dispara_tiro (Tiro tiros[], Nave nave) {
    for (int i = 0; i < NUM_TIROS; i++) {
        if (!tiros[i].ativo) {
            tiros[i].x = nave.x;
            tiros[i].y = SCREEN_H - GRASS_H / 2 - NAVE_H - TIRO_RAIO; // Posiciona acima da nave
            tiros[i].vel_y = -TIRO_VEL_Y; // Negativo para subir
            tiros[i].raio = TIRO_RAIO;
            tiros[i].cor = al_map_rgb(255, 255, 0); 
            tiros[i].ativo = 1;
            break; // 
        }
     }
}

void update_tiros (Tiro tiros[]) {
    for (int i = 0; i < NUM_TIROS; i++) {
        if (tiros[i].ativo) {
            tiros[i].y += tiros[i].vel_y;
            if (tiros[i].y < 0) {
               tiros[i].ativo = 0;
            }
        }
    }
}

int load_record () {
    FILE *file = fopen (RECORD_FILE, "r");
    int record = 0;

    if (file != NULL) {
        if (fscanf(file, "%d", &record) != 1) {
            record = 0;
        }
        fclose(file);
    }
    return record;
}

void save_record (int newRecord) {
    FILE *file = fopen(RECORD_FILE, "w");
    if (file != NULL) {
        fprintf(file,"%d", newRecord);
        fclose(file);
    } else {
        fprintf(stderr, "Erro ao salvar o recorde no arquivo %s \n", RECORD_FILE);
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
    ALLEGRO_FONT *size_32 = al_load_font("fontes/arial.ttf", 32, 1);   
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

    Tiro tiros[NUM_TIROS];
    initTiros(tiros);


	//inicia o temporizador
	al_start_timer(timer);
	
	int playing = 1;
    int contador_tiro = 0;
    int score = 0;
    int highScore = load_record();
    
	while(playing) {
		ALLEGRO_EVENT ev;
		//espera por um evento e o armazena na variavel de evento ev
		al_wait_for_event(event_queue, &ev);
		draw_scenario();


		//se o tipo de evento for um evento do temporizador, ou seja, se o tempo passou de t para t+1
		if(ev.type == ALLEGRO_EVENT_TIMER) {

            draw_scenario();
            update_nave(&nave);
            draw_nave(nave);
            draw_tiros(tiros);
            update_all_aliens(aliens, ALIENS_TOTAL, SCREEN_W, ALIEN_W);
            update_tiros(tiros);
            al_draw_textf(size_32, al_map_rgb(255, 255, 255), 10, 10, 0, "SCORE: %d", score);
            al_draw_textf(size_32, al_map_rgb(0, 255, 0), SCREEN_W - 10, 10, ALLEGRO_ALIGN_RIGHT, "RECORD: %d", highScore);

            contador_tiro++;
            if (contador_tiro >= TIRO_INTERVALO_FPS) {
                dispara_tiro(tiros, nave);
                contador_tiro = 0;
            }

            for (int i = 0; i < ALIENS_TOTAL; i++) {    //desenha todos os aliens
               draw_alien(aliens[i]);
            }


            for (int i = 0; i < NUM_TIROS; i++) {
                if(tiros[i].ativo) {
                    for (int j = 0; j < ALIENS_TOTAL; j++) {
                        if (aliens[j].ativo) {
                            if (colisao_tiro_alien(tiros[i], aliens[j])) {
                                aliens[j].ativo = 0;
                                tiros[i].ativo = 0;
                                score += 25;
                                break;
                            }
                        }
                    }
                }
            }
            
            al_flip_display();
			

            for (int i = 0; i < ALIENS_TOTAL; i++) {
                if (colisao_alien_solo(aliens[i])) {
                    playing = 0;
                    break;
                    printf("Alien tocou o solo! Game Over.\n");
                }
                if (colisao_alien_nave(aliens[i], nave)) {
                    playing = 0;
                    break;
                     printf("Alien colidiu com a nave! Game Over.\n");
            }
        }

			//atualiza a tela (quando houver algo para mostrar)
			
			if(al_get_timer_count(timer)%(int)FPS == 0) {
				printf("\n%d segundos se passaram...", (int)(al_get_timer_count(timer)/FPS));
		
		//se o tipo de evento for o fechamento da tela (clique no x da janela)
	    } 
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
			printf("\ncodigo tecla: %d\n", ev.keyboard.keycode);

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
    }
    printf("\nFim de jogo! Seu score final: %d\n", score);
    if (score > highScore) {
        highScore = score; // Atualiza o recorde
        save_record(highScore); // Salva o novo recorde no arquivo
        printf("NOVO RECORDE: %d!\n", highScore);
    } else {
        printf("Recorde atual: %d\n", highScore);
    }
    
	//procedimentos de fim de jogo (fecha a tela, limpa a memoria, etc)

 
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
   
 
	return 0;
}
