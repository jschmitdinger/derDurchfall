
/* ======================================= DER DURCHFALL ========================================== //
| INSTITUTO FEDERAL DE SANTA CATARINA - IFSC		FEDERAL INSTITUTE OF SANTA CATARINA				||
| DISCIPLINA DE PROGRAMACAO C						C PROGRAMMING LANGUAGE							||
| PROFESSOR FERNANDO PACHECO						PROFESSOR FERNANDO PACHECO						||
| CAIO PRUJANSKY E JOHANN SCHMITDINGER				CAIO PRUJANSKY E JOHANN SCHMITDINGER			||
| ARQUIVO: durchfallEngine.c						FILE: durchfallEngine.c							||
| VERSAO: v1.0										VERSION: v1.0									||
| DATA DE ALTERACAO: 02/06/2015						LAST ALTERED DATE: 02/06/2015					||
// ================================================================================================ */

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <allegro5/allegro.h>
#include "durchfallDefines.h"
#include "durchfallStructs.h"
#include "durchfallEngine.h"

// GRAPHIC CONTROL FUNCTIONS ====================================================================== //
int loadSettings(graphicSettings *settings)
{
	settings->fp = fopen("files/config.txt", "r");
	if(!settings->fp)
		return 0;
	fscanf(settings->fp, "%s", settings->st);
	settings->displayX = atoi(settings->st);
	fscanf(settings->fp, "%s", settings->st);
	settings->displayY = atoi(settings->st);
	fscanf(settings->fp, "%s", settings->st);
	settings->fps = GAME_SPEED/atoi(settings->st);
	if((settings->fps != 2) && (settings->fps != 4) && (settings->fps != 6) && (settings->fps != 12))
		return 0;
	settings->fpsCount = 0;

	return 1;
}
void fpsControl(graphicSettings *settings)
{
	if(settings->fpsCount < (settings->fps-1))
		settings->fpsCount++;
	else
		settings->fpsCount = 0;

	return;
}

// TIME CONTROL FUNCTIONS ========================================================================= //
void setTimer(int *timer, int time)
{
	if(time > (*timer))
		*timer = time;
	return;
}
void checkTimer(timeControl *control)
{

	if(control->time > 0){
		control->time -= 1;
		control->flag = 0;
	} else{
		control->flag = 1;
	}

	return;
}

