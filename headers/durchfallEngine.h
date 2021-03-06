/* ======================================= DER DURCHFALL ========================================== //
| INSTITUTO FEDERAL DE SANTA CATARINA - IFSC		FEDERAL INSTITUTE OF SANTA CATARINA				||
| DISCIPLINA DE PROGRAMACAO C						C PROGRAMMING LANGUAGE							||
| PROFESSOR FERNANDO PACHECO						PROFESSOR FERNANDO PACHECO						||
| CAIO PRUJANSKY E JOHANN SCHMITDINGER				CAIO PRUJANSKY E JOHANN SCHMITDINGER			||
| ARQUIVO: durchfallEngine.h						FILE: durchfallEngine.h							||
| VERSAO: v1.0										VERSION: v1.0									||
| DATA DE ALTERACAO: 02/06/2015						LAST ALTERED DATE: 02/06/2015					||
// ================================================================================================ */

#ifndef DURCHFALLENGINE_H_
#define DURCHFALLENGINE_H_

#include "durchfallDefines.h"
#include "durchfallStructs.h"

// GRAPHIC CONTROL FUNCTIONS ====================================================================== //
int loadSettings(graphicSettings *settings);
void fpsControl(graphicSettings *settings);

// TIME CONTROL FUNCTIONS ========================================================================= //
void setTimer(int *timer, int time);
void checkTimer(timeControl *timer);
void controlAnimation(aniControl *ani, int speed, int limit);

// PHYSICS FUNCTIONS ============================================================================== //
void phAddAc(float *ac, float power, char dir, float mass, float trig, float limit);
void phMoveObject(avatar *object);
void phMoveEnemy(avatar *enemy, circle vital[],float totalCircles);
void phMoveShots(projectile shots[]);
void phNormalize(float *ac, float power, float mass);
void phColideBallRec(avatar *object, square *block);
void phColideBallLine(avatar *object, float lineX0, float lineY0, float lineX1, float lineY1);
void phColide2Ball(avatar *object1, avatar *object2, ALLEGRO_SAMPLE *sound);
void phColideShotRec(avatar *object, square *block);
void phColideShotBall(avatar *shooter, avatar *target, ALLEGRO_SAMPLE *sound);

// ATTACK FUNCTIONS =============================================================================== //
void atkTackle(avatar *agent, int targetX, int targetY);
void atkShoot(avatar *agent, int targetX, int targetY);
void vitalDemage(circle *vital, avatar *enemy);

// MAP FUNCTIONS ================================================================================== //
void initMap(gameMap *map, char filePath[]);
void deinitMap(gameMap *map);
void moveViewPoint(avatar *reference, mapView *view);

// AVATAR FUNCTIONS =============================================================================== //
int initPlayer(avatar *player, char type, int arenaWidth, int arenaHeight);
int initEnemy(avatar *player, gameMap *map, char type, int arenaWidth, int arenaHeight);
void setLookDirection(avatar *agent, int targetX, int targetY);

#endif /* DURCHFALLSTRUCTS_H_ */
