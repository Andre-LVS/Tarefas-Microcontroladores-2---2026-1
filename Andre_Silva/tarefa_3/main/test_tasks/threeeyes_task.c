#include "threeeyes_task.h"
#include "body_2wd.h"

//=========================
//Para parar o robô ao detectar um obstáculo


#include "ir_line_task.h" // p/ eu usar o handle da task
#include "wheel.h" //para eu usar o wheel_SetVel
#include "pid_task.h"

//=========================

//-----terefa3------------

#include "wcet_gpio.h"
#include "esp_task_wdt.h"

//------------------------
const static char *TAG = "three_eyes";

portTASK_FUNCTION(Threeeyes, args)
{
    ThreeEyes_Init();
    
    esp_task_wdt_add(NULL); // Tarefa 3
    
    
	//ThreeEyes_DisableLeft();
    //ThreeEyes_DisableRight();
    ultrasonic_value_t sensor[3];
    char *near_sensor_name;
    char *sensor_name[] = {"left", "middle", "right"};
	while(1)
	{
		
		WCET_INICIO(WCET_GPIO_THREEEYES); // Tarefa 3
		
        ThreeEyes_TrigAndWait(portMAX_DELAY);
        ThreeEyes_Read(&sensor[0], &sensor[1], &sensor[2]);
        
        uint32_t min_ticks = 0xFFFFFFFF; 
        near_sensor_name = "none";

        for ( int i = 0; i < 3; i++ )
        {
            if (sensor[i].isUpdated == pdTRUE && sensor[i].tof_ticks < min_ticks) 
            {
                min_ticks = sensor[i].tof_ticks;
                near_sensor_name = sensor_name[i];
            }
        }

        float distance = (min_ticks * (1000000.0 / esp_clk_apb_freq())) / 58.0;
        
         //=========================
        //Parar para o robô ao detectar um obstáculo

        
        if (distance < 12.0)
        {
			 //vTaskSuspend(pid_task_handle);   //por algum motivo o taskSUspend e taskresume nõa funciona, o robô trava
            //wheel_SetVel( 0, 0);
            Body2WD_detectarObjeto(1);   
            ESP_LOGI(TAG, "Obstáculo detectado.");
        }

        else
        {
             Body2WD_detectarObjeto(0);
            //vTaskResume(pid_task_handle);
        }

        
        //=========================
        ESP_LOGI(TAG, "The sensor with the nearest detected object was: %s (Distance: %.2f cm)", near_sensor_name, distance);
        //printf("The sensor with the nearest detected object was: %s (Distance: %"PRIu32" ticks)\n", near_sensor_name, min_ticks);
        
        WCET_FIM(WCET_GPIO_THREEEYES);//tarefa 3
        esp_task_wdt_reset(); //tarefa 3
    
        vTaskDelay(pdMS_TO_TICKS(500));
    }
	
}
