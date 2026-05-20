idf_component_register(SRCS "body_2wd.c" "threeeyes.c" "wheel.c" "main.c"  "mpu6050.c" "test_tasks/pid_task.c"  "test_tasks/ir_line_task.c" "test_tasks/threeeyes_task.c" "test_tasks/imu_task.c" "test_tasks/wheel_task.c"
                    INCLUDE_DIRS "." "./test_tasks"
                    REQUIRES bdc_motor driver esp_adc esp_timer)
