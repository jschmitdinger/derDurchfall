
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
void controlAnimation(aniControl *ani, int speed, int limit)
{
	//
	if(ani->time > 0)
		ani->time--;
	else{
		ani->time = speed;
		ani->step++;
	}

	if(ani->step >= limit)
		ani->step = 0;

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
        else if(limit == 0)
            *ac = limit;
	} else{
		if((*ac-power) > (-limit))
			*ac -= (power * mass * trig);
        else if(limit == 0)
            *ac = limit;
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
void phMoveShots(projectile shots[])
{
	//
	int i;
	for(i=0; i<TOTAL_SHOTS; i++){
		if(shots[i].enable){
			shots[i].coordX += shots[i].acX;
			shots[i].coordY += shots[i].acY;
		}
	}

	return;
}
void phMoveEnemy(avatar *enemy, circle vital[],float totalCircles)
{
	int setX, setY, setD;

    setD = 0;

    setX = vital[enemy->objective].stepX[enemy->stepCounter];
    setY = vital[enemy->objective].stepY[enemy->stepCounter];
    setD = sqrt(pow((enemy->coordX - vital[enemy->objective].stepX[enemy->stepCounter]),2) + pow((enemy->coordY - vital[enemy->objective].stepY[enemy->stepCounter]),2));

    if(setD < 20)
    	enemy->stepCounter++;

    if(enemy->stepCounter >= vital[enemy->objective].totalSteps){
    	setX = vital[enemy->objective].coordX;
    	setY = vital[enemy->objective].coordY;
    }

    if(setX - enemy->coordX > 0)
    {
        phAddAc(&enemy->acX,enemy->power,1,enemy->weight,1,ENEMY_SPEED);
    }
    if(setY - enemy->coordY > 0)
    {
        phAddAc(&enemy->acY,enemy->power,1,enemy->weight,1,ENEMY_SPEED);
    }
    if(setX - enemy->coordX < 0)
    {
        phAddAc(&enemy->acX,enemy->power,-1,enemy->weight,1,ENEMY_SPEED);
    }
    if(setY - enemy->coordY < 0)
    {
        phAddAc(&enemy->acY,enemy->power,-1,enemy->weight,1,ENEMY_SPEED);
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
void phColide2Ball(avatar *object1, avatar *object2, ALLEGRO_SAMPLE *sound)
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

        if(!object1->timeElement.flag){
        	object2->life -= 2*DAMEGE_CONST;
        	object1->timeElement.time = 0;
        	setTimer(&object2->timeExplosion.time, 25);
            if(object2->life <= 0)
            	object1->score+=10;
            al_play_sample(sound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
        }
        if(!object2->timeElement.flag){
        	object1->life -= 2*DAMEGE_CONST;
        	object2->timeElement.time = 0;
        	setTimer(&object1->timeExplosion.time, 25);
            if(object1->life <= 0)
            	object2->score+=10;
            al_play_sample(sound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
        }
    }
    return;

}
void phColideShotRec(avatar *object, square *block)
{
    int i;

    for(i = 0; i<TOTAL_SHOTS; i++){
        if((object->shots[i].coordX > block->coordX1)&&(object->shots[i].coordX < block->coordX2)&&(object->shots[i].coordY > block->coordY1)&&(object->shots[i].coordY < block->coordY2)&&(object->shots[i].enable == 1)){
            if(((object->shots[i].coordX-object->shots[i].acX > block->coordX2)||(object->shots[i].coordX-object->shots[i].acX < block->coordX1))||((object->shots[i].coordY-object->shots[i].acY > block->coordY2)||(object->shots[i].coordY-object->shots[i].acY < block->coordY1)))
                object->shots[i].enable = 0;
        }
        else{
            if(((object->shots[i].coordX-object->shots[i].acX < block->coordX2)&&(object->shots[i].coordX-object->shots[i].acX > block->coordX1))&&((object->shots[i].coordY-object->shots[i].acY < block->coordY2)&&(object->shots[i].coordY-object->shots[i].acY > block->coordY1)))
                object->shots[i].enable = 0;
        }

    }
}
void phColideShotBall(avatar *shooter, avatar *target, ALLEGRO_SAMPLE *sound)
{
    float distance;
    int i;

    for(i = 0; i<TOTAL_SHOTS; i++)
    {
        distance = sqrt(pow((shooter->shots[i].coordX - target->coordX),2)+(pow((shooter->shots[i].coordY - target->coordY),2)));

        if((distance<=target->radius)&&(shooter->shots[i].enable == 1))
        {
            shooter->shots[i].enable = 0;
            target->life-=10;
            al_play_sample(sound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
            setTimer(&target->timeExplosion.time, 25);
            if(target->life <= 0)
            	shooter->score+=10;
        }
    }
}

// ATTACK FUNCTIONS =============================================================================== //
void atkTackle(avatar *agent, int targetX, int targetY)
{
	float angle;
	float trigX, trigY;

	angle = atan((targetY - agent->coordY)/(targetX - agent->coordX)) * (-1);

	trigX = cos(angle);
	trigY = sin(angle);

	if(targetX > agent->coordX){
		if(trigX < 0)
			trigX *= (-1);
	} else if(targetX < agent->coordX){
		if(trigX > 0)
			trigX *= (-1);
	}
	if(targetY > agent->coordY){
		if(trigY < 0)
			trigY *= (-1);
	} else if(targetY < agent->coordY){
		if(trigY > 0)
			trigY *= (-1);
	}

	agent->acX = 0;
	agent->acY = 0;

	phAddAc(&agent->acX, TACKLE_SPEED*trigX, 1, agent->weight, 1, TACKLE_SPEED);
	phAddAc(&agent->acY, TACKLE_SPEED*trigY, 1, agent->weight, 1, TACKLE_SPEED);

	setTimer(&agent->timeFlame.time, agent->delay[ATCK_MELEE]);
	setTimer(&agent->cantouch.time, agent->delay[ATCK_MELEE]);

	agent->timeElement.time = agent->delay[ATCK_MELEE];

	return;
}

void atkShoot(avatar *agent, int targetX, int targetY)
{
	float angle;
	float trigX, trigY;

	angle = atan((targetY - agent->coordY)/(targetX - agent->coordX)) * (-1);

	trigX = cos(angle);
	trigY = sin(angle);

	if(targetX > agent->coordX){
		if(trigX < 0)
			trigX *= (-1);
	} else if(targetX < agent->coordX){
		if(trigX > 0)
			trigX *= (-1);
	}
	if(targetY > agent->coordY){
		if(trigY < 0)
			trigY *= (-1);
	} else if(targetY < agent->coordY){
		if(trigY > 0)
			trigY *= (-1);
	}

	if(agent->shotCount >= TOTAL_SHOTS){
		agent->shotCount = 0;
		fflush(stdout);
	}

	if(!agent->shots[agent->shotCount].enable){

		agent->shots[agent->shotCount].coordX = agent->coordX;
		agent->shots[agent->shotCount].coordY = agent->coordY;
		agent->shots[agent->shotCount].acX = BULLET_SPEED*trigX;
		agent->shots[agent->shotCount].acY = BULLET_SPEED*trigY;
		agent->shots[agent->shotCount].enable = 1;
		agent->shots[agent->shotCount].aniShots.step = 0;
		agent->shots[agent->shotCount].aniShots.time = 0;

		agent->shotCount++;
	}

	fflush(stdout);

	return;
}
void vitalDemage(circle *vital, avatar *enemy)
{
    float distance;

    distance = sqrt(pow((vital->coordX - enemy->coordX),2) + pow((vital->coordY - enemy->coordY),2));

    if((distance < vital->radius + enemy->radius)&&(vital->timeDemage.flag))
    {
        vital->life-=10;
        if(vital->life <= 0)
            vital->life = 0;
        setTimer(&vital->timeDemage.time, 100);
    }
}

// MAP FUNCTIONS ================================================================================== //
void initMap(gameMap *map, char filePath[])
{
	int i, j, type;
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
	fscanf(map->fileMap, "%s", map->st);
	map->totalEnemies = atoi(map->st);

	map->squares = (square*) malloc(map->totalSquares*sizeof(square));
	map->lines = (line*) malloc(map->totalLines*sizeof(line));
	map->triangles = (triangle*) malloc(map->totalTriangles*sizeof(triangle));
	map->circles = (circle*) malloc(map->totalCircles*sizeof(circle));
	map->enemies = (avatar*) malloc(map->totalEnemies*sizeof(avatar));

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
			fscanf(map->fileMap, "%d", &map->circles[countC].coordX);
			fscanf(map->fileMap, "%d", &map->circles[countC].coordY);
			fscanf(map->fileMap, "%d", &map->circles[countC].radius);
			fscanf(map->fileMap, "%d", &map->circles[countC].totalSteps);

			map->circles[countC].stepX = (int*) malloc(map->circles[countC].totalSteps*sizeof(int));
			map->circles[countC].stepY = (int*) malloc(map->circles[countC].totalSteps*sizeof(int));

			for(j=0; j<map->circles[countC].totalSteps; j++){
				fscanf(map->fileMap, "%s", map->st);
				map->circles[countC].stepX[j] = atoi(map->st);
				fscanf(map->fileMap, "%s", map->st);
				map->circles[countC].stepY[j] = atoi(map->st);
			}

			map->circles[countC].stepCounter = 0;
			map->circles[countC].timeDemage.time = 0;
			map->circles[countC].life = 100;

			countC++;
			break;
		}
	}

	return;
}
void deinitMap(gameMap *map)
{
	//
	free(map->circles);
	free(map->enemies);
	free(map->lines);
	free(map->squares);
	free(map->triangles);

	return;
}
void moveViewPoint(avatar *reference, mapView *view)
{
	if(((reference->coordX - view->coordX) <= (view->rangeX * 0.35)) && (view->coordX > 0))
		view->coordX += reference->acX;
	if((reference->coordX - view->coordX) >= (view->rangeX * 0.65) && (view->coordX < (view->totalWidth - view->rangeX)))
		view->coordX += reference->acX;
	if((reference->coordY - view->coordY) <= (view->rangeY * 0.2) && (view->coordY > 0))
		view->coordY += reference->acY;
	if((reference->coordY - view->coordY) >= (view->rangeY * 0.70) && (view->coordY < (view->totalHeight - view->rangeY)))
		view->coordY += reference->acY;

	return;
}

// AVATAR FUNCTIONS =============================================================================== //
int initPlayer(avatar *player, char type, int arenaWidth, int arenaHeight)
{
	int i;

	player->acX = 0;
	player->acY = 0;
	player->coordX = arenaWidth/2;
	player->coordY = arenaHeight/2;
	player->shotCount = 0;

	player->delay[ATCK_MELEE] = 100;
	player->delay[ATCK_SHOOT] = 10;
	player->delay[ATCK_MOOVE] = 10;
	player->timeElement.flag = ELEMENT_UN;

	player->posHead = 0;

	player->aniBody.step = 0;
	player->aniBody.time = 0;

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

	player->timeAttack.flag = false;
	player->timeAttack.time = 0;
	player->timeMovement.flag = false;
	player->timeMovement.time = 0;
	player->timeFlame.flag = false;
	player->timeFlame.time = 0;
	player->timeExplosion.flag = false;
	player->timeExplosion.time = 0;
	player->cantouch.flag= false;
	player->cantouch.time= 0;

	player->score = 0;

	player->aniBody.step = 0;
	player->aniBody.time = 0;
	player->aniMelee.step = 0;
	player->aniMelee.time = 0;

	player->life = 100;

	for(i=0; i<TOTAL_SHOTS; i++){
		player->shots[i].enable = 0;
		player->shots[i].acX = 0;
		player->shots[i].acY = 0;
		player->shots[i].coordX = player->coordX;
		player->shots[i].coordY = player->coordY;
	}

	return 1;
}
int initEnemy(avatar *player, gameMap *map, char type, int arenaWidth, int arenaHeight)
{
	int i, j;
	int circle_distance;

	player->acX = 0;
	player->acY = 0;
	player->enable = 0;
	player->life = 20;
	player->shotCount = 0;

	player->delay[ATCK_MELEE] = 100;
	player->delay[ATCK_SHOOT] = 100;
	player->delay[ATCK_MOOVE] = 10;
	player->timeElement.flag = ELEMENT_UN;

	player->timeElement.flag = 1;

	player->stepCounter = 0;

	for(i=0; i<TOTAL_SHOTS; i++){
		player->shots[i].enable = 0;
		player->shots[i].acX = 0;
		player->shots[i].acY = 0;
		player->shots[i].coordX = player->coordX;
		player->shots[i].coordY = player->coordY;
	}


	switch(type){
	case ENEMY_NORMAL:
	    for(i=0; i<TOTAL_SHOTS; i++){
		player->shots[i].enable = 0;
		player->shots[i].acX = 0;
		player->shots[i].acY = 0;
		player->shots[i].coordX = player->coordX;
		player->shots[i].coordY = player->coordY;
        player->coordX = map->width/2 - 50;
        player->coordY = 100;
		player->radius = 15;
		player->power = 0.2;
		player->weight = 15/player->radius;
	}
	break;
	case ENEMY_PHASE2:
	    for(i=0; i<TOTAL_SHOTS; i++){
		player->shots[i].enable = 0;
		player->shots[i].acX = 0;
		player->shots[i].acY = 0;
		player->shots[i].coordX = player->coordX;
		player->shots[i].coordY = player->coordY;
        player->coordX = map->width/2 + 50;
        player->coordY = 100;
		player->radius = 15;
		player->power = 0.2;
		player->weight = 15/player->radius;
	}
	break;
	case ENEMY_PHASE3:
	    for(i=0; i<TOTAL_SHOTS; i++){
		player->shots[i].enable = 0;
		player->shots[i].acX = 0;
		player->shots[i].acY = 0;
		player->shots[i].coordX = player->coordX;
		player->shots[i].coordY = player->coordY;
        player->coordX = map->width - 100;
        player->coordY = map->height/2 + 100;
		player->radius = 15;
		player->power = 0.2;
		player->weight = 15/player->radius;
	}
	break;
	case ENEMY_PHASE4:
	    for(i=0; i<TOTAL_SHOTS; i++){
		player->shots[i].enable = 0;
		player->shots[i].acX = 0;
		player->shots[i].acY = 0;
		player->shots[i].coordX = player->coordX;
		player->shots[i].coordY = player->coordY;
        player->coordX = 100;
        player->coordY = map->height/2 + 100;
		player->radius = 15;
		player->power = 0.2;
		player->weight = 15/player->radius;
	}
	break;
	default:	for(i=0; i<TOTAL_SHOTS; i++){
		player->shots[i].enable = 0;
		player->shots[i].acX = 0;
		player->shots[i].acY = 0;
		player->shots[i].coordX = player->coordX;
		player->shots[i].coordY = player->coordY;
        player->coordX = map->width/3*2;
        player->coordY = 50;
		player->radius = 15;
		player->power = 0.2;
		player->weight = 15/player->radius;
	}
	for(i=0; i<TOTAL_SHOTS; i++){
		player->shots[i].enable = 0;
		player->shots[i].acX = 0;
		player->shots[i].acY = 0;
		player->shots[i].coordX = player->coordX;
		player->shots[i].coordY = player->coordY;
	}

		return 0;	for(i=0; i<TOTAL_SHOTS; i++){
			player->shots[i].enable = 0;
			player->shots[i].acX = 0;
			player->shots[i].acY = 0;
			player->shots[i].coordX = player->coordX;
			player->shots[i].coordY = player->coordY;
		}
		for(i=0; i<TOTAL_SHOTS; i++){
			player->shots[i].enable = 0;
			player->shots[i].acX = 0;
			player->shots[i].acY = 0;
			player->shots[i].coordX = player->coordX;
			player->shots[i].coordY = player->coordY;
		}

		break;
	}	for(i=0; i<TOTAL_SHOTS; i++){
		player->shots[i].enable = 0;
		player->shots[i].acX = 0;
		player->shots[i].acY = 0;
		player->shots[i].coordX = player->coordX;
		player->shots[i].coordY = player->coordY;
	}


	circle_distance = sqrt(pow((player->coordX - map->circles[0].coordX),2) + pow((player->coordY - map->circles[0].coordY),2));

    for(j = 0; j<map->totalCircles; j++)
    {
        if((circle_distance > (sqrt(pow((player->coordX - map->circles[j].coordX),2) + pow((player->coordY - map->circles[j].coordY),2))))||(j == 0))
        {
            circle_distance = sqrt(pow((player->coordX - map->circles[j].coordX),2) + pow((player->coordY - map->circles[j].coordY),2));
            player->objective = j;
        }
    }

	return 1;
}
void setLookDirection(avatar *agent, int targetX, int targetY)
{
	float angle;
	float trigX, trigY;

	angle = atan((targetY - agent->coordY)/(targetX - agent->coordX)) * (-1);

	trigX = cos(angle);
	trigY = sin(angle);

	if(targetX > agent->coordX){
		if(trigX < 0)
			trigX *= (-1);
	} else if(targetX < agent->coordX){
		if(trigX > 0)
			trigX *= (-1);
	}
	if(targetY > agent->coordY){
		if(trigY < 0)
			trigY *= (-1);
	} else if(targetY < agent->coordY){
		if(trigY > 0)
			trigY *= (-1);
	}

	if(sqrt(pow(trigX, 2)) >= sqrt(pow(trigY, 2))){
		if(trigX > 0)
			agent->posHead = 1;
		else
			agent->posHead = 3;
	} else{
		if(trigY > 0)
			agent->posHead = 0;
		else
			agent->posHead = 2;
	}

	return;
}
