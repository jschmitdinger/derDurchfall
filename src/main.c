/* ======================================= DER DURCHFALL ========================================== //
| INSTITUTO FEDERAL DE SANTA CATARINA - IFSC			FEDERAL INSTITUTE OF SANTA CATARINA			||
| DISCIPLINA DE PROGRAMACAO C							C PROGRAMMING LANGUAGE						||
| PROFESSOR FERNANDO PACHECO							PROFESSOR FERNANDO PACHECO					||
| CAIO PRUJANSKY E JOHANN SCHMITDINGER					CAIO PRUJANSKY AND JOHANN SCHMITDINGER		||
| ARQUIVO: main.c										FILE: main.c								||
| VERSAO: v1.0											VERSION: v1.0								||
| DATA DE ALTERACAO: 02/06/2015							LAST ALTERED DATE: 02/06/2015				||
// ================================================================================================ */

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
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
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

	sounds samples;

	cursor mouse;
	mouse.btn[0] = 0;
	mouse.btn[1] = 0;
	mouse.btn[2] = 0;

	char n;
	char rank_names[10][10];
	int rank_scores[10];
	FILE *rank;

	profile user;
	user.score = 0;

	logo logos[6];
	logos[LOGO_IFSC].acX = 10;
	logos[LOGO_IFSC].acY = 10;
	logos[LOGO_IFSC].coordX = -500;
	logos[LOGO_IFSC].coordY = -900;
	logos[LOGO_ALLEGRO].acX = 10;
	logos[LOGO_ALLEGRO].acY = 10;
	logos[LOGO_ALLEGRO].coordX = settings.displayX+500;
	logos[LOGO_ALLEGRO].coordY = -600;
	logos[2].acX = 0;
	logos[2].acY = 5;
	logos[2].coordX = settings.displayX/2;
	logos[2].coordY = -2000;
	logos[3].acX = 0;
	logos[3].acY = 5;
	logos[3].coordX = settings.displayX/2;
	logos[3].coordY = -1900;

	logos[4].acX = 5;
	logos[4].acY = 0;
	logos[4].coordX = -6000;
	logos[4].coordY = settings.displayY/2-200;
	logos[5].acX = 5;
	logos[5].acY = 0;
	logos[5].coordX = settings.displayX+5000;
	logos[5].coordY = settings.displayY/2-200;

    char key[TOTAL_KEY] = {false};
	int i, j, k;
    int send_enemy = 0;
	int number_enemy = 0;
	int enemys_dead = 0;
    int enemy_type = 0;
	int enemys_phase = 1;

	gameStatus game;
	game.quit = false;
	game.stage = STAGE_INTRO_SCREEN;
	game.acX = 0;
	game.acY = 0;
	game.bgxcount = 0;
	game.bgycount = 0;
	game.invert = 1;
	game.menuChangeTimer = 0;

	timeControl menuControl;
	menuControl.flag = false;
	menuControl.time = 0;

	menuOptionControl menus[8];
	menus[STAGE_MAIN_MENU].first = settings.displayY*0.55;
	menus[STAGE_MAIN_MENU].distancia = 60;
	menus[STAGE_MAIN_MENU].options = 4;
	menus[STAGE_MAIN_MENU].local = 0;

	gameMap map;

	avatar player;

	mapView view;

	square limits;

	spritsCrianca sprites;

	// INITIALIZING EVERYTHING / INICIALISANDO TUDO =============================================== //
	// Initializing allegro / Inicializando allegro ----------------------------------------------- //
    if (!al_init())
        return AL_INIT_ERROR;

    // Initializing display / Inicializando display ----------------------------------------------- //
    ALLEGRO_DISPLAY *display = NULL;
    display = al_create_display(settings.displayX, settings.displayY);
    if(!display)
		return AL_DISPLAY_ERROR;

	// Initializing text fonts / Inicializando fontes para texto ---------------------------------- //
    al_init_font_addon();
    al_init_ttf_addon();
    ALLEGRO_FONT *font[FONT_NAME][FONT_SIZES];
    font[FONT_ARIAL][FONT_16] = al_load_font("fonts/arial.ttf", 16, 0);
    font[FONT_ARIAL][FONT_24] = al_load_font("fonts/arial.ttf", 24, 0);
    font[FONT_ARIAL][FONT_26] = al_load_font("fonts/arial.ttf", 26, 0);
    font[FONT_ARIAL][FONT_32] = al_load_font("fonts/arial.ttf", 32, 0);
    font[FONT_ARIAL][FONT_34] = al_load_font("fonts/arial.ttf", 34, 0);
    for(i=0; i<FONT_NAME; i++){
        for(j=0; j<FONT_SIZES; j++){
            if(font[i][j] == false)
                return AL_FONT_ERROR;
        }
    }

    al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), 5, 0, ALLEGRO_ALIGN_LEFT, "Initializing allegro5...");
    al_flip_display();

    // Initializing timer / Inicializando timer --------------------------------------------------- //
    ALLEGRO_TIMER *timer = NULL;
    al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), 5, 15, ALLEGRO_ALIGN_LEFT, "   Setting timer...");
    al_flip_display();
    timer = al_create_timer(1.0 / GAME_SPEED);
    if(!timer)
        return AL_TIMER_ERROR;
    al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), 5, 15, ALLEGRO_ALIGN_LEFT, "   Setting timer... Done.");
    al_flip_display();

    // Initializing allegro's primitive addon / Inicializando addon de primitivos do allegro ------ //
    al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), 5, 30, ALLEGRO_ALIGN_LEFT, "   Initializing primitives addon...");
    al_flip_display();
    if(!al_init_primitives_addon())
        return AL_PRIMITIVE_ERROR;
    al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), 5, 30, ALLEGRO_ALIGN_LEFT, "   Initializing primitives addon... Done.");
    al_flip_display();

    // Initializing the keyboard / Inicializando o teclado ---------------------------------------- //
    al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), 5, 45, ALLEGRO_ALIGN_LEFT, "   Initializing keyboard...");
    al_flip_display();
	if(!al_install_keyboard())
        return AL_KEYBOARD_ERROR;
    al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), 5, 45, ALLEGRO_ALIGN_LEFT, "   Initializing keyboard... Done.");
    al_flip_display();

	// Initializing the mouse / Inicializando o mouse --------------------------------------------- //
    al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), 5, 60, ALLEGRO_ALIGN_LEFT, "   Initializing mouse...");
    al_flip_display();
	if(!al_install_mouse())
		return AL_MOUSE_ERROR;
    al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), 5, 60, ALLEGRO_ALIGN_LEFT, "   Initializing mouse... Done.");
    al_flip_display();

    // Initializing the eventQueue / Inicializando a fila de eventos ----------------------------- //
    ALLEGRO_EVENT_QUEUE *eventQueue = NULL;
    eventQueue = al_create_event_queue();
    if(!eventQueue)
        return AL_EVENT_QUEUE_ERROR;
    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_timer_event_source(timer));
    al_register_event_source(eventQueue, al_get_keyboard_event_source());
    al_register_event_source(eventQueue, al_get_mouse_event_source());

    // Initializing bitviews / Inicializando bitmaps -------------------------------------------- //
    al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), 5, 75, ALLEGRO_ALIGN_LEFT, "   Initializing bitmap addon...");
    al_flip_display();
    if(!al_init_image_addon())
    	return AL_IMAGE_ERROR;
    al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), 5, 75, ALLEGRO_ALIGN_LEFT, "   Initializing bitmap addon... Done.");
    al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), 5, 0, ALLEGRO_ALIGN_LEFT, "Initializing allegro5... Done.");
    al_flip_display();

    // Loading bitmaps / Carregando bitmaps ----------------------------------------------------- //
    al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), 5, 90, ALLEGRO_ALIGN_LEFT, "Reading data files...");
    al_flip_display();
	initMap(&map, "maps/map01.txt");
    al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), 5, 90, ALLEGRO_ALIGN_LEFT, "Reading data files... Done.");
    al_flip_display();

    if(!al_install_audio()){
       fprintf(stderr, "failed to initialize audio!\n");
       return -1;
    }

    if(!al_init_acodec_addon()){
       fprintf(stderr, "failed to initialize audio codecs!\n");
       return -1;
    }

    if (!al_reserve_samples(15)){
       fprintf(stderr, "failed to reserve samples!\n");
       return -1;
    }

	initPlayer(&player, TYPE_NORMAL, map.width, map.height);

	for(i = 0; i < enemys_phase; i++)
        initEnemy(&map.enemies[i], &map, TYPE_NORMAL,map.width,map.height);

	view.totalWidth = map.width;
	view.totalHeight = map.height;
	view.rangeX = settings.displayX;
	view.rangeY = settings.displayY;
	view.coordX = (view.totalWidth/2) - (view.rangeX/2);
	view.coordY = (view.totalHeight/2) - (view.rangeY/2);

	limits.coordX1 = 0;
	limits.coordY1 = 0;
	limits.coordX2 = map.width;
	limits.coordY2 = map.height;
	limits.flagA = 1;
	limits.flagB = 1;
	limits.flagC = 1;
	limits.flagD = 1;

	sprites.logos[LOGO_IFSC] = al_load_bitmap("images/logos/ifsc.png");
	al_convert_mask_to_alpha(sprites.logos[LOGO_IFSC], al_map_rgb(255, 0, 255));
	sprites.logos[LOGO_ALLEGRO] = al_load_bitmap("images/logos/allegro.png");
	al_convert_mask_to_alpha(sprites.logos[LOGO_ALLEGRO], al_map_rgb(255, 0, 255));

	sprites.menuItems[START] = al_load_bitmap("images/menu/start game.png");
	sprites.menuItems[MULTI] = al_load_bitmap("images/menu/multiplayer.png");
	sprites.menuItems[LEADER] = al_load_bitmap("images/menu/leaderboard.png");
	sprites.menuItems[SETTINGS] = al_load_bitmap("images/menu/setting.png");
	sprites.menuItems[QUIT] = al_load_bitmap("images/menu/quit.png");
	for(i=0; i<TOTAL_ITEMS; i++)
		al_convert_mask_to_alpha(sprites.menuItems[i], al_map_rgb(255, 0, 255));

    al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), 5, 105, ALLEGRO_ALIGN_LEFT, "Loading bitmaps...");
    al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), 5, 120, ALLEGRO_ALIGN_LEFT, "   Loading background...");
    al_flip_display();
    sprites.bg = al_load_bitmap("images/map/bg.jpg");
    sprites.title = al_load_bitmap("images/menu/rsz_game_title.png");
    al_convert_mask_to_alpha(sprites.title, al_map_rgb(255, 0, 255));
    al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), 5, 120, ALLEGRO_ALIGN_LEFT, "   Loading background... Done.");
    al_flip_display();

	al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), 5, 135, ALLEGRO_ALIGN_LEFT, "   Loading walls...");
	al_flip_display();
	sprites.block = al_load_bitmap("images/map/block.png");
	al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), 5, 135, ALLEGRO_ALIGN_LEFT, "   Loading walls... Done.");
	al_flip_display();

	al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), 5, 150, ALLEGRO_ALIGN_LEFT, "   Loading player...");
	al_flip_display();
	sprites.spriteHead = al_load_bitmap("images/character/player_head32_28.bmp");
    al_convert_mask_to_alpha(sprites.spriteHead, al_map_rgb(255, 0, 255));
    sprites.spriteBody = al_load_bitmap("images/character/player_body22.bmp");
    al_convert_mask_to_alpha(sprites.spriteBody, al_map_rgb(255, 0, 255));
    sprites.spriteMelee1 = al_load_bitmap("images/attacks/fire_and_boom_blue_70_67.bmp");
    al_convert_mask_to_alpha(sprites.spriteMelee1, al_map_rgb(255, 0, 255));
	al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), 5, 150, ALLEGRO_ALIGN_LEFT, "   Loading player... Done.");
	al_flip_display();
	al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), 5, 165, ALLEGRO_ALIGN_LEFT, "   Loading shots...");
	al_flip_display();
    sprites.projectile1 = al_load_bitmap("images/attacks/shot_blue_34_34.bmp");
    al_convert_mask_to_alpha(sprites.projectile1, al_map_rgb(0,255,0));
	al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), 5, 165, ALLEGRO_ALIGN_LEFT, "   Loading shots... Done.");
	al_flip_display();

	al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), 5, 180, ALLEGRO_ALIGN_LEFT, "   Loading enemies...");
	al_flip_display();
    sprites.enemy = al_load_bitmap("images/enemies/blob_44_44.bmp");
    al_convert_mask_to_alpha(sprites.enemy, al_map_rgb(255,0,255));
    sprites.projectile2 = al_load_bitmap("images/attacks/shot_pink_34_34.bmp");
    al_convert_mask_to_alpha(sprites.projectile2, al_map_rgb(0,255,0));
    sprites.spriteMelee2 = al_load_bitmap("images/attacks/fire_and_boom_70_67.bmp");
    al_convert_mask_to_alpha(sprites.spriteMelee2, al_map_rgb(255, 0, 255));
	al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), 5, 180, ALLEGRO_ALIGN_LEFT, "   Loading enemies... Done.");
	al_flip_display();

	al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), 5, 195, ALLEGRO_ALIGN_LEFT, "   Loading vitals...");
	al_flip_display();
    sprites.circle = al_load_bitmap("images/map/vital.png");
    al_convert_mask_to_alpha(sprites.circle, al_map_rgb(255, 0, 255));
    al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), 5, 105, ALLEGRO_ALIGN_LEFT, "Loading bitmaps... Done.");
	al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), 5, 195, ALLEGRO_ALIGN_LEFT, "   Loading vitals... Done.");
	al_flip_display();

	sprites.caio = al_load_bitmap("images/dev/caio.jpg");
	sprites.johann = al_load_bitmap("images/dev/johann.jpg");

	samples.menuBackground = al_load_sample("sounds/intro.wav");
	samples.menuSwitch = al_load_sample("sounds/pick.wav");
	samples.menuSelect = al_load_sample("sounds/select.wav");
	samples.type = al_load_sample("sounds/typewriter.wav");
	samples.shotp = al_load_sample("sounds/player shot.wav");
	samples.damagep = al_load_sample("sounds/player damage.wav");
	samples.shote = al_load_sample("sounds/enemy shot.wav");
	samples.damagep = al_load_sample("sounds/gotshot.wav");
	samples.hit = al_load_sample("sounds/colision.wav");
	samples.gameBackground = al_load_sample("sounds/starfox.wav");
	al_play_sample(samples.menuBackground, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);

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
                	if(game.menuChangeTimer <= 240){
						if(logos[LOGO_IFSC].coordX > 0){
							phNormalize(&logos[LOGO_IFSC].acX, 0.1, 1);
							phNormalize(&logos[LOGO_IFSC].acY, 0.1, 1);
						}
						if(logos[LOGO_ALLEGRO].coordX < settings.displayX){
							phNormalize(&logos[LOGO_ALLEGRO].acX, 0.2, 1);
							phNormalize(&logos[LOGO_ALLEGRO].acY, 0.2, 1);
						}
                	} else if(game.menuChangeTimer <= 1050){
                		phAddAc(&logos[LOGO_IFSC].acX, 0.5, 1, 1, 1, 10);
                		phAddAc(&logos[LOGO_ALLEGRO].acX, 0.5, 1, 1, 1, 10);
                		if(logos[2].coordY >= 0){
							phNormalize(&logos[2].acX, 0.05, 1);
							phNormalize(&logos[2].acY, 0.05, 1);
							phNormalize(&logos[3].acX, 0.05, 1);
							phNormalize(&logos[3].acY, 0.05, 1);
                		}
                	} else if(game.menuChangeTimer <= 1550){
                		phAddAc(&logos[2].acX, 0.5, 1, 1, 1, 10);
                		phAddAc(&logos[3].acX, 0.5, -1, 1, 1, 10);
                		if(logos[4].coordX >= 0){
							phNormalize(&logos[4].acX, 0.05, 1);
							phNormalize(&logos[4].acY, 0.05, 1);
                		}
                		if(logos[5].coordX <= settings.displayX){
							phNormalize(&logos[5].acX, 0.05, 1);
							phNormalize(&logos[5].acY, 0.05, 1);
                		}
                	} else if(game.menuChangeTimer <= 1800){
                		phAddAc(&logos[4].acX, 0.5, 1, 1, 1, 10);
                		phAddAc(&logos[5].acX, 0.5, -1, 1, 1, 10);
                	} else if(game.menuChangeTimer >= 2050){
                		setTimer(&menuControl.time, 20);
                		game.stage = STAGE_MAIN_MENU;
                	}
					logos[LOGO_IFSC].coordX += logos[LOGO_IFSC].acX;
					logos[LOGO_IFSC].coordY += logos[LOGO_IFSC].acY;
					logos[LOGO_ALLEGRO].coordX -= logos[LOGO_IFSC].acX;
					logos[LOGO_ALLEGRO].coordY += logos[LOGO_IFSC].acY;
					logos[2].coordX += logos[2].acX;
					logos[2].coordY += logos[2].acY;
					logos[3].coordX += logos[3].acX;
					logos[3].coordY += logos[3].acY;
					logos[4].coordX += logos[4].acX;
					logos[4].coordY += logos[4].acY;
					logos[5].coordX -= logos[5].acX;
					logos[5].coordY += logos[5].acY;

					if(key[KEY_ENTER]){
						al_play_sample(samples.menuSelect, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
						setTimer(&menuControl.time, 20);
						game.stage = STAGE_MAIN_MENU;
					}

            		game.menuChangeTimer++;

                    break;
                case STAGE_MAIN_MENU:
                  	if(key[KEY_UP] && menuControl.flag){
                  		setTimer(&menuControl.time, 20);
                  		if(menus[STAGE_MAIN_MENU].local > 0)
                  			menus[STAGE_MAIN_MENU].local--;
                  		al_play_sample(samples.menuSwitch, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                  	}
                  	if(key[KEY_DOWN] && menuControl.flag){
                  		setTimer(&menuControl.time, 20);
                  		if(menus[STAGE_MAIN_MENU].local < 4)
                  			menus[STAGE_MAIN_MENU].local++;
                  		al_play_sample(samples.menuSwitch, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                  	}
                	if(key[KEY_ENTER] && menuControl.flag){
                		switch(menus[STAGE_MAIN_MENU].local){
                		case 0:
                			j=0;
                    		game.stage = STAGE_PLAYER_SCREEN;
                			break;
                		case 2:
						    for(i=0; i<10; i++)
						    {
						        rank_scores[i] = 0;
						        for(j=0; j<10; j++)
						        {
						            rank_names[i][j] = '\0';
						        }
						    }
						    // Reads table
						    rank=fopen("files/rank.txt", "r");

						    i = 0;
						    while(1){
						        n=fscanf(rank, "%s %d", rank_names[i], &rank_scores[i]);

						        if(n == EOF){
						            break;
						        }
						        i++;
						    }

						    fclose(rank);
                			game.stage = STAGE_SCORE_SCREEN;
                			break;
                		case 4:
                    		game.quit = true;
                			break;
                		default:
                			break;
                		}
                		al_play_sample(samples.menuSelect, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                		setTimer(&menuControl.time, 20);
                	}
                	if(((game.invert == 1) && (game.bgxcount > 2280)) || ((game.invert == (-1)) && (game.bgxcount < 200)))
                		game.invert *= (-1);
            		phAddAc(&game.acX, 0.2, game.invert, 1, 1, 2);
            		phAddAc(&game.acY, 0.2, game.invert, 1, 1, 2);
            		game.bgxcount += game.acX;
            		game.bgycount += game.acY;
                    break;
                case STAGE_PLAYER_SCREEN:
                	if(menuControl.flag){
						if(key[KEY_A]){
							al_play_sample(samples.type, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
							user.name[j] = 'A';
							if(j<9)
								j++;
							setTimer(&menuControl.time, 20);
						}
						if(key[KEY_B]){
							al_play_sample(samples.type, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
							user.name[j] = 'B';
							if(j<9)
								j++;
							setTimer(&menuControl.time, 20);
						}
						if(key[KEY_C]){
							al_play_sample(samples.type, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
							user.name[j] = 'C';
							if(j<9)
								j++;
							setTimer(&menuControl.time, 20);
						}
						if(key[KEY_D]){
							al_play_sample(samples.type, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
							user.name[j] = 'D';
							if(j<9)
								j++;
							setTimer(&menuControl.time, 20);
						}
						if(key[KEY_E]){
							al_play_sample(samples.type, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
							user.name[j] = 'E';
							if(j<9)
								j++;
							setTimer(&menuControl.time, 20);
						}
						if(key[KEY_F]){
							al_play_sample(samples.type, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
							user.name[j] = 'F';
							if(j<9)
								j++;
							setTimer(&menuControl.time, 20);
						}
						if(key[KEY_G]){
							al_play_sample(samples.type, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
							user.name[j] = 'G';
							if(j<9)
								j++;
							setTimer(&menuControl.time, 20);
						}
						if(key[KEY_H]){
							al_play_sample(samples.type, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
							user.name[j] = 'H';
							if(j<9)
								j++;
							setTimer(&menuControl.time, 20);
						}
						if(key[KEY_I]){
							al_play_sample(samples.type, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
							user.name[j] = 'I';
							if(j<9)
								j++;
							setTimer(&menuControl.time, 20);
						}
						if(key[KEY_J]){
							al_play_sample(samples.type, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
							user.name[j] = 'J';
							if(j<9)
								j++;
							setTimer(&menuControl.time, 20);
						}
						if(key[KEY_K]){
							al_play_sample(samples.type, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
							user.name[j] = 'K';
							if(j<9)
								j++;
							setTimer(&menuControl.time, 20);
						}
						if(key[KEY_L]){
							al_play_sample(samples.type, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
							user.name[j] = 'L';
							if(j<9)
								j++;
							setTimer(&menuControl.time, 20);
						}
						if(key[KEY_M]){
							al_play_sample(samples.type, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
							user.name[j] = 'M';
							if(j<9)
								j++;
							setTimer(&menuControl.time, 20);
						}
						if(key[KEY_N]){
							al_play_sample(samples.type, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
							user.name[j] = 'N';
							if(j<9)
								j++;
							setTimer(&menuControl.time, 20);
						}
						if(key[KEY_O]){
							al_play_sample(samples.type, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
							user.name[j] = 'O';
							if(j<9)
								j++;
							setTimer(&menuControl.time, 20);
						}
						if(key[KEY_P]){
							al_play_sample(samples.type, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
							user.name[j] = 'P';
							if(j<9)
								j++;
							setTimer(&menuControl.time, 20);
						}
						if(key[KEY_Q]){
							al_play_sample(samples.type, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
							user.name[j] = 'Q';
							if(j<9)
								j++;
							setTimer(&menuControl.time, 20);
						}
						if(key[KEY_R]){
							al_play_sample(samples.type, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
							user.name[j] = 'R';
							if(j<9)
								j++;
							setTimer(&menuControl.time, 20);
						}
						if(key[KEY_S]){
							al_play_sample(samples.type, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
							user.name[j] = 'S';
							if(j<9)
								j++;
							setTimer(&menuControl.time, 20);
						}
						if(key[KEY_T]){
							al_play_sample(samples.type, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
							user.name[j] = 'T';
							if(j<9)
								j++;
							setTimer(&menuControl.time, 20);
						}
						if(key[KEY_U]){
							al_play_sample(samples.type, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
							user.name[j] = 'U';
							if(j<9)
								j++;
							setTimer(&menuControl.time, 20);
						}
						if(key[KEY_V]){
							al_play_sample(samples.type, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
							user.name[j] = 'V';
							if(j<9)
								j++;
							setTimer(&menuControl.time, 20);
						}
						if(key[KEY_W]){
							al_play_sample(samples.type, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
							user.name[j] = 'W';
							if(j<9)
								j++;
							setTimer(&menuControl.time, 20);
						}
						if(key[KEY_X]){
							al_play_sample(samples.type, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
							user.name[j] = 'X';
							if(j<9)
								j++;
							setTimer(&menuControl.time, 20);
						}
						if(key[KEY_Y]){
							al_play_sample(samples.type, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
							user.name[j] = 'Y';
							if(j<9)
								j++;
							setTimer(&menuControl.time, 20);
						}
						if(key[KEY_Z]){
							al_play_sample(samples.type, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
							user.name[j] = 'Z';
							if(j<9)
								j++;
							setTimer(&menuControl.time, 20);
						}
						if(key[KEY_BACKSPACE]){
							al_play_sample(samples.type, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
							if(j>0){
								j--;
								user.name[j] = '\0';
							}
							setTimer(&menuControl.time, 20);
						}
						if(key[KEY_ENTER]){
							al_stop_samples();
							al_play_sample(samples.menuSelect, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
						    send_enemy = 0;
							number_enemy = 0;
							enemys_dead = 0;
							enemys_phase = 1;

							initPlayer(&player, TYPE_NORMAL, map.width, map.height);

							for(i = 0; i < enemys_phase; i++)
						        initEnemy(&map.enemies[i], &map, TYPE_NORMAL,map.width,map.height);

							view.totalWidth = map.width;
							view.totalHeight = map.height;
							view.rangeX = settings.displayX;
							view.rangeY = settings.displayY;
							view.coordX = (view.totalWidth/2) - (view.rangeX/2);
							view.coordY = (view.totalHeight/2) - (view.rangeY/2);

							limits.coordX1 = 0;
							limits.coordY1 = 0;
							limits.coordX2 = map.width;
							limits.coordY2 = map.height;
							limits.flagA = 1;
							limits.flagB = 1;
							limits.flagC = 1;
							limits.flagD = 1;

							for(i=0; i<map.totalCircles; i++)
								map.circles[i].life = 100;

						    for(i=0; i<10; i++)
						    {
						        rank_scores[i] = 0;
						        for(j=0; j<10; j++)
						        {
						            rank_names[i][j] = '\0';
						        }
						    }
						    // Reads table
						    rank=fopen("files/rank.txt", "r");

						    i = 0;
						    while(1){
						        n=fscanf(rank, "%s %d", rank_names[i], &rank_scores[i]);

						        if(n == EOF){
						            break;
						        }
						        i++;
						    }

						    fclose(rank);

							game.stage = STAGE_IN_GAME;
							al_play_sample(samples.gameBackground, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);
							setTimer(&menuControl.time, 20);
						}
                	}
                	if(((game.invert == 1) && (game.bgxcount > 2280)) || ((game.invert == (-1)) && (game.bgxcount < 200)))
                		game.invert *= (-1);
            		phAddAc(&game.acX, 0.2, game.invert, 1, 1, 2);
            		phAddAc(&game.acY, 0.2, game.invert, 1, 1, 2);
            		game.bgxcount += game.acX;
            		game.bgycount += game.acY;
                	break;
                case STAGE_IN_GAME:
                	// Enters pause menu
                	if(key[KEY_P]){
                		setTimer(&menuControl.time, 20);
                		game.stage = STAGE_PAUSE_MENU;
                	}

                	// Moves player up
                	if(key[KEY_W] && player.timeMovement.flag)
                		phAddAc(&player.acY, player.power, -1, player.weight, 1, NORMAL_SPEED);

                	// Moves player down
                	if(key[KEY_S] && player.timeMovement.flag)
                		phAddAc(&player.acY, player.power, 1, player.weight, 1, NORMAL_SPEED);

                	// Moves player left
                	if(key[KEY_A] && player.timeMovement.flag)
                		phAddAc(&player.acX, player.power, -1, player.weight, 1, NORMAL_SPEED);

                	// Moves player right
                	if(key[KEY_D] && player.timeMovement.flag)
                		phAddAc(&player.acX, player.power, 1, player.weight, 1, NORMAL_SPEED);

                	// Brakes player vertically
                	if((!key[KEY_W]) && (!key[KEY_S]))
                		phNormalize(&player.acY, player.power/2, player.weight);

                	// Brakes player horizontally
                	if((!key[KEY_A]) && (!key[KEY_D]))
                		phNormalize(&player.acX, player.power/2, player.weight);

                	// Triggers melee attack for player
                	if((mouse.btn[MOUSE_RIGHT]) && (player.timeAttack.flag)){
                		atkTackle(&player, mouse.coordX + view.coordX, mouse.coordY + view.coordY);
                		setTimer(&player.timeMovement.time, player.delay[ATCK_MOOVE]);
                		setTimer(&player.timeAttack.time, player.delay[ATCK_MELEE]);
                	}

                	// Triggers projectile attack for player
                	if((mouse.btn[MOUSE_LEFT]) && (player.timeAttack.flag)){
                		atkShoot(&player, mouse.coordX + view.coordX, mouse.coordY + view.coordY);
                		setTimer(&player.timeAttack.time, player.delay[ATCK_SHOOT]);
                		al_play_sample(samples.shotp, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                	}

                	// Triggers projectile attack for enemies
                	for(i=0; i<enemys_phase; i++){
                    	if((map.enemies[i].timeAttack.flag) && (map.enemies[i].enable)){
                    		map.enemies[i].distanceToPlayer = DISTANCE(player.coordX, player.coordY, map.enemies[i].coordX, map.enemies[i].coordY);
                    		if(((map.enemies[i].distanceToPlayer > (-CONST_DISTANCE_FAR)) && (map.enemies[i].distanceToPlayer < (CONST_DISTANCE_FAR))) ||
                    				((map.enemies[i].distanceToPlayer < (-CONST_DISTANCE_FAR)) && (map.enemies[i].distanceToPlayer > (CONST_DISTANCE_FAR)))){
                    			atkShoot(&map.enemies[i], player.coordX, player.coordY);
								setTimer(&map.enemies[i].timeAttack.time, map.enemies[i].delay[ATCK_SHOOT]);
								al_play_sample(samples.shote, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                    		}
                    	}
                	}

                	// Triggers melee attack for enemies
                	for(i=0; i<enemys_phase; i++){
                    	if((map.enemies[i].timeAttack.flag) && (map.enemies[i].enable)){
                    		map.enemies[i].distanceToPlayer = DISTANCE(player.coordX, player.coordY, map.enemies[i].coordX, map.enemies[i].coordY);
                    		if(((map.enemies[i].distanceToPlayer > (-CONST_DISTANCE)) && (map.enemies[i].distanceToPlayer < (CONST_DISTANCE))) ||
                    				((map.enemies[i].distanceToPlayer < (-CONST_DISTANCE)) && (map.enemies[i].distanceToPlayer > (CONST_DISTANCE)))){
								atkTackle(&map.enemies[i], player.coordX, player.coordY);
								setTimer(&map.enemies[i].timeAttack.time, map.enemies[i].delay[ATCK_MELEE]);
                    		}
                    	}
                	}

                    for(i = 0; i < (enemys_phase); i++){
                        if(map.enemies[i].enable == 1)
                        {
                            phMoveEnemy(&map.enemies[i], map.circles, map.totalCircles);

                            phColide2Ball(&map.enemies[i],&player, samples.hit);
                            phColideBallRec(&map.enemies[i], &limits);

                            phColideShotBall(&player,&map.enemies[i], samples.damagep);
                            phColideShotBall(&map.enemies[i],&player, samples.damagep);

                            for(j=0; j<map.totalCircles; j++)
                                vitalDemage(&map.circles[j], &map.enemies[i]);

                            for(j = 0; j <i; j++)
                            {
                                if(map.enemies[j].enable == 1)
                                    phColide2Ball(&map.enemies[i],&map.enemies[j], samples.hit);
                            }
                            for(j=0; j<map.totalSquares; j++)
                                phColideBallRec(&map.enemies[i], &map.squares[j]);

                            if(map.enemies[i].life <= 0)
                            {
                                map.enemies[i].enable = 0;
                                enemys_dead++;
                            }

                            setLookDirection(&map.enemies[i], player.coordX, player.coordY);
                        }
                    }

                    if((send_enemy == 50)&&(number_enemy<=enemys_phase)){
                        map.enemies[number_enemy].enable = 1;
                        number_enemy++;

                        send_enemy = 0;
                    }

                    if(number_enemy<=enemys_phase)
                        send_enemy++;

                    // Collides player and his shots with limits
                	phColideBallRec(&player, &limits);
                	phColideShotRec(&player, &limits);

                	// Colides player and shots from player with rectangles
                	for(i=0; i<map.totalSquares; i++){
                		phColideBallRec(&player, &map.squares[i]);
                		phColideShotRec(&player, &map.squares[i]);
                		for(j=0; j<enemys_phase; j++)
                			phColideShotRec(&map.enemies[j], &map.squares[i]);
                    }

                    if(enemys_dead == enemys_phase){
                        number_enemy = 0;
                        enemys_dead = 0;
                        enemy_type = rand() % PHASE1_ENEMYS;
                        enemys_phase+=3;
                        if(enemys_phase>map.totalEnemies)
                            enemys_phase = map.totalEnemies;
                        for(i=0; i<enemys_phase; i++)
                            initEnemy(&map.enemies[i], &map, enemy_type,map.width,map.height);
                    }

                	// Move players shots
                    phMoveShots(player.shots);

                	// Move enemies shots
                	for(i=0; i<enemys_phase; i++)
                		phMoveShots(map.enemies[i].shots);

                	// Move player
                	phMoveObject(&player);

                	// Move view area
                	moveViewPoint(&player, &view);

                	for(i=0; i<map.totalCircles; i++){
                		if((map.circles[i].life <= 0) || (player.life <= 0)){
                            for(i=0; i<10; i++){
                                if(player.score >= rank_scores[i]){
                                    for(j=9; j!=i; j--) {
                                        rank_scores[j] = rank_scores[j-1];
                                        for(k=0; k<10; k++){
                                            rank_names[j][k] = rank_names[j-1][k];
                                        }
                                    }
                                    rank_scores[i] = player.score;
                                    strcpy(rank_names[i], user.name);

                                    i = 20;
                                }
                            }

                            rank = fopen("files/rank.txt", "w");

                            for(i=0; i<10; i++){
                                if(rank_scores[i] != 0)
                                    fprintf(rank, "%s %d\n",rank_names[i],rank_scores[i]);
                            }

                            fclose(rank);
                			game.stage = STAGE_GAME_OVER;
                			//deinitMap(&map);
                		}
                	}

                	setLookDirection(&player, mouse.coordX + view.coordX, mouse.coordY + view.coordY);

                	controlAnimation(&player.aniBody, 10, 4);
                	controlAnimation(&player.aniMelee, 5, 5);
                	for(i=0; i<TOTAL_SHOTS; i++)
                		controlAnimation(&player.shots[i].aniShots, 10, 3);

                	for(i=0; i<enemys_phase; i++){
                		for(j=0; j<TOTAL_SHOTS; j++)
                			controlAnimation(&map.enemies[i].shots[j].aniShots, 10, 3);
            			controlAnimation(&map.enemies[i].aniMelee, 5, 5);
            			controlAnimation(&map.enemies[i].aniBody, 8, 2);
                	}

                    checkTimer(&player.timeAttack);
                    checkTimer(&player.timeMovement);
                    checkTimer(&player.timeElement);
                    checkTimer(&player.cantouch);
                    checkTimer(&player.timeFlame);
                    checkTimer(&player.timeExplosion);
                    for(i=0; i<enemys_phase; i++){
                    	checkTimer(&map.enemies[i].timeAttack);
                        checkTimer(&map.enemies[i].timeElement);
                        checkTimer(&map.enemies[i].cantouch);
                        checkTimer(&map.enemies[i].timeFlame);
                        checkTimer(&map.enemies[i].timeExplosion);
                    }
                    for(i=0; i<map.totalCircles; i++)
                        checkTimer(&map.circles[i].timeDemage);

                    break;
                case STAGE_PAUSE_MENU:
                	if(key[KEY_BACKSPACE]){
                		setTimer(&menuControl.time, 20);
                		game.stage = STAGE_IN_GAME;
                	}
                	if(((game.invert == 1) && (game.bgxcount > 2280)) || ((game.invert == (-1)) && (game.bgxcount < 200)))
                		game.invert *= (-1);
            		phAddAc(&game.acX, 0.2, game.invert, 1, 1, 2);
            		phAddAc(&game.acY, 0.2, game.invert, 1, 1, 2);
            		game.bgxcount += game.acX;
            		game.bgycount += game.acY;
                	break;
                case STAGE_GAME_OVER:
                	if(key[KEY_ENTER]){
                		setTimer(&menuControl.time, 20);
                		game.stage = STAGE_MAIN_MENU;
                	}
                	if(((game.invert == 1) && (game.bgxcount > 2280)) || ((game.invert == (-1)) && (game.bgxcount < 200)))
                		game.invert *= (-1);
            		phAddAc(&game.acX, 0.2, game.invert, 1, 1, 2);
            		phAddAc(&game.acY, 0.2, game.invert, 1, 1, 2);
            		game.bgxcount += game.acX;
            		game.bgycount += game.acY;
                	break;
                case STAGE_SCORE_SCREEN:
                	if(key[KEY_BACKSPACE]){
                		setTimer(&menuControl.time, 20);
                		game.stage = STAGE_MAIN_MENU;
                	}
                	if(((game.invert == 1) && (game.bgxcount > 2280)) || ((game.invert == (-1)) && (game.bgxcount < 200)))
                		game.invert *= (-1);
            		phAddAc(&game.acX, 0.2, game.invert, 1, 1, 2);
            		phAddAc(&game.acY, 0.2, game.invert, 1, 1, 2);
            		game.bgxcount += game.acX;
            		game.bgycount += game.acY;
                	break;

                	break;
                default:
                    return AL_STATE_ERROR;
                    break;
            }

            checkTimer(&menuControl);
        }

        // GAME ART / ARTE DO JOGO --------------------------------------------------------------- //
        if(al_is_event_queue_empty(eventQueue) && (settings.fpsCount == 0)){
            al_clear_to_color(al_map_rgb(0,0,0));
            switch(game.stage){
                case STAGE_INTRO_SCREEN:
                	al_draw_bitmap(sprites.logos[LOGO_IFSC], logos[LOGO_IFSC].coordX, logos[LOGO_IFSC].coordY, 0);
                	al_draw_bitmap(sprites.logos[LOGO_ALLEGRO], logos[LOGO_ALLEGRO].coordX - 250, logos[LOGO_ALLEGRO].coordY, 0);
                	al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(255,255,255), logos[2].coordX, logos[2].coordY, ALLEGRO_ALIGN_CENTRE, "Von Caio Prujansky");
                	al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(255,255,255), logos[3].coordX, logos[3].coordY, ALLEGRO_ALIGN_CENTRE, "und Johann Schmitdinger");
                	al_draw_bitmap(sprites.johann, logos[4].coordX-100, logos[4].coordY, 0);
                	al_draw_bitmap(sprites.caio, logos[5].coordX-100, logos[5].coordY, 0);
                	break;
                case STAGE_MAIN_MENU:
                	al_draw_bitmap_region(sprites.bg, game.bgxcount, game.bgycount, settings.displayX, settings.displayY, 0, 0, 0);

                	al_draw_bitmap(sprites.title, 500, 20, 0);

                	al_draw_bitmap(sprites.menuItems[START], settings.displayX*0.05, menus[STAGE_MAIN_MENU].first, 0);
                	al_draw_bitmap(sprites.menuItems[MULTI], settings.displayX*0.05, menus[STAGE_MAIN_MENU].first+1*menus[STAGE_MAIN_MENU].distancia, 0);
                	al_draw_bitmap(sprites.menuItems[LEADER], settings.displayX*0.05, menus[STAGE_MAIN_MENU].first+2*menus[STAGE_MAIN_MENU].distancia, 0);
                	al_draw_bitmap(sprites.menuItems[SETTINGS], settings.displayX*0.05, menus[STAGE_MAIN_MENU].first+3*menus[STAGE_MAIN_MENU].distancia, 0);
                	al_draw_bitmap(sprites.menuItems[QUIT], settings.displayX*0.05, menus[STAGE_MAIN_MENU].first+4*menus[STAGE_MAIN_MENU].distancia, 0);

                	al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(0,0,0), settings.displayX*0.55+3, menus[STAGE_MAIN_MENU].first
                			+ menus[STAGE_MAIN_MENU].local*menus[STAGE_MAIN_MENU].distancia+3, ALLEGRO_ALIGN_CENTRE, "<");
                	al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(0,0,0), settings.displayX*0.55+3, menus[STAGE_MAIN_MENU].first
                			+ menus[STAGE_MAIN_MENU].local*menus[STAGE_MAIN_MENU].distancia-3, ALLEGRO_ALIGN_CENTRE, "<");
                	al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(0,0,0), settings.displayX*0.55-3, menus[STAGE_MAIN_MENU].first
                			+ menus[STAGE_MAIN_MENU].local*menus[STAGE_MAIN_MENU].distancia-3, ALLEGRO_ALIGN_CENTRE, "<");
                	al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(0,0,0), settings.displayX*0.55-3, menus[STAGE_MAIN_MENU].first
                			+ menus[STAGE_MAIN_MENU].local*menus[STAGE_MAIN_MENU].distancia+3, ALLEGRO_ALIGN_CENTRE, "<");
                	al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(0,0,0), settings.displayX*0.55+3, menus[STAGE_MAIN_MENU].first
                			+ menus[STAGE_MAIN_MENU].local*menus[STAGE_MAIN_MENU].distancia, ALLEGRO_ALIGN_CENTRE, "<");
                	al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(0,0,0), settings.displayX*0.55-3, menus[STAGE_MAIN_MENU].first
                			+ menus[STAGE_MAIN_MENU].local*menus[STAGE_MAIN_MENU].distancia, ALLEGRO_ALIGN_CENTRE, "<");
                	al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(0,0,0), settings.displayX*0.55, menus[STAGE_MAIN_MENU].first
                			+ menus[STAGE_MAIN_MENU].local*menus[STAGE_MAIN_MENU].distancia+3, ALLEGRO_ALIGN_CENTRE, "<");
                	al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(0,0,0), settings.displayX*0.55, menus[STAGE_MAIN_MENU].first
                			+ menus[STAGE_MAIN_MENU].local*menus[STAGE_MAIN_MENU].distancia-3, ALLEGRO_ALIGN_CENTRE, "<");
                	al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(255,255,255), settings.displayX*0.55, menus[STAGE_MAIN_MENU].first
                			+ menus[STAGE_MAIN_MENU].local*menus[STAGE_MAIN_MENU].distancia, ALLEGRO_ALIGN_CENTRE, "<");
                    break;
                case STAGE_PLAYER_SCREEN:
                	al_draw_bitmap_region(sprites.bg, game.bgxcount, game.bgycount, settings.displayX, settings.displayY, 0, 0, 0);
                	al_draw_filled_rectangle(settings.displayX*0.28, settings.displayY*0.38, settings.displayX*0.72, settings.displayY*0.62,
                			al_map_rgb(0,0,0));
                	al_draw_filled_rectangle(settings.displayX*0.3, settings.displayY*0.4, settings.displayX*0.7, settings.displayY*0.6,
                			al_map_rgb(255,255,255));
                	al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(0,0,0), settings.displayX*0.5, settings.displayY*0.4, ALLEGRO_ALIGN_CENTRE,
                			"NAME:");
                	al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(0,0,0), settings.displayX*0.5, settings.displayY*0.5, ALLEGRO_ALIGN_CENTRE,
                			"%s", user.name);
                	break;
                case STAGE_IN_GAME:
                	// Mapa
                	al_draw_bitmap_region(sprites.bg, view.coordX, view.coordY, settings.displayX, settings.displayY, 0, 0, 0);

                	// Areas Vitais
                    for(i=0; i<map.totalCircles; i++)
                    	al_draw_bitmap(sprites.circle, map.circles[i].coordX - view.coordX - 150, map.circles[i].coordY - view.coordY - 150, 0);
                        //al_draw_filled_circle(map.circles[i].coordX - view.coordX, map.circles[i].coordY - view.coordY, map.circles[i].radius, al_map_rgb(255, 0, 0));

                    // Tiros do jogador
                    for(i=0; i<TOTAL_SHOTS; i++){
                    	if(player.shots[i].enable){
                    		al_draw_bitmap_region(sprites.projectile1, player.shots[i].aniShots.step*33, 0, 34, 34, player.shots[i].coordX - view.coordX - 17, player.shots[i].coordY - view.coordY - 17, 0);
                    		//al_draw_filled_circle(player.shots[i].coordX - view.coordX, player.shots[i].coordY - view.coordY, 5, al_map_rgb(255, 255, 255));
                    	}
                    }

                    // Tiros dos inimigos
                    for(i=0; i<enemys_phase; i++){
						for(j=0; j<TOTAL_SHOTS; j++){
							if(map.enemies[i].shots[j].enable)
								al_draw_bitmap_region(sprites.projectile2, map.enemies[i].shots[j].aniShots.step*33, 0, 34, 34, map.enemies[i].shots[j].coordX - view.coordX - 17, map.enemies[i].shots[j].coordY - view.coordY - 17, 0);
								//al_draw_filled_circle(map.enemies[i].shots[j].coordX - view.coordX, map.enemies[i].shots[j].coordY - view.coordY, 5, al_map_rgb(255, 255, 255));
						}
                    }

                    // Jogador
                    //al_draw_filled_circle(player.coordX - view.coordX, player.coordY - view.coordY, player.radius, al_map_rgb(255, 255, 255));
                    al_draw_bitmap_region(sprites.spriteHead, (!player.timeAttack.flag)*32, player.posHead*28, 32, 28, player.coordX - view.coordX - 16, player.coordY - view.coordY - 14, 0);
                    al_draw_bitmap_region(sprites.spriteBody, player.aniBody.step*22, 0, 22, 20, player.coordX - view.coordX - 11, player.coordY - view.coordY + 4, 0);
                    if((!player.timeFlame.flag) && (player.timeExplosion.flag)){
                    	al_draw_bitmap_region(sprites.spriteMelee1, player.aniMelee.step*68, 0, 69, 66, player.coordX - view.coordX - 35, player.coordY - view.coordY - 33, 0);
                    }
                    if(!player.timeExplosion.flag){
                    	al_draw_bitmap_region(sprites.spriteMelee1, player.aniMelee.step*68, 65, 69, 66, player.coordX - view.coordX - 35, player.coordY - view.coordY - 33, 0);
                    }

                    // Inimigos
                    for(i = 0; i < enemys_phase; i++){
                        if(map.enemies[i].enable){
                            //al_draw_filled_circle(map.enemies[i].coordX - view.coordX, map.enemies[i].coordY - view.coordY, map.enemies[i].radius, al_map_rgb(0, 255, 255));
                            al_draw_bitmap_region(sprites.enemy, map.enemies[i].aniBody.step * 44, map.enemies[i].posHead * 43, 44, 44,
                            		map.enemies[i].coordX - view.coordX - 22, map.enemies[i].coordY - view.coordY - 22, 0);
                            if((!map.enemies[i].timeFlame.flag) && (map.enemies[i].timeExplosion.flag)){
                            	al_draw_bitmap_region(sprites.spriteMelee2, map.enemies[i].aniMelee.step*68, 0, 69, 66, map.enemies[i].coordX - view.coordX - 35,
                            			map.enemies[i].coordY - view.coordY - 33, 0);
                            }
                        }
                        if(!map.enemies[i].timeExplosion.flag){
                        	al_draw_bitmap_region(sprites.spriteMelee2, map.enemies[i].aniMelee.step*68, 65, 69, 66, map.enemies[i].coordX - view.coordX - 35,
                        			map.enemies[i].coordY - view.coordY - 33, 0);
                        }
                    }

                    // Retangulos do jogo
                    for(i=0; i<map.totalSquares; i++){
                    	/*al_draw_rectangle(map.squares[i].coordX1 - view.coordX, map.squares[i].coordY1 - view.coordY, map.squares[i].coordX2 - view.coordX,
                    			map.squares[i].coordY2 - view.coordY, al_map_rgb(255, 255, 255), 5);*/
                    	for(j=0; j<(map.squares[i].coordX2 - map.squares[i].coordX1); j+=32){
                    		for(k=0; k<(map.squares[i].coordY2 - map.squares[i].coordY1); k+=32){
                    			al_draw_bitmap(sprites.block, map.squares[i].coordX1 - view.coordX - 16 + j, map.squares[i].coordY1 - view.coordY - 16 + k, 0);
                    		}
                    	}
                	}

                    // Limites do mapa
                	al_draw_rectangle(limits.coordX1 - view.coordX, limits.coordY1 - view.coordY,
                			limits.coordX2 - view.coordX, limits.coordY2 - view.coordY, al_map_rgb(255, 255, 255), 5);

                	// VIDA
                	al_draw_filled_rounded_rectangle(settings.displayX*0.05, settings.displayY*0.85, settings.displayX*0.45, settings.displayY*0.95,
                			10, 10, al_map_rgb(0,0,0));
                    al_draw_filled_rounded_rectangle(settings.displayX*0.06, settings.displayY*0.86, settings.displayX*0.44*(player.life/100), settings.displayY*0.94,
                    		10, 10, al_map_rgb(0,0,255));
                    al_draw_filled_rounded_rectangle(settings.displayX*0.06, settings.displayY*0.90, settings.displayX*0.44*(player.life/100), settings.displayY*0.94,
                    		10, 10, al_map_rgb(50,0,255));
                    for(i=0; i<map.totalCircles; i++){
                    	al_draw_filled_circle(settings.displayX*0.95 - i*100, settings.displayY*0.90, 42, al_map_rgb(0, 0, 0));
                    	al_draw_filled_circle(settings.displayX*0.95 - i*100, settings.displayY*0.90, 40, al_map_rgb(map.circles[i].life/100*255, 0, 0));
                    }

                    // PONTUACAO
                    al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(0,0,0), settings.displayX*0.5+2, settings.displayY*0.02+2, ALLEGRO_ALIGN_CENTRE,
                    		"%d", player.score);
                    al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(0,0,0), settings.displayX*0.5+2, settings.displayY*0.02-2, ALLEGRO_ALIGN_CENTRE,
                    		"%d", player.score);
                    al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(0,0,0), settings.displayX*0.5-2, settings.displayY*0.02-2, ALLEGRO_ALIGN_CENTRE,
                    		"%d", player.score);
                    al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(0,0,0), settings.displayX*0.5-2, settings.displayY*0.02+3, ALLEGRO_ALIGN_CENTRE,
                    		"%d", player.score);
                    al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(0,0,0), settings.displayX*0.5+2, settings.displayY*0.02, ALLEGRO_ALIGN_CENTRE,
                    		"%d", player.score);
                    al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(0,0,0), settings.displayX*0.5-2, settings.displayY*0.02, ALLEGRO_ALIGN_CENTRE,
                    		"%d", player.score);
                    al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(0,0,0), settings.displayX*0.5, settings.displayY*0.02+2, ALLEGRO_ALIGN_CENTRE,
                    		"%d", player.score);
                    al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(0,0,0), settings.displayX*0.5, settings.displayY*0.02-2, ALLEGRO_ALIGN_CENTRE,
                    		"%d", player.score);
                    al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(255,255,255), settings.displayX*0.5, settings.displayY*0.02, ALLEGRO_ALIGN_CENTRE,
                    		"%d", player.score);

                	// Cursor
                    al_draw_filled_circle(mouse.coordX, mouse.coordY, 5, al_map_rgb(255, 255, 255));

                    break;
                case STAGE_PAUSE_MENU:
                	al_draw_bitmap_region(sprites.bg, game.bgxcount, game.bgycount, settings.displayX, settings.displayY, 0, 0, 0);
                	al_draw_textf(font[FONT_ARIAL][FONT_16], al_map_rgb(255,255,255), settings.displayX/2,
                			settings.displayY/2, ALLEGRO_ALIGN_CENTRE, "PAUSED");
                	break;
                case STAGE_GAME_OVER:
                	al_draw_bitmap_region(sprites.bg, game.bgxcount, game.bgycount, settings.displayX, settings.displayY, 0, 0, 0);

                	al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(0,0,0), settings.displayX/2+2,
                			settings.displayY*0.1+2, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
                	al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(0,0,0), settings.displayX/2+2,
                			settings.displayY*0.1-2, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
                	al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(0,0,0), settings.displayX/2-2,
                			settings.displayY*0.1-2, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
                	al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(0,0,0), settings.displayX/2-2,
                			settings.displayY*0.1+2, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
                	al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(0,0,0), settings.displayX/2+2,
                			settings.displayY*0.1, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
                	al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(0,0,0), settings.displayX/2-2,
                			settings.displayY*0.1, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
                	al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(0,0,0), settings.displayX/2,
                			settings.displayY*0.1+2, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
                	al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(0,0,0), settings.displayX/2,
                			settings.displayY*0.1-1, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
                	al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(255,255,255), settings.displayX/2,
                			settings.displayY*0.1, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
                	al_draw_filled_rounded_rectangle(settings.displayX*0.19, settings.displayY*0.24, settings.displayX*0.81, settings.displayY*0.91,
                			10, 10, al_map_rgb(0,0,0));
                	al_draw_filled_rounded_rectangle(settings.displayX*0.2, settings.displayY*0.25, settings.displayX*0.8, settings.displayY*0.9,
                			10, 10, al_map_rgb(255,255,255));

                	for(i=0; i<10; i++){
                		al_draw_textf(font[FONT_ARIAL][FONT_24], al_map_rgb(0,0,0), settings.displayX*0.22,
                				settings.displayY*0.3+(i*25), ALLEGRO_ALIGN_LEFT, "%s", rank_names[i]);
                		al_draw_textf(font[FONT_ARIAL][FONT_24], al_map_rgb(0,0,0), settings.displayX*0.60,
                				settings.displayY*0.3+(i*25), ALLEGRO_ALIGN_LEFT, "%d", rank_scores[i]);
                	}
                	break;
                case STAGE_SCORE_SCREEN:
                	al_draw_bitmap_region(sprites.bg, game.bgxcount, game.bgycount, settings.displayX, settings.displayY, 0, 0, 0);

                	al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(0,0,0), settings.displayX/2+2,
                			settings.displayY*0.1+2, ALLEGRO_ALIGN_CENTRE, "TOP SCORES");
                	al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(0,0,0), settings.displayX/2+2,
                			settings.displayY*0.1-2, ALLEGRO_ALIGN_CENTRE, "TOP SCORES");
                	al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(0,0,0), settings.displayX/2-2,
                			settings.displayY*0.1-2, ALLEGRO_ALIGN_CENTRE, "TOP SCORES");
                	al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(0,0,0), settings.displayX/2-2,
                			settings.displayY*0.1+2, ALLEGRO_ALIGN_CENTRE, "TOP SCORES");
                	al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(0,0,0), settings.displayX/2+2,
                			settings.displayY*0.1, ALLEGRO_ALIGN_CENTRE, "TOP SCORES");
                	al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(0,0,0), settings.displayX/2-2,
                			settings.displayY*0.1, ALLEGRO_ALIGN_CENTRE, "TOP SCORES");
                	al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(0,0,0), settings.displayX/2,
                			settings.displayY*0.1+2, ALLEGRO_ALIGN_CENTRE, "TOP SCORES");
                	al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(0,0,0), settings.displayX/2,
                			settings.displayY*0.1-1, ALLEGRO_ALIGN_CENTRE, "TOP SCORES");
                	al_draw_textf(font[FONT_ARIAL][FONT_34], al_map_rgb(255,255,255), settings.displayX/2,
                			settings.displayY*0.1, ALLEGRO_ALIGN_CENTRE, "TOP SCORES");
                	al_draw_filled_rounded_rectangle(settings.displayX*0.19, settings.displayY*0.24, settings.displayX*0.81, settings.displayY*0.91,
                			10, 10, al_map_rgb(0,0,0));
                	al_draw_filled_rounded_rectangle(settings.displayX*0.2, settings.displayY*0.25, settings.displayX*0.8, settings.displayY*0.9,
                			10, 10, al_map_rgb(255,255,255));

                	for(i=0; i<10; i++){
                		al_draw_textf(font[FONT_ARIAL][FONT_24], al_map_rgb(0,0,0), settings.displayX*0.22,
                				settings.displayY*0.3+(i*25), ALLEGRO_ALIGN_LEFT, "%s", rank_names[i]);
                		al_draw_textf(font[FONT_ARIAL][FONT_24], al_map_rgb(0,0,0), settings.displayX*0.60,
                				settings.displayY*0.3+(i*25), ALLEGRO_ALIGN_LEFT, "%d", rank_scores[i]);
                	}
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
    al_destroy_bitmap(sprites.bg);
    al_destroy_bitmap(sprites.block);
    al_destroy_bitmap(sprites.circle);
    al_destroy_bitmap(sprites.enemy);
    al_destroy_bitmap(sprites.projectile1);
    al_destroy_bitmap(sprites.projectile2);
    al_destroy_bitmap(sprites.spriteBody);
    al_destroy_bitmap(sprites.spriteHead);
    al_destroy_bitmap(sprites.spriteMelee1);
    al_destroy_bitmap(sprites.spriteMelee2);
    al_destroy_bitmap(sprites.logos[LOGO_IFSC]);
    al_destroy_bitmap(sprites.logos[LOGO_ALLEGRO]);
    al_destroy_bitmap(sprites.title);
    for(i=0; i<TOTAL_ITEMS; i++)
    	al_destroy_bitmap(sprites.menuItems[i]);
    al_destroy_sample(samples.gameBackground);
    al_destroy_sample(samples.hit);
    al_destroy_sample(samples.menuBackground);
    al_destroy_sample(samples.menuSelect);
    al_destroy_sample(samples.type);

    return SABABA;
}
