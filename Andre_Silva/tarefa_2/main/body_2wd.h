
//int Body2WD_SetTwist( int v, int w );
//void Body2WD_GetLocation(int &x, int &y);

//=============================================
#include "pid_ctrl.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h" //Par usar o mutex

#define VEL_RODA_MAX 290
#define VEL_RODA_MIN 180


//iniciar o PID e o Mutex
void Body2WD_init(void);

void Body2WD_SetErro(float erro);

void Body2WD_Compute(void);

//Parr ao detectar objeto
void Body2WD_detectarObjeto(int bloqueado); // 1 = parar, 0 = livre

//=============================================
