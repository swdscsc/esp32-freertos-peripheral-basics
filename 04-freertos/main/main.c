#include <stdio.h>
#include "freertos/FreeRTOS.h"            //系统
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "led.h"
#include "key.h"

//任务1：按键控制输出hello world
void key_task(void *arg)
{
    uint8_t key;
    while(1)
    {
        key = key_scan();
        if(key == 1)
        {
            printf("hello world\n");
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

//任务2：灯闪烁
void led_task(void *arg)
{
    while(1)
    {
        gpio_set_level(GPIO_NUM_1,0);
        vTaskDelay(pdMS_TO_TICKS(500));
        gpio_set_level(GPIO_NUM_1,1);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void app_main(void)
{
    led_init();
    key_init();

    //创建任务
    xTaskCreate(key_task,"key_task",4096,NULL,1,NULL);
    xTaskCreate(led_task,"led_task",4096,NULL,1,NULL);
}