// PHYSICS FUNCTIONS ============================================================================== //
void phAddAc(float *ac, float power, char dir, float mass, float trig, float limit)
{
	if(dir > 0){
		if((*ac+power) < limit)
			*ac += (power * mass * trig);
		else
			*ac = limit;
	} else{
		if((*ac-power) > (-limit))
			*ac -= (power * mass * trig);
		else
			*ac = (-limit);
	}

	return;
}
void phNormalize(float *ac, float power, float mass)
{
	if(*ac > 0)
		phAddAc(ac, power, -1, mass, 1, 0);
	else
		phAddAc(ac, power, 1, mass, 1, 0);

	return;
}
void phMoveObject(avatar *object)
{
	object->coordX += object->acX;
	object->coordY += object->acY;

	object->dAngle = atan(object->acY/object->acX);

	return;
}
void phColideBallRec(avatar *object, square *block)
{
	if((object->coordX > block->coordX1) && (object->coordX < block->coordX2) && (object->coordY > block->coordY1) &&
			(object->coordY < block->coordY2)){
		// COLISION FROM INSIDE
		if(((object->coordX - object->radius) <= block->coordX1) && ((object->coordX + object->radius) < block->coordX2)
				&& ((object->coordY + object->radius) < block->coordY2) && ((object->coordY - object->radius) > block->coordY1)
				&& block->flagA){
			phAddAc(&object->acX, 2*object->acX, -1, object->weight, 1, NORMAL_SPEED);
			object->coordX = block->coordX1 + object->radius + 1;
		}
		if(((object->coordX + object->radius) >= block->coordX2) && ((object->coordX - object->radius) > block->coordX1)
				&& ((object->coordY + object->radius) < block->coordY2) && ((object->coordY - object->radius) > block->coordY1)
				&& block->flagB){
			phAddAc(&object->acX, 2*object->acX, -1, object->weight, 1, NORMAL_SPEED);
			object->coordX = block->coordX2 - object->radius - 1;
		}
		if(((object->coordX - object->radius) > block->coordX1) && ((object->coordX + object->radius) < block->coordX2)
				&& ((object->coordY - object->radius) <= block->coordY1) && ((object->coordY + object->radius) < block->coordY2)
				&& block->flagC){
			phAddAc(&object->acY, 2*object->acY, -1, object->weight, 1, NORMAL_SPEED);
			object->coordY = block->coordY1 + object->radius + 1;
		}
		if(((object->coordX - object->radius) > block->coordX1) && ((object->coordX + object->radius) < block->coordX2)
				&& ((object->coordY + object->radius) >= block->coordY2) && ((object->coordY - object->radius) > block->coordY1)
				&& block->flagD){
			phAddAc(&object->acY, 2*object->acY, -1, object->weight, 1, NORMAL_SPEED);
			object->coordY = block->coordY2 - object->radius - 1;
		}
	} else{
		// COLISION FROM OUTSIDE
		if(((object->coordX + object->radius) < block->coordX2) && ((object->coordX + object->radius) >= block->coordX1)
				&& ((object->coordY /*+ object->radius*/) < block->coordY2) && ((object->coordY /*- object->radius*/) > block->coordY1)
				&& block->flagA){
			phAddAc(&object->acX, 2*object->acX, -1, object->weight, 1, NORMAL_SPEED);
			object->coordX = block->coordX1 - object->radius - 1;
		}
		if(((object->coordX - object->radius) > block->coordX1) && ((object->coordX - object->radius) <= block->coordX2)
				&& ((object->coordY /*+ object->radius*/) < block->coordY2) && ((object->coordY /*- object->radius*/) > block->coordY1)
				&& block->flagB){
			phAddAc(&object->acX, 2*object->acX, -1, object->weight, 1, NORMAL_SPEED);
			object->coordX = block->coordX2 + object->radius + 1;
		}
		if(((object->coordX /*- object->radius*/) > block->coordX1) && ((object->coordX /*+ object->radius*/) < block->coordX2)
				&& ((object->coordY + object->radius) < block->coordY2) && ((object->coordY + object->radius) >= block->coordY1)
				&& block->flagC){
			phAddAc(&object->acY, 2*object->acY, -1, object->weight, 1, NORMAL_SPEED);
			object->coordY = block->coordY1 - object->radius - 1;
		}
		if(((object->coordX /*- object->radius*/) > block->coordX1) && ((object->coordX /*+ object->radius*/) < block->coordX2)
				&& ((object->coordY - object->radius) > block->coordY1) && ((object->coordY - object->radius) <= block->coordY2)
				&& block->flagD){
			phAddAc(&object->acY, 2*object->acY, -1, object->weight, 1, NORMAL_SPEED);
			object->coordY = block->coordY2 + object->radius + 1;
		}
		if(((object->coordX + object->radius) > block->coordX1) && ((object->coordY + object->radius) > block->coordY1) &&
				(object->coordX < block->coordX2) && (object->coordY < block->coordY2) && block->flagA && block->flagC){
			phAddAc(&object->acX, 2*object->acX, -1, object->weight, 1, NORMAL_SPEED);
			phAddAc(&object->acY, 2*object->acY, -1, object->weight, 1, NORMAL_SPEED);
			object->coordX = block->coordX1 - object->radius - 1;
			object->coordY = block->coordY1 - object->radius - 1;
		}
		if(((object->coordX + object->radius) > block->coordX1) && ((object->coordY - object->radius) < block->coordY2) &&
				(object->coordX < block->coordX2) && (object->coordY > block->coordY1) && block->flagA && block->flagD){
			phAddAc(&object->acX, 2*object->acX, -1, object->weight, 1, NORMAL_SPEED);
			phAddAc(&object->acY, 2*object->acY, -1, object->weight, 1, NORMAL_SPEED);
			object->coordX = block->coordX1 - object->radius - 1;
			object->coordY = block->coordY2 + object->radius + 1;
		}
		if(((object->coordX - object->radius) < block->coordX2) && ((object->coordY + object->radius) > block->coordY1) &&
				(object->coordX > block->coordX1) && (object->coordY < block->coordY2) && block->flagB && block->flagC){
			phAddAc(&object->acX, 2*object->acX, -1, object->weight, 1, NORMAL_SPEED);
			phAddAc(&object->acY, 2*object->acY, -1, object->weight, 1, NORMAL_SPEED);
			object->coordX = block->coordX2 + object->radius + 1;
			object->coordY = block->coordY1 - object->radius - 1;
		}
		if(((object->coordX - object->radius) < block->coordX2) && ((object->coordY - object->radius) < block->coordY2) &&
				(object->coordX > block->coordX1) && (object->coordY > block->coordY1) && block->flagB && block->flagD){
			phAddAc(&object->acX, 2*object->acX, -1, object->weight, 1, NORMAL_SPEED);
			phAddAc(&object->acY, 2*object->acY, -1, object->weight, 1, NORMAL_SPEED);
			object->coordX = block->coordX2 + object->radius + 1;
			object->coordY = block->coordY2 + object->radius + 1;
		}
	}

	return;
}
void phColideBallLine(avatar *object, float lineX0, float lineY0, float lineX1, float lineY1)
{
	float a, b, c;
	float distance;
	float lineAngle, objAngle, colAngle, refAngle;
	float bigY, smawY, bigX, smawX;
	int objDir;
	float hip;

	a = lineY0 - lineY1;
	b = lineX1 - lineX0;
	c = (lineX0 * lineY1) - (lineX1 * lineY0);

	lineAngle = atan((lineY1 - lineY0)/(lineX1 - lineX0)) * (-1);
	objAngle = atan(object->acY/object->acX) * (-1);

	hip = sqrt((object->acX*object->acX) + (object->acY*object->acY));

	distance = ((a*object->coordX)+(b*object->coordY)+c)/sqrt((a*a)+(b*b));

	if(lineX1 > lineX0){
		bigX = lineX1;
		smawX = lineX0;
	} else{
		bigX = lineX0;
		smawX = lineX1;
	}
	if(lineY1 > lineY0){
		bigY = lineY1;
		smawY = lineY0;
	} else{
		bigY = lineY0;
		smawY = lineY1;
	}

	if((((object->coordX + object->radius) <= bigX) || ((object->coordX - object->radius)) >= smawX) && (((object->coordY + object->radius) <= bigY) || ((object->coordY - object->radius) >= smawY))){
		if((((distance-object->radius) <= 0) && ((distance+object->radius) > 0)) || (((distance+object->radius) >= 0) && ((distance+object->radius) < 0))){
			colAngle = lineAngle - objAngle;
			refAngle = colAngle + lineAngle;
			if(!isnan(refAngle)){
				object->acX = cos(refAngle) * hip;
				object->acY = sin(refAngle) * hip;
			}
			if((refAngle > 0) && (refAngle <= (M_PI/2))){
				if(object->acX < 0)
					object->acX *= (-1);
				if(object->acY > 0)
					object->acY *= (-1);
			}
			else if((refAngle > (M_PI/2)) && (refAngle <= M_PI)){
				if(object->acX > 0)
					object->acX *= (-1);
				if(object->acY > 0)
					object->acY *= (-1);
			}
			else if((refAngle > M_PI) && (refAngle <= (M_PI*3/2))){
				if(object->acX > 0)
					object->acX *= (-1);
				if(object->acY < 0)
					object->acY *= (-1);
			}
			else if((refAngle > (M_PI*3/2)) && (refAngle <= (2*M_PI))){
				if(object->acX < 0)
					object->acX *= (-1);
				if(object->acY < 0)
					object->acY *= (-1);
			}
		}
	}

	return;
}
// MAP FUNCTIONS ================================================================================== //
void initMap(gameMap *map, char filePath[])
{
	map->fileMap = fopen(filePath, "r");
	fscanf(map->fileMap, "%s", map->st);
	map->width = atoi(map->st);
	fscanf(map->fileMap, "%s", map->st);
	map->height = atoi(map->st);

	return;
}
void moveViewPoint(avatar *reference, mapView *view)
{
	if(((reference->coordX - view->coordX) <= (view->rangeX * 0.2)) && (view->coordX > 0))
		view->coordX += reference->acX;
	if((reference->coordX - view->coordX) >= (view->rangeX * 0.8) && (view->coordX < (view->totalWidth - view->rangeX)))
		view->coordX += reference->acX;
	if((reference->coordY - view->coordY) <= (view->rangeY * 0.2) && (view->coordY > 0))
		view->coordY += reference->acY;
	if((reference->coordY - view->coordY) >= (view->rangeY * 0.8) && (view->coordY < (view->totalHeight - view->rangeY)))
		view->coordY += reference->acY;

	return;
}

// AVATAR FUNCTIONS =============================================================================== //
int initPlayer(avatar *player, char type, int arenaWidth, int arenaHeight)
{
	player->acX = 0;
	player->acY = 0;
	player->coordX = arenaWidth/2;
	player->coordY = arenaHeight/2;

	switch(type){
	case TYPE_NORMAL:
		player->radius = 15;
		player->power = 0.2;
		player->weight = 15/player->radius;
		break;
	default:
		return 0;
		break;
	}

	return 1;
}