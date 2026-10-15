#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c_master.h"

// I2C 主机读取从设备寄存器（示例：读 QMI8658 的 WHO_AM_I = 0x00，器件地址 0x6B）
#define DEV_ADDR  0x6B
#define WHO_AM_I  0x00

void app_main(void)
{
    i2c_master_bus_config_t bus_cfg = {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = GPIO_NUM_21,
        .scl_io_num = GPIO_NUM_22,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
    };
    i2c_master_bus_handle_t bus;
    i2c_new_master_bus(&bus_cfg, &bus);

    i2c_master_dev_handle_t dev;
    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address  = DEV_ADDR,
        .scl_speed_hz    = 400000,
    };
    i2c_master_bus_add_device(bus, &dev_cfg, &dev);

    uint8_t reg = WHO_AM_I;
    uint8_t id = 0;
    while (1) {
        i2c_master_transmit_receive(dev, &reg, 1, &id, 1, -1);
        printf("WHO_AM_I = 0x%02X\n", id);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
