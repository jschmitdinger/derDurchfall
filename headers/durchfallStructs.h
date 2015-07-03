/* ======================================= DER DURCHFALL ========================================== //
| INSTITUTO FEDERAL DE SANTA CATARINA - IFSC		FEDERAL INSTITUTE OF SANTA CATARINA				||
| DISCIPLINA DE PROGRAMACAO C						C PROGRAMMING LANGUAGE							||
| PROFESSOR FERNANDO PACHECO						PROFESSOR FERNANDO PACHECO						||
| CAIO PRUJANSKY E JOHANN SCHMITDINGER				CAIO PRUJANSKY E JOHANN SCHMITDINGER			||
| ARQUIVO: durchfallStructs.h						FILE: durchfallStructs.h						||
| VERSAO: v1.0										VERSION: v1.0									||
| DATA DE ALTERACAO: 02/06/2015						LAST ALTERED DATE: 02/06/2015					||
// ================================================================================================ */

#ifndef DURCHFALLSTRUCTS_H_
#define DURCHFALLSTRUCTS_H_

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

typedef struct logo{
	float acX;
	float acY;
	float coordX;
	float coordY;
} logo;

typedef struct sounds{
	ALLEGRO_SAMPLE_ID *bkg;
	ALLEGRO_SAMPLE *menuBackground;
	ALLEGRO_SAMPLE *menuSwitch;
	ALLEGRO_SAMPLE *menuSelect;
	ALLEGRO_SAMPLE *type;
	ALLEGRO_SAMPLE *gameBackground;
	ALLEGRO_SAMPLE *shotp;
	ALLEGRO_SAMPLE *shote;
	ALLEGRO_SAMPLE *damagep;
	ALLEGRO_SAMPLE *firee;
	ALLEGRO_SAMPLE *firep;
	ALLEGRO_SAMPLE *hit;
} sounds;

typedef struct spritsCrianca{
	ALLEGRO_BITMAP *logos[2];
	ALLEGRO_BITMAP *menuItems[5];
	ALLEGRO_BITMAP *projectile1;
	ALLEGRO_BITMAP *projectile2;
	ALLEGRO_BITMAP *spriteHead;
	ALLEGRO_BITMAP *spriteBody;
	ALLEGRO_BITMAP *spriteMelee1;
	ALLEGRO_BITMAP *spriteMelee2;
	ALLEGRO_BITMAP *circle;
	ALLEGRO_BITMAP *bg;
	ALLEGRO_BITMAP *block;
	ALLEGRO_BITMAP *enemy;
	ALLEGRO_BITMAP *title;
	ALLEGRO_BITMAP *caio;
	ALLEGRO_BITMAP *johann;
} spritsCrianca;

typedef struct menuOptionControl{
	int local;
	int distancia;
	int first;
	int options;
} menuOptionControl;

typedef struct graphicSettings{
	FILE *fp;
	char st[4];
	int displayX;
	int displayY;
	int fps;
	int fpsCount;
} graphicSettings;

typedef struct gameStatus{
	char quit;
	char stage;
	float bgxcount;
	float bgycount;
	char invert;
	float acX;
	float acY;
	int menuChangeTimer;
} gameStatus;

typedef struct cursor{
	int coordX;
	int coordY;
	char btn[3];
} cursor;

typedef struct timeControl{
	char flag;
	int time;
} timeControl;

typedef struct aniControl{
	int step;
	int time;
} aniControl;

typedef struct projectile{
	int coordX;
	int coordY;
	int acX;
	int acY;
	int enable;
	aniControl aniShots;
} projectile;

typedef struct avatar{
	int score;
	float coordX;
	float coordY;
	float radius;
	float acX;
	float acY;
	float power;
	float weight;
	float dAngle;
	float life;
	char enable;
	int stepCounter;
	int objective;
	int shotCount;
	int delay[3];
	float distanceToPlayer;
	int posHead;
	timeControl timeElement;
	timeControl timeMovement;
	timeControl timeAttack;
	timeControl cantouch;
	timeControl timeFlame;
	timeControl timeExplosion;
	aniControl aniBody;
	aniControl aniMelee;
	projectile shots[TOTAL_SHOTS];
} avatar;

typedef struct square{
	int coordX1;
	int coordX2;
	int coordY1;
	int coordY2;
	char flagA;
	char flagB;
	char flagC;
	char flagD;
	char enable;
} square;

typedef struct triangle{
	int coordX1;
	int coordY1;
	int coordX2;
	int coordY2;
	int coordX3;
	int coordY3;
	char enable;
} triangle;

typedef struct circle{
	int coordX;
	int coordY;
	int radius;
	char enable;
	int totalSteps;
	int *stepX;
	int *stepY;
	int stepCounter;
	float life;
	timeControl timeDemage;
} circle;

typedef struct line{
	int coordX1;
	int coordY1;
	int coordX2;
	int coordY2;
	char enable;
} line;

typedef struct gameMap{
	FILE *fileMap;
	int height;
	int width;
	int totalSquares;
	int totalLines;
	int totalTriangles;
	int totalCircles;
	int totalEnemies;
	char st[100];
	square *squares;
	triangle *triangles;
	line *lines;
	circle *circles;
	avatar *enemies;
} gameMap;

typedef struct mapView{
	float totalWidth;
	float totalHeight;
	float rangeX;
	float rangeY;
	float coordX;
	float coordY;
} mapView;

typedef struct profile{
    char name[10];
    int score;
}profile;

#endif /* DURCHFALLSTRUCTS_H_ */
