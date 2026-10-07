#include <stdio.h>
#include "freertos/FreeRTOS.h"            //系统
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "led.h"

void app_main(void)
{
   
    led_init();

    while (1)                 
    {
        gpio_set_level(GPIO_NUM_1,0);           //控制IO1引脚输出低电平
        vTaskDelay(pdMS_TO_TICKS(500));         //延时500ms
        gpio_set_level(GPIO_NUM_1,1);           //控制IO1引脚输出高电平
        vTaskDelay(pdMS_TO_TICKS(500));         //延时500ms
    }
}


