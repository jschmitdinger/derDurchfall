/* ======================================= DER DURCHFALL ========================================== //
| INSTITUTO FEDERAL DE SANTA CATARINA - IFSC		FEDERAL INSTITUTE OF SANTA CATARINA				||
| DISCIPLINA DE PROGRAMACAO C						C PROGRAMMING LANGUAGE							||
| PROFESSOR FERNANDO PACHECO						PROFESSOR FERNANDO PACHECO						||
| CAIO PRUJANSKY E JOHANN SCHMITDINGER				CAIO PRUJANSKY E JOHANN SCHMITDINGER			||
| ARQUIVO: durchfallDefines.h						FILE: durchfallDefines.h						||
| VERSAO: v1.0										VERSION: v1.0									||
| DATA DE ALTERACAO: 02/06/2015						LAST ALTERED DATE: 02/06/2015					||
// ================================================================================================ */

#ifndef DURCHFALLDEFINES_H_
#define DURCHFALLDEFINES_H_

// ??? ============================================================================================ //
#define SABABA 							0
#define MUITOS                          10000

// MACRO FUNCTIONS ================================================================================ //
#define DISTANCE(x1,y1,x2,y2)			sqrt(pow((x1 - x2),2)+(pow((y1 - y2),2)))

// SYSTEM MACROS ================================================================================== //
#define GAME_SPEED						120

// ERROR CODES ==================================================================================== //
// INITIALIZATION CODES --------------------------------------------------------------------------- //
#define AL_FILE_ERROR					9100
#define AL_INIT_ERROR                   9101
#define AL_DISPLAY_ERROR                9102
#define AL_PRIMITIVE_ERROR              9103
#define AL_KEYBOARD_ERROR               9104
#define AL_EVENT_QUEUE_ERROR            9105
#define AL_TIMER_ERROR                  9106
#define AL_FONT_ERROR                   9107
#define AL_STATE_ERROR                  9108
#define AL_IMAGE_ERROR                  9109
#define AL_FILE_CONFIG_ERROR			9110
#define AL_MOUSE_ERROR					9111

// KEY DEFINITIONS ================================================================================ //
#define TOTAL_KEY                       50
#define KEY_A                           0
#define KEY_B                           1
#define KEY_C                           2
#define KEY_D                           3
#define KEY_E                           4
#define KEY_F                           5
#define KEY_G                           6
#define KEY_H                           7
#define KEY_I                           8
#define KEY_J                           9
#define KEY_K                           10
#define KEY_L                           12
#define KEY_M                           13
#define KEY_N                           14
#define KEY_O                           15
#define KEY_P                           16
#define KEY_Q                           17
#define KEY_R                           18
#define KEY_S                           19
#define KEY_T                           20
#define KEY_U                           21
#define KEY_V                           22
#define KEY_W                           23
#define KEY_X                           24
#define KEY_Y                           25
#define KEY_Z                           26
#define KEY_UP                          27
#define KEY_DOWN                        28
#define KEY_LEFT                        29
#define KEY_RIGHT                       30
#define KEY_ESC                         31
#define KEY_ENTER                       32
#define KEY_SHIFT                       33
#define KEY_BACKSPACE					34

// MOUSE DEFINITIONS ============================================================================== //
#define MOUSE_LEFT						0
#define MOUSE_RIGHT						1
#define MOUSE_MIDDLE					2

// MENU ITEMS ===================================================================================== //
#define TOTAL_ITEMS						5
#define START							0
#define MULTI							1
#define LEADER							2
#define SETTINGS						3
#define QUIT							4

#define LOGO_IFSC						0
#define LOGO_ALLEGRO					1

// FONT DEFINITIONS =============================================================================== //
#define FONT_NAME                       1
#define FONT_SIZES                      5
#define FONT_ARIAL                      0
#define FONT_CENTAUR                    1
#define FONT_STENCIL                    2
#define FONT_16                         0
#define FONT_24                         1
#define FONT_32							2
#define FONT_34							3
#define FONT_26                         4

// GAME STAGES =================================================================================== //
#define STAGE_INTRO_SCREEN              0
#define STAGE_MAIN_MENU                 1
#define STAGE_IN_GAME                   2
#define STAGE_PAUSE_MENU				3
#define STAGE_GAME_OVER					4
#define STAGE_SCORE_SCREEN				5
#define STAGE_SETTINGS_SCREEN			6
#define STAGE_PLAYER_SCREEN				7

// PLAYER DEFINITIONS ============================================================================ //
#define TYPE_NORMAL						0
#define ATCK_MELEE						0
#define ATCK_SHOOT						1
#define ATCK_MOOVE						2
#define NORMAL_SPEED					5
#define TACKLE_SPEED					10
#define TOTAL_SHOTS						100
#define BULLET_SPEED					5

#define ELEMENT_UN						0
#define ELEMENT_NORMAL					1
#define DAMEGE_CONST					10

#define CONST_DISTANCE					200
#define CONST_DISTANCE_FAR				500

// ENEMYS DEFINITIONS ============================================================================ //
#define PHASE1_ENEMYS                   4
#define ENEMY_NORMAL                    0
#define ENEMY_PHASE2                    1
#define ENEMY_PHASE3                    2
#define ENEMY_PHASE4                    3
#define ENEMY_SPEED                     3

// MAP DEFINITIONS =============================================================================== //
#define MAP_SQUARES_MAX					100

// PHYSICS DEFINITIONS =========================================================================== //
#define CONST_POWER						0.2

#endif /* DURCHFALLDEFINES_H_ */
