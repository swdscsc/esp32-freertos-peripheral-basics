#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/uart.h"

// 综合示例：按键中断 -> 队列 -> 任务读取 -> UART 上报
#define BTN_GPIO  GPIO_NUM_0
#define UART_NUM  UART_NUM_0

static QueueHandle_t g_queue;

static void IRAM_ATTR btn_isr(void *arg)
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
    gpio_isr_handler_add(BTN_GPIO, btn_isr, NULL);

    uart_config_t cfg = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
    uart_param_config(UART_NUM, &cfg);
    uart_driver_install(UART_NUM, 256, 256, 0, NULL, 0);

    uint32_t dummy;
    int cnt = 0;
    char msg[64];
    while (1) {
        if (xQueueReceive(g_queue, &dummy, portMAX_DELAY)) {
            vTaskDelay(pdMS_TO_TICKS(50));
            if (gpio_get_level(BTN_GPIO) == 0) {
                cnt++;
                int n = snprintf(msg, sizeof(msg), "button #%d via UART\r\n", cnt);
                uart_write_bytes(UART_NUM, msg, n);
            }
        }
    }
}
