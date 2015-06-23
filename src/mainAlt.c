/* ======================================= DER DURCHFALL ========================================== //
| INSTITUTO FEDERAL DE SANTA CATARINA - IFSC			FEDERAL INSTITUTE OF SANTA CATARINA			||
| DISCIPLINA DE PROGRAMACAO C							C PROGRAMMING LANGUAGE						||
| PROFESSOR FERNANDO PACHECO							PROFESSOR FERNANDO PACHECO					||
| CAIO PRUJANSKY E JOHANN SCHMITDINGER					CAIO PRUJANSKY AND JOHANN SCHMITDINGER		||
| ARQUIVO: main.c										FILE: main.c								||
| VERSAO: v1.0											VERSION: v1.0								||
| DATA DE ALTERACAO: 02/06/2015							LAST ALTERED DATE: 02/06/2015				||
// ================================================================================================00 */

// LIBRARY INCLUSIONS / INCLUSAO DE BIBLIOTECAS =================================================== //
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "durchfallDefines.h"
#include "durchfallStructs.h"
#include "durchfallEngine.h"

int main(void)
{
	// LOCAL VARIABLES DECLARATIONS / DECLARACAO DE VARIAVEIS LOCAIS ============================== //
	// Reading config values from file / Ler valores de configuracao do arquivo ------------------- //
	graphicSettings settings;
	if(!loadSettings(&settings))
		return AL_FILE_ERROR;

	cursor mouse;
    char key[TOTAL_KEY] = {false};
	int i, j;
    int send_enemy = 0;
	int number_enemy = 0;

	gameStatus game;
	game.quit = false;
	game.stage = STAGE_INTRO_SCREEN;

	timeControl menuControl;
	menuControl.flag = false;
	menuControl.time = 0;

	gameMap map;
	initMap(&map, "maps/map01.txt");

	avatar player;
	initPlayer(&player, TYPE_NORMAL, map.width, map.height);

	for(i = 0; i < map.totalEnemies; i++)
	{
        initEnemy(&map.enemies[i], &map, TYPE_NORMAL,map.width,map.height);
	}

	mapView view;
	view.totalWidth = map.width;
	view.totalHeight = map.height;
	view.rangeX = settings.displayX;
	view.rangeY = settings.displayY;
	view.coordX = (view.totalWidth/2) - (view.rangeX/2);
	view.coordY = (view.totalHeight/2) - (view.rangeY/2);

	square limits;
	limits.coordX1 = 0;
	limits.coordY1 = 0;
	limits.coordX2 = map.width;
	limits.coordY2 = map.height;
	limits.flagA = 1;
	limits.flagB = 1;
	limits.flagC = 1;
	limits.flagD = 1;

	// INITIALIZING EVERYTHING / INICIALISANDO TUDO =============================================== //
	// Initializing allegro / Inicializando allegro ----------------------------------------------- //
    if (!al_init())
        return AL_INIT_ERROR;

    // Initializing display / Inicializando display ----------------------------------------------- //
    ALLEGRO_DISPLAY *display = NULL;
    display = al_create_display(settings.displayX, settings.displayY);
    if(!display)
		return AL_DISPLAY_ERROR;

    // Initializing timer / Inicializando timer --------------------------------------------------- //
    ALLEGRO_TIMER *timer = NULL;
    timer = al_create_timer(1.0 / GAME_SPEED);
    if(!timer)
        return AL_TIMER_ERROR;

    // Initializing allegro's primitive addon / Inicializando addon de primitivos do allegro ------ //
    if(!al_init_primitives_addon())
        return AL_PRIMITIVE_ERROR;

    // Initializing the keyboard / Inicializando o teclado ---------------------------------------- //
	if(!al_install_keyboard())
        return AL_KEYBOARD_ERROR;

	// Initializing the mouse / Inicializando o mouse --------------------------------------------- //
	if(!al_install_mouse())
		return AL_MOUSE_ERROR;

	// Initializing text fonts / Inicializando fontes para texto ---------------------------------- //
    al_init_font_addon();
    al_init_ttf_addon();
    ALLEGRO_FONT *font[FONT_NAME][FONT_SIZES];
    font[FONT_ARIAL][FONT_16] = al_load_font("fonts/arial.ttf", 16, 0);
    font[FONT_ARIAL][FONT_24] = al_load_font("fonts/arial.ttf", 24, 0);
    for(i=0; i<FONT_NAME; i++){
        for(j=0; j<FONT_SIZES; j++){
            if(font[i][j] == false)
                return AL_FONT_ERROR;
        }
    }

    // Initializing the eventQueue / Inicializando a fila de eventos ----------------------------- //
    ALLEGRO_EVENT_QUEUE *eventQueue = NULL;
    eventQueue = al_create_event_queue();
    if(!eventQueue)
        return AL_EVENT_QUEUE_ERROR;
    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_timer_event_source(timer));
    al_register_event_source(eventQueue, al_get_keyboard_event_source());
    al_register_event_source(eventQueue, al_get_mouse_event_source());

    // Initializing bitviews / Inicializando bitmaps ---------------------------------------------- //
    if(!al_init_image_addon())
    	return AL_IMAGE_ERROR;

    // GAME LOOP / LOOP DO JOGO ================================================================== //
    al_start_timer(timer);
    while(!game.quit){
        ALLEGRO_EVENT ev;
        al_wait_for_event(eventQueue, &ev);

        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        	game.quit = true;

        if(ev.type == ALLEGRO_EVENT_MOUSE_AXES || ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {
           mouse.coordX = ev.mouse.x;
           mouse.coordY = ev.mouse.y;
        }

        if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
            switch(ev.mouse.button){
            case 1:
            	mouse.btn[MOUSE_LEFT] = true;
            	break;
            case 2:
            	mouse.btn[MOUSE_RIGHT] = true;
            	break;
            case 3:
            	mouse.btn[MOUSE_MIDDLE] = true;
            	break;
            }
        }

        if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
            switch(ev.mouse.button){
            case 1:
            	mouse.btn[MOUSE_LEFT] = false;
            	break;
            case 2:
            	mouse.btn[MOUSE_RIGHT] = false;
            	break;
            case 3:
            	mouse.btn[MOUSE_MIDDLE] = false;
            	break;
            }
        }

        if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
            switch(ev.keyboard.keycode){
                case ALLEGRO_KEY_UP:
                    key[KEY_UP] = true;
                    break;
                case ALLEGRO_KEY_DOWN:
                    key[KEY_DOWN] = true;
                    break;
                case ALLEGRO_KEY_LEFT:
                    key[KEY_LEFT] = true;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    key[KEY_RIGHT] = true;
                    break;
                case ALLEGRO_KEY_A:
                    key[KEY_A] = true;
                    break;
                case ALLEGRO_KEY_B:
                    key[KEY_B] = true;
                    break;
                case ALLEGRO_KEY_C:
                    key[KEY_C] = true;
                    break;
                case ALLEGRO_KEY_D:
                    key[KEY_D] = true;
                    break;
                case ALLEGRO_KEY_E:
                    key[KEY_E] = true;
                    break;
                case ALLEGRO_KEY_F:
                    key[KEY_F] = true;
                    break;
                case ALLEGRO_KEY_G:
                    key[KEY_G] = true;
                    break;
                case ALLEGRO_KEY_H:
                    key[KEY_H] = true;
                    break;
                case ALLEGRO_KEY_I:
                    key[KEY_I] = true;
                    break;
                case ALLEGRO_KEY_J:
                    key[KEY_J] = true;
                    break;
                case ALLEGRO_KEY_K:
                    key[KEY_K] = true;
                    break;
                case ALLEGRO_KEY_L:
                    key[KEY_L] = true;
                    break;
                case ALLEGRO_KEY_M:
                    key[KEY_M] = true;
                    break;
                case ALLEGRO_KEY_N:
                    key[KEY_N] = true;
                    break;
                case ALLEGRO_KEY_O:
                    key[KEY_O] = true;
                    break;
                case ALLEGRO_KEY_P:
                    key[KEY_P] = true;
                    break;
                case ALLEGRO_KEY_Q:
                    key[KEY_Q] = true;
                    break;
                case ALLEGRO_KEY_R:
                    key[KEY_R] = true;
                    break;
                case ALLEGRO_KEY_S:
                    key[KEY_S] = true;
                    break;
                case ALLEGRO_KEY_T:
                    key[KEY_T] = true;
                    break;
                case ALLEGRO_KEY_U:
                    key[KEY_U] = true;
                    break;
                case ALLEGRO_KEY_V:
                    key[KEY_V] = true;
                    break;
                case ALLEGRO_KEY_W:
                    key[KEY_W] = true;
                    break;
                case ALLEGRO_KEY_X:
                    key[KEY_X] = true;
                    break;
                case ALLEGRO_KEY_Y:
                    key[KEY_Y] = true;
                    break;
                case ALLEGRO_KEY_Z:
                    key[KEY_Z] = true;
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    key[KEY_ESC] = true;
                    break;
                case ALLEGRO_KEY_ENTER:
                    key[KEY_ENTER] = true;
                    break;
                case ALLEGRO_KEY_LSHIFT:
                    key[KEY_SHIFT] = true;
                    break;
                case ALLEGRO_KEY_RSHIFT:
                    key[KEY_SHIFT] = true;
                    break;
                case ALLEGRO_KEY_BACKSPACE:
                	key[KEY_BACKSPACE] = true;
                	break;
            }
        }
        if(ev.type == ALLEGRO_EVENT_KEY_UP){
            switch(ev.keyboard.keycode){
                case ALLEGRO_KEY_UP:
                    key[KEY_UP] = false;
                    break;
                case ALLEGRO_KEY_DOWN:
                    key[KEY_DOWN] = false;
                    break;
                case ALLEGRO_KEY_LEFT:
                    key[KEY_LEFT] = false;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    key[KEY_RIGHT] = false;
                    break;
                case ALLEGRO_KEY_A:
                    key[KEY_A] = false;
                    break;
                case ALLEGRO_KEY_B:
                    key[KEY_B] = false;
                    break;
                case ALLEGRO_KEY_C:
                    key[KEY_C] = false;
                    break;
                case ALLEGRO_KEY_D:
                    key[KEY_D] = false;
                    break;
                case ALLEGRO_KEY_E:
                    key[KEY_E] = false;
                    break;
                case ALLEGRO_KEY_F:
                    key[KEY_F] = false;
                    break;
                case ALLEGRO_KEY_G:
                    key[KEY_G] = false;
                    break;
                case ALLEGRO_KEY_H:
                    key[KEY_H] = false;
                    break;
                case ALLEGRO_KEY_I:
                    key[KEY_I] = false;
                    break;
                case ALLEGRO_KEY_J:
                    key[KEY_J] = false;
                    break;
                case ALLEGRO_KEY_K:
                    key[KEY_K] = false;
                    break;
                case ALLEGRO_KEY_L:
                    key[KEY_L] = false;
                    break;
                case ALLEGRO_KEY_M:
                    key[KEY_M] = false;
                    break;
                case ALLEGRO_KEY_N:
                    key[KEY_N] = false;
                    break;
                case ALLEGRO_KEY_O:
                    key[KEY_O] = false;
                    break;
                case ALLEGRO_KEY_P:
                    key[KEY_P] = false;
                    break;
                case ALLEGRO_KEY_Q:
                    key[KEY_Q] = false;
                    break;
                case ALLEGRO_KEY_R:
                    key[KEY_R] = false;
                    break;
                case ALLEGRO_KEY_S:
                    key[KEY_S] = false;
                    break;
                case ALLEGRO_KEY_T:
                    key[KEY_T] = false;
                    break;
                case ALLEGRO_KEY_U:
                    key[KEY_U] = false;
                    break;
                case ALLEGRO_KEY_V:
                    key[KEY_V] = false;
                    break;
                case ALLEGRO_KEY_W:
                    key[KEY_W] = false;
                    break;
                case ALLEGRO_KEY_X:
                    key[KEY_X] = false;
                    break;
                case ALLEGRO_KEY_Y:
                    key[KEY_Y] = false;
                    break;
                case ALLEGRO_KEY_Z:
                    key[KEY_Z] = false;
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    key[KEY_ESC] = false;
                    break;
                case ALLEGRO_KEY_ENTER:
                    key[KEY_ENTER] = false;
                    break;
                case ALLEGRO_KEY_LSHIFT:
                    key[KEY_SHIFT] = false;
                    break;
                case ALLEGRO_KEY_RSHIFT:
                    key[KEY_SHIFT] = false;
                    break;
                case ALLEGRO_KEY_BACKSPACE:
                	key[KEY_BACKSPACE] = false;
                	break;
            }
        }

        // GAME LOGIC / LOGICA DO JOGO ----------------------------------------------------------- //
        if(ev.type == ALLEGRO_EVENT_TIMER){
        	fpsControl(&settings);
            if(key[KEY_ESC]) game.quit=true;
            switch(game.stage){
                case STAGE_INTRO_SCREEN:
                	if(key[KEY_ENTER] && menuControl.flag){
                		setTimer(&menuControl.time, 20);
                		game.stage = STAGE_MAIN_MENU;
                	}
                    break;
                case STAGE_MAIN_MENU:
                	if(key[KEY_ENTER] && menuControl.flag){
                		setTimer(&menuControl.time, 20);
                		game.stage = STAGE_IN_GAME;
                	}
                    break;
                case STAGE_IN_GAME:
                	if(key[KEY_P]){
                		setTimer(&menuControl.time, 20);
                		game.stage = STAGE_PAUSE_MENU;
                	}
                	if(key[KEY_W] && player.timeMovement.flag)
                		phAddAc(&player.acY, player.power, -1, player.weight, 1, NORMAL_SPEED);
                	if(key[KEY_S] && player.timeMovement.flag)
                		phAddAc(&player.acY, player.power, 1, player.weight, 1, NORMAL_SPEED);
                	if(key[KEY_A] && player.timeMovement.flag)
                		phAddAc(&player.acX, player.power, -1, player.weight, 1, NORMAL_SPEED);
                	if(key[KEY_D] && player.timeMovement.flag)
                		phAddAc(&player.acX, player.power, 1, player.weight, 1, NORMAL_SPEED);
                	if((!key[KEY_W]) && (!key[KEY_S]))
                		phNormalize(&player.acY, player.power/2, player.weight);
                	if((!key[KEY_A]) && (!key[KEY_D]))
                		phNormalize(&player.acX, player.power/2, player.weight);
                	if(mouse.btn[MOUSE_LEFT] && player.timeAttack.flag){
                		atkTackle(&player, mouse.coordX + view.coordX, mouse.coordY + view.coordY);
                		player.timeAttack.time = 100;
                		player.timeMovement.time = 10;
                	}

                    for(i = 0; i < map.totalEnemies; i++)
                    {
                        if(map.enemies[i].enable == 1)
                        {
                            phMoveEnemy(&map.enemies[i], map.circles, map.totalCircles);

                            phColide2Ball(&map.enemies[i],&player);
                            phColideBallRec(&map.enemies[i], &limits);

                            for(j = 0; j <i; j++)
                            {
                                phColide2Ball(&map.enemies[i],&map.enemies[j]);
                            }
                            for(j=0; j<map.totalSquares; j++)
                                phColideBallRec(&map.enemies[i], &map.squares[j]);
                        }
                    }

                    if((send_enemy == 50)&&(number_enemy<=map.totalEnemies))
                    {
                        map.enemies[number_enemy].enable = 1;
                        number_enemy++;

                        send_enemy = 0;
                    }
                    if(number_enemy<=map.totalEnemies)
                        send_enemy++;

                	phColideBallRec(&player, &limits);

                	for(i=0; i<map.totalSquares; i++)
                		phColideBallRec(&player, &map.squares[i]);

                	phMoveObject(&player);

                	moveViewPoint(&player, &view);

                    break;
                case STAGE_PAUSE_MENU:
                	if(key[KEY_BACKSPACE]){
                		setTimer(&menuControl.time, 20);
                		game.stage = STAGE_IN_GAME;
                	}
                	break;
                default:
                    return AL_STATE_ERROR;
                    break;
            }

            checkTimer(&menuControl);
            checkTimer(&player.timeAttack);
            checkTimer(&player.timeMovement);
        }

        // GAME ART / ARTE DO JOGO --------------------------------------------------------------- //
        if(al_is_event_queue_empty(eventQueue) && (settings.fpsCount == 0)){
            al_clear_to_color(al_map_rgb(0,0,0));
            switch(game.stage){
                case STAGE_INTRO_SCREEN:
                	al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), settings.displayX/2,
                			settings.displayY/2, ALLEGRO_ALIGN_CENTRE, "INTRO SCREEN");
                    break;
                case STAGE_MAIN_MENU:
                	al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), settings.displayX/2,
                			settings.displayY/2, ALLEGRO_ALIGN_CENTRE, "MAIN MENU");
                    break;
                case STAGE_IN_GAME:
                	al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), settings.displayX/2,
                			settings.displayY*0.1, ALLEGRO_ALIGN_CENTRE, "IN GAME");
                	al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), settings.displayX*0.1,
                			settings.displayY*0.9, ALLEGRO_ALIGN_CENTRE, "coordX: %.2f", player.coordX - view.coordX);
                	al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), settings.displayX*0.1,
                			settings.displayY*0.95, ALLEGRO_ALIGN_CENTRE, "coordX: %.2f", player.coordX);
                	al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), settings.displayX*0.2,
                			settings.displayY*0.9, ALLEGRO_ALIGN_CENTRE, "coordY: %.2f", player.coordY - view.coordY);
                	al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), settings.displayX*0.2,
                			settings.displayY*0.95, ALLEGRO_ALIGN_CENTRE, "coordY: %.2f", player.coordY);
                	al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), settings.displayX*0.3,
                			settings.displayY*0.9, ALLEGRO_ALIGN_CENTRE, "acX: %.2f", player.acX);
                	al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), settings.displayX*0.3,
                			settings.displayY*0.95, ALLEGRO_ALIGN_CENTRE, "acY: %.2f", player.acY);
                	al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), settings.displayX*0.4,
                			settings.displayY*0.9, ALLEGRO_ALIGN_CENTRE, "angle: %.2f", player.dAngle);

                    for(i=0; i<map.totalCircles; i++)
                    {
                        al_draw_filled_circle(map.circles[i].coordX - view.coordX, map.circles[i].coordY - view.coordY, map.circles[i].radius, al_map_rgb(255, 0, 0));
                    }

                    al_draw_filled_circle(player.coordX - view.coordX, player.coordY - view.coordY, player.radius, al_map_rgb(255, 255, 255));

                    for(i = 0; i < map.totalEnemies; i++)
                    {
                        if(map.enemies[i].enable == 1)
                            al_draw_filled_circle(map.enemies[i].coordX - view.coordX, map.enemies[i].coordY - view.coordY, map.enemies[i].radius, al_map_rgb(0, 255, 255));
                    }

                    for(i=0; i<map.totalSquares; i++){
                    	al_draw_rectangle(map.squares[i].coordX1 - view.coordX, map.squares[i].coordY1 - view.coordY, map.squares[i].coordX2 - view.coordX,
                    			map.squares[i].coordY2 - view.coordY, al_map_rgb(255, 255, 255), 5);
                	}

                	al_draw_rectangle(limits.coordX1 - view.coordX, limits.coordY1 - view.coordY,
                			limits.coordX2 - view.coordX, limits.coordY2 - view.coordY, al_map_rgb(255, 255, 255), 5);
                    al_draw_filled_circle(mouse.coordX, mouse.coordY, 5, al_map_rgb(255, 255, 255));
                    break;
                case STAGE_PAUSE_MENU:
                	al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), settings.displayX/2,
                			settings.displayY/2, ALLEGRO_ALIGN_CENTRE, "PAUSED");
                	break;
                default:
                    return AL_STATE_ERROR;
                    break;
            }
            al_flip_display();
        }
    }

    // ALLEGRO DEINIT / DESTRUICAO DOS COMPONENTES =============================================== //
    al_destroy_display(display);
    al_destroy_event_queue(eventQueue);
    al_destroy_timer(timer);
    for(i=0; i<FONT_NAME; i++){
        for(j=0; j<FONT_SIZES; j++){
            al_destroy_font(font[i][j]);
        }
    }

    return SABABA;
}
