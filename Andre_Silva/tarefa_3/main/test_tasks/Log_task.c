/*
 * Log_task.cpp
 *
 *  Created on: 17 de jun. de 2026
 *      Author: aluno
 */

#include "Log_task.h"
#include "wheel.h"
#include "wcet_gpio.h"
#include "esp_log.h"

#define PERIODO_LOG 1000 //ms

static const char *TAG = "Log_Task";

portTASK_FUNCTION(Log_Task, arg){
	TickType_t xLastWakeTime = xTaskGetTickCount();
	
	while(1){
		
		WCET_INICIO(WCET_GPIO_LOG);
		
		int pulsos_E = 0;
		int pulsos_D = 0;
		
		wheel_GetEndoderPulses(&pulsos_E, &pulsos_D);
		
		ESP_LOGI(TAG, "Encoder Esq: %d | Encoder Dir: %d", pulsos_E, pulsos_D);
		
		WCET_FIM(WCET_GPIO_LOG);
		
		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(PERIODO_LOG));
		
	}
}
