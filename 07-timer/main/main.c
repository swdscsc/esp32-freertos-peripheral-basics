#include <stdio.h>
#include "esp_timer.h"

// 高精度周期性定时器（esp_timer），每 1 秒触发一次回调
static void periodic_cb(void *arg)
{
    static int n = 0;
    printf("esp_timer tick %d\n", ++n);
}

void app_main(void)
{
    esp_timer_handle_t handle;
    esp_timer_create_args_t args = {
        .callback = periodic_cb,
        .name     = "periodic",
    };
    esp_timer_create(&args, &handle);
    esp_timer_start_periodic(handle, 1000000);   // 1 000 000 us = 1s
}
