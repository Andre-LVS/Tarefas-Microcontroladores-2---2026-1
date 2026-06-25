#include "pid_task.h"
#include "body_2wd.h" // para eu usar o Body2WD_compute()

//----------tarefa 3-----------
//WCET e Watch_Dog
#include "wcet_gpio.h"
#include "esp_task_wdt.h"
//-----------------------------

#define PID_PERIODO_MS 20 //loop do pid em ms

TaskHandle_t pid_task_handle = NULL; //handle para o vTaskSuspend ou  vTaskResume

portTASK_FUNCTION(PID_Task, arg){
	
	//-----------tarefa 3-------------
	wcet_gpio_int();
	esp_task_wdt_add(NULL);
	//-------------------------------

    TickType_t xLastWakeTime = xTaskGetTickCount();

    while(1){
		
		WCET_INICIO(WCET_GPIO_PID); //Tarefa 3 -> inicio do WCET
		
        //calcula o PID e move os motores
        Body2WD_Compute();
        
        WCET_FIM(WCET_GPIO_PID);  //Tarefa 3 -> fim do WCET
        
        esp_task_wdt_reset(); //Alimentea o doguinho?

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(PID_PERIODO_MS));

    }



}