#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"

// UART0 回显：把收到的字节原样发回
void app_main(void)
{
    uart_config_t cfg = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
    uart_param_config(UART_NUM_0, &cfg);
    uart_driver_install(UART_NUM_0, 256, 256, 0, NULL, 0);

    uint8_t buf[128];
    while (1) {
        int n = uart_read_bytes(UART_NUM_0, buf, sizeof(buf) - 1, 20 / portTICK_PERIOD_MS);
        if (n > 0) {
            buf[n] = 0;
            uart_write_bytes(UART_NUM_0, (const char *)buf, n);
            printf("echo: %s", buf);
        }
    }
}
