#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_adc/adc_oneshot.h"

// 单次 ADC 采样（ADC1 通道 0，GPIO1）
void app_main(void)
{
    adc_oneshot_unit_handle_t unit;
    adc_oneshot_unit_init_cfg_t uc = { .unit_id = ADC_UNIT_1 };
    adc_oneshot_new_unit(&uc, &unit);

    adc_oneshot_chan_cfg_t cc = {
        .atten    = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    adc_oneshot_config_channel(unit, ADC_CHANNEL_0, &cc);

    int raw;
    while (1) {
        adc_oneshot_read(unit, ADC_CHANNEL_0, &raw);
        printf("adc raw = %d\n", raw);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
