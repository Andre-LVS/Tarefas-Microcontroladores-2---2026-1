

/*
int Body2WD_SetTwist( int v, int w )
{
	int rotation = WHEEL_AXIS_LENGHT_2*w;
	int wL = v + rotation;
	int wR = v - rotation;
	
	
	
	return 1;
}
*/

#include "body_2wd.h"
#include "pid_ctrl.h"
#include "wheel.h"
#include "esp_log.h"

#include "pid_task.h"

static const char *TAG = "Body2WD";

static pid_ctrl_block_handle_t PID_linear;
static pid_ctrl_block_handle_t PID_angular;

static int _bloqueado = 0; // 0 = livre, 1 = obstáculo detectado

static int pulsosAnteriorRoda_D = 0; // Última leitura do encoder direito
static int pulsosAnteriorRoda_E = 0; // Última leitura do encoder esquerdo

static float _erro_compartilhado = 0.0; //erro que será compartilhado com a terefa ir_line task
										//Ex: 0.0 = reto, 1.0 = desviar à direita, -1.0 = desviar à esquerda

static SemaphoreHandle_t mutex_erro_compartilhado = NULL;//proteção para eu poder compartilhar a variável de erro


//inicializar o PID e o mutex
void Body2WD_init(void){
	
	mutex_erro_compartilhado = xSemaphoreCreateMutex();
	
	//-------------
	//pid linear
	pid_ctrl_config_t linear_config ={
		.init_param ={
			.kp = 120.00, // Ganho proporcional <- reage ao erro de velocidade ~80.00 ~110
			.ki = 50.00, // Ganho integral →eliminar erro residual?~10
			.kd = 0.0,
			.max_output = VEL_RODA_MAX,
			.min_output = VEL_RODA_MIN,
			 
		}
	};
	pid_new_control_block(&linear_config, &PID_linear);
	
	//pid angular 
	
		pid_ctrl_config_t angular_config ={
		.init_param ={
			.kp = 85.0,  //testar valores diferentes
			.ki = 0.1,  //aumetar valor depois de kp ficar estável. começar ao poucos. +1.0
			.kd = 0.0,
			.max_output = VEL_RODA_MAX,
			.min_output = -VEL_RODA_MAX,
			 
		}
	};
	pid_new_control_block(&angular_config, &PID_angular);
	
	ESP_LOGI(TAG, "Pid e mutex inicializados");
	
	//-------------?
};

//passa o valor do erro e obter/devolver o mutex
void Body2WD_SetErro(float erro){
	if (xSemaphoreTake(mutex_erro_compartilhado, pdMS_TO_TICKS(10))==pdTRUE){ 
		
		vTaskResume(pid_task_handle); //Para resumir o pid task quando o robô estiver no chão

		_erro_compartilhado=erro; //variável compartilhada
		xSemaphoreGive(mutex_erro_compartilhado);
		
	}
};
//mutex ->Parar ao detectar objeto=================

void Body2WD_detectarObjeto(int bloqueado) {
    if (xSemaphoreTake(mutex_erro_compartilhado, pdMS_TO_TICKS(10)) == pdTRUE) {
        _bloqueado = bloqueado;
        xSemaphoreGive(mutex_erro_compartilhado);
    }
}
//======================================



void Body2WD_Compute(){

//=========Para ao detectar objeto=============

int bloqueado_local = 0;
    if (xSemaphoreTake(mutex_erro_compartilhado, pdMS_TO_TICKS(10)) == pdTRUE) {
        bloqueado_local = _bloqueado;
        xSemaphoreGive(mutex_erro_compartilhado);
    }
    if (bloqueado_local) {
        wheel_SetVel(0, 0);
        return; // sai da função sem calcular PID
    }

//=============================================
	
	float erro_local = 0.0;
	if (xSemaphoreTake(mutex_erro_compartilhado, pdMS_TO_TICKS(10))==pdTRUE){ 
		
		erro_local=_erro_compartilhado;
		xSemaphoreGive(mutex_erro_compartilhado);
		
	}
	
	float v = 0.0;
	float w = 0.0;
	
	//------------------------
	
	int pulsoAtualRoda_D = 0;
	int pulsoAtualRoda_E = 0;
	
	wheel_GetEndoderPulses(&pulsoAtualRoda_E, &pulsoAtualRoda_D);

	float velRoda_D = (pulsoAtualRoda_D - pulsosAnteriorRoda_D) / pdMS_TO_TICKS(20);
	float velRoda_E = (pulsoAtualRoda_E - pulsosAnteriorRoda_E) / pdMS_TO_TICKS(20);
	
	pulsosAnteriorRoda_D = pulsoAtualRoda_D;
	pulsosAnteriorRoda_E = pulsoAtualRoda_E;
	
	float velMedia = (velRoda_D + velRoda_E)/2.0;
	//------------------------
	
	//------------PID linear--------
	float erro_linear = 0.6 - velMedia; // 1.0 = velocidade "1 unidade de pulso/tick"
										// 2.0 = mais rápido | 0.5 = mais devagar


	pid_compute(PID_linear, erro_linear, &v);
	
	//------------PID angular------
	float erro_angular = 0.0 - erro_local;
	pid_compute(PID_angular, erro_angular, &w);
		
	int wL = v + w;
	int wR = v - w;

	if (wL < VEL_RODA_MIN) {wL = VEL_RODA_MIN;}
	if (wR < VEL_RODA_MIN) {wR = VEL_RODA_MIN;}
	if (wL > VEL_RODA_MAX) {wL = VEL_RODA_MAX;}
	if (wR > VEL_RODA_MAX) {wR = VEL_RODA_MAX;}

	ESP_LOGI(TAG, "v=%.1f w=%.1f | wL=%d wR=%d", v, w, wL, wR);
	
	wheel_SetVel((uint32_t)wL, (uint32_t)wR);
};




