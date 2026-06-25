/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */

#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_private/esp_clk.h"
#include "driver/mcpwm_cap.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "driver/pulse_cnt.h"
#include "bdc_motor.h"
#include "pid_ctrl.h"
#include "hal/gpio_types.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "wheel.h"
#include "threeeyes_task.h"
#include "wheel_task.h"
#include "imu_task.h"
#include <inttypes.h>

//----------------------
#include "ir_line_task.h"
#include "body_2wd.h"
#include "pid_task.h"
//----------------------
#include "Log_task.h"
#include "wcet_gpio.h"
#include "esp_task_wdt.h"

//----------------------
#define THREE_EYES_TASK
//#define IMU_TASK
//#define WHEEL_CTRL_TASK
#define IR_LINE_TASK
#define PID_TASK

//tarefa 3
#define LOG_TASK;

void app_main(void)
{

    wheel_Init(); // inicializa motores e encoders
    //wheel_GoForward(); // define direção
    Body2WD_init(); // inicializa PID e mutex
								 	
//Tarefa 3 -> ADicionar WatchDog |\/\/|
//								 |/\/\|

esp_task_wdt_config_t wdt_config = {
	
	.timeout_ms = 500,     		//500 ms de timeout
	.idle_core_mask = 0,   		//
	.trigger_panic  = false, 	//Não reinicia o sistema
};
	esp_task_wdt_reconfigure(&wdt_config);

//---------------------------
#ifdef THREE_EYES_TASK
    xTaskCreate(Threeeyes,
                "threeeyes",
                configMINIMAL_STACK_SIZE*3,
                NULL,
                5,
                NULL);
#endif

//--------tarefa 3-----
#ifdef LOG_TASK

	xTaskCreate(Log_Task,
				"Log",
				configMINIMAL_STACK_SIZE*3,
				NULL,
				3,
				NULL);

#endif

//======================================

#ifdef IR_LINE_TASK
    xTaskCreate(IR_Line,
                "IR_Line",
                configMINIMAL_STACK_SIZE*4,
                NULL,
                5,
                &ir_line_task_handle);  //para a terefa treeeye_task.c poder suspender a ir_line_task quando detectar objeto
#endif

#ifdef PID_TASK
    xTaskCreate(PID_Task,
                "pid",
                configMINIMAL_STACK_SIZE*3,
                NULL,
                6,
                &pid_task_handle);
#endif
//=========================================

#ifdef IMU_TASK
    xTaskCreate(IMU_Task,
                "imu",
                configMINIMAL_STACK_SIZE*3,
                NULL,
                5,
                NULL);
#endif

#ifdef WHEEL_CTRL_TASK
    xTaskCreate(wheel_ctrl,
                "wheel",
                configMINIMAL_STACK_SIZE*3,
                NULL,
                5,
                NULL);
#endif
}