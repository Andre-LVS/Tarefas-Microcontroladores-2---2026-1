/*
 * wcet_gpio.h
 *Tarefa 3:
 * Estas macros definem o liga/desliga de um pino GPIO para iniciar no inicio e fim de cada tarefa. 
 *Com o osciloscópio, dá para ver o tempo de execução de cada tarefa, pois um pino ficara alto durante este tempo.
 *
 
 */

#ifndef MAIN_WCET_GPIO_H_
#define MAIN_WCET_GPIO_H_

#include "driver/gpio.h" 

#define WCET_GPIO_PID		GPIO_NUM_46 //Depois ver quias gpios estao livres
#define WCET_GPIO_IRLINE	GPIO_NUM_47
#define WCET_GPIO_THREEEYES	GPIO_NUM_48
#define WCET_GPIO_LOG		GPIO_NUM_45

//Iniciar pinos como saída
static inline void wcet_gpio_int(void){
	gpio_config_t cfg={
	.pin_bit_mask = (1ULL << WCET_GPIO_PID) 		|
					(1ULL << WCET_GPIO_IRLINE) 		|
					(1ULL << WCET_GPIO_THREEEYES)	|
					(1ULL << WCET_GPIO_LOG),
	.mode 			= 	GPIO_MODE_OUTPUT,
	.pull_down_en 	=	GPIO_PULLDOWN_DISABLE,
	.pull_up_en 	=	GPIO_PULLUP_DISABLE,
	.intr_type		=	GPIO_INTR_DISABLE,
	};
	gpio_config(&cfg);
}
//Macros para usar dentro das tarefas
#define WCET_INICIO(gpio_num) gpio_set_level((gpio_num), 1)
#define WCET_FIM(gpio_num) gpio_set_level((gpio_num), 0)




#endif /* MAIN_WCET_GPIO_H_ */
