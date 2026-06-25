
//#include "wdt_stuckedTask.h"
#include "esp_task_wdt.h"
#include "wheel.h"
#include "esp_log.h"

static const char *TAG = "WDT";

void esp_task_wdt_isr_user_handler(void){
	
	esp_rom_printf("Watch_Dog latindo! Robô girando...");
	
	wheel_GoLeft();// girar no eixo
	wheel_SetVel(250, 250);
	while(1){
		//Fica preso aqui girando
	}
}
