#include <stdio.h>
#include "freertos/FreeRTOS.h"            //系统
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "led.h"
#include "key.h"

void app_main(void)
{
    uint8_t key;
    led_init();
    key_init();

    while (1)                 
    {
        key = key_scan();
        if(key == 1)
        {
            printf("按键按下\n");
            LED_TUN;
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

