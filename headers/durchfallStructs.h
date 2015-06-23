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

typedef struct avatar{
	float coordX;
	float coordY;
	float radius;
	float acX;
	float acY;
	float power;
	float weight;
	float dAngle;
	int life;
	char enable;
	timeControl timeMovement;
	timeControl timeAttack;
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

#endif /* DURCHFALLSTRUCTS_H_ */
