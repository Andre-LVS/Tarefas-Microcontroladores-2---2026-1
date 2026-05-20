#ifndef PID_TASK_H
#define PID_TASK_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern TaskHandle_t pid_task_handle;

portTASK_FUNCTION(PID_Task, arg);

#endif