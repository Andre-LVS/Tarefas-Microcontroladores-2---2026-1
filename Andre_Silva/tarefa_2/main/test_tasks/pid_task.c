#include "pid_task.h"
#include "body_2wd.h" // para eu usar o Body2WD_compute()


#define PID_PERIODO_MS 20 //loop do pid em ms

TaskHandle_t pid_task_handle = NULL; //handle para o vTaskSuspend ou  vTaskResume

portTASK_FUNCTION(PID_Task, arg){

    TickType_t xLastWakeTime = xTaskGetTickCount();

    while(1){

        //calcula o PID e move os motores
        Body2WD_Compute();

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(PID_PERIODO_MS));

    }



}