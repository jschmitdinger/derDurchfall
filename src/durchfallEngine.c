
/* ======================================= DER DURCHFALL ========================================== //
| INSTITUTO FEDERAL DE SANTA CATARINA - IFSC		FEDERAL INSTITUTE OF SANTA CATARINA				||
| DISCIPLINA DE PROGRAMACAO C						C PROGRAMMING LANGUAGE							||
| PROFESSOR FERNANDO PACHECO						PROFESSOR FERNANDO PACHECO						||
| CAIO PRUJANSKY E JOHANN SCHMITDINGER				CAIO PRUJANSKY E JOHANN SCHMITDINGER			||
| ARQUIVO: durchfallEngine.c						FILE: durchfallEngine.c							||
| VERSAO: v1.0										VERSION: v1.0									||
| DATA DE ALTERACAO: 02/06/2015						LAST ALTERED DATE: 02/06/2015					||
// ================================================================================================ 00*/

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
	if(!settings->fp){
		printf("\nNao encontrei o arquivo");
		return 0;
	}
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
void phMoveEnemy(avatar *enemy, circle *vital)
{

    if(vital->coordX - enemy->coordX > 0)
    {
        phAddAc(&enemy->acX,enemy->power,1,enemy->weight,1,NORMAL_SPEED);
    }
    if(vital->coordY - enemy->coordY > 0)
    {
        phAddAc(&enemy->acY,enemy->power,1,enemy->weight,1,NORMAL_SPEED);
    }
    if(vital->coordX - enemy->coordX < 0)
    {
        phAddAc(&enemy->acX,enemy->power,-1,enemy->weight,1,NORMAL_SPEED);
    }
    if(vital->coordY - enemy->coordY < 0)
    {
        phAddAc(&enemy->acY,enemy->power,-1,enemy->weight,1,NORMAL_SPEED);
    }
    phMoveObject(enemy);
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
void phColide2Ball(avatar *object1, avatar *object2)
{
    float distance;
    float auxX;
    float auxY;

    distance = sqrt(pow((object1->coordX - object2->coordX),2)+(pow((object1->coordY - object2->coordY),2)));

    if(distance <= (object1->radius + object2->radius))
    {
        if(distance < (object1->radius + object2->radius))
        {
            if(object1->coordX < object2->coordX)
            {
                object1->coordX = object1->coordX - (object1->radius + object2->radius - distance)/2;
                object2->coordX = object2->coordX + (object1->radius + object2->radius - distance)/2;
            }

            if(object1->coordX >= object2->coordX)
            {
                object1->coordX = object1->coordX + (object1->radius + object1->radius - distance)/2;
                object2->coordX = object2->coordX - (object1->radius + object2->radius - distance)/2;
            }
            if(object1->coordY < object2->coordY)
            {
                object1->coordY = object1->coordY - (object1->radius + object2->radius - distance)/2;
                object2->coordY = object2->coordY + (object1->radius + object2->radius - distance)/2;
            }

            if(object1->coordY >= object2->coordY)
            {
                object1->coordY = object1->coordY + (object1->radius + object1->radius - distance)/2;
                object2->coordY = object2->coordY - (object1->radius + object2->radius - distance)/2;
            }
        }

        phAddAc(&object1->acX, object1->acX*1.5,-1,object1->weight,1,NORMAL_SPEED);
        phAddAc(&object1->acY, object1->acY*1.5,-1,object1->weight,1,NORMAL_SPEED);

        phAddAc(&object2->acX, object2->acX*1.5,-1,object2->weight,1,NORMAL_SPEED);
        phAddAc(&object2->acY, object2->acY*1.5,-1,object2->weight,1,NORMAL_SPEED);

        auxX = object2->acX;
        object2->acX = object1->acX*-2;
        object1->acX = auxX*-2;

        auxY = object2->acY;
        object2->acY = object1->acY*-2;
        object1->acY = auxY*-2;
    }
    return;

}
// MAP FUNCTIONS ================================================================================== //
void initMap(gameMap *map, char filePath[])
{
	int i, type;
	int countS = 0, countL = 0, countT = 0, countC = 0;

	map->fileMap = fopen(filePath, "r");
	if(!map->fileMap)
		printf("\nNao abri o arquivo map.txt");
	fflush(stdout);
	fscanf(map->fileMap, "%s", map->st);
	map->width = atoi(map->st);
	fscanf(map->fileMap, "%s", map->st);
	map->height = atoi(map->st);
	fscanf(map->fileMap, "%s", map->st);
	map->totalSquares = atoi(map->st);
	fscanf(map->fileMap, "%s", map->st);
	map->totalLines = atoi(map->st);;
	fscanf(map->fileMap, "%s", map->st);
	map->totalTriangles = atoi(map->st);
	fscanf(map->fileMap, "%s", map->st);
	map->totalCircles = atoi(map->st);

	map->squares = (square*) malloc(map->totalSquares*sizeof(square));
	map->lines = (line*) malloc(map->totalLines*sizeof(line));
	map->triangles = (triangle*) malloc(map->totalTriangles*sizeof(triangle));
	map->circles = (circle*) malloc(map->totalCircles*sizeof(circle));

	for(i=0; i<(map->totalSquares+map->totalLines+map->totalTriangles+map->totalCircles); i++){
		fscanf(map->fileMap, "%s", map->st);
		type = atoi(map->st);
		switch(type){
		case 0:
			fscanf(map->fileMap, "%d", &map->squares[countS].coordX1);
			fscanf(map->fileMap, "%d", &map->squares[countS].coordY1);
			fscanf(map->fileMap, "%d", &map->squares[countS].coordX2);
			fscanf(map->fileMap, "%d", &map->squares[countS].coordY2);
			map->squares[i].flagA = 1;
			map->squares[i].flagB = 1;
			map->squares[i].flagC = 1;
			map->squares[i].flagD = 1;
			countS++;
			break;
		case 1:
			fscanf(map->fileMap, "%d", &map->lines[countL].coordX1);
			fscanf(map->fileMap, "%d", &map->lines[countL].coordY1);
			fscanf(map->fileMap, "%d", &map->lines[countL].coordX2);
			fscanf(map->fileMap, "%d", &map->lines[countL].coordY2);
			countL++;
			break;
		case 2:
			fscanf(map->fileMap, "%d", &map->triangles[countT].coordX1);
			fscanf(map->fileMap, "%d", &map->triangles[countT].coordY1);
			fscanf(map->fileMap, "%d", &map->triangles[countT].coordX2);
			fscanf(map->fileMap, "%d", &map->triangles[countT].coordY2);
			fscanf(map->fileMap, "%d", &map->triangles[countT].coordX3);
			fscanf(map->fileMap, "%d", &map->triangles[countT].coordY3);
			countT++;
			break;
		case 3:
			printf("\nCaiu aqui");
			fscanf(map->fileMap, "%d", &map->circles[countC].coordX);
			fscanf(map->fileMap, "%d", &map->circles[countC].coordY);
			fscanf(map->fileMap, "%d", &map->circles[countC].radius);
			countC++;
			break;
		}
	}

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
int initEnemy(avatar *player, char type, int arenaWidth, int arenaHeight)
{
	player->acX = 0;
	player->acY = 0;
	player->coordX = 50;
	player->coordY = 50;
	player->enable = 0;
	player->life = 20;

	switch(type){
	case ENEMY_NORMAL:
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

