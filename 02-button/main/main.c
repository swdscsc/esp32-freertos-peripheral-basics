#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

// GPIO 输入 + 下降沿中断 + 队列 + 软件消抖
#define BTN_GPIO  GPIO_NUM_0

static QueueHandle_t g_queue;

static void IRAM_ATTR gpio_isr_handler(void *arg)
{
    uint32_t dummy = 1;
    xQueueSendFromISR(g_queue, &dummy, NULL);
}

void app_main(void)
{
    g_queue = xQueueCreate(10, sizeof(uint32_t));

    gpio_set_direction(BTN_GPIO, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BTN_GPIO, GPIO_PULLUP_ONLY);
    gpio_set_intr_type(BTN_GPIO, GPIO_INTR_NEGEDGE);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BTN_GPIO, gpio_isr_handler, NULL);

    uint32_t dummy;
    int cnt = 0;
    while (1) {
        if (xQueueReceive(g_queue, &dummy, portMAX_DELAY)) {
            vTaskDelay(pdMS_TO_TICKS(50));          // 消抖
            if (gpio_get_level(BTN_GPIO) == 0) {
                cnt++;
                printf("button pressed, count=%d\n", cnt);
            }
        }
    }
}
