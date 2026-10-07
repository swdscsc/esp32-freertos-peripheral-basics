# ESP32 外设驱动与 FreeRTOS 多任务基础实践

ESP32-S3 嵌入式开发的三个递进模块：**GPIO 点灯 → 按键检测 → FreeRTOS 多任务**。
每个模块都是可独立编译的完整 ESP-IDF 工程。

| 模块 | 内容 | 关键知识点 |
| --- | --- | --- |
| `02-led` | GPIO 输出控制 LED 闪烁 | `gpio_set_level`、延时节奏控制 |
| `03-key` | 按键扫描 + 电平翻转控制 LED | GPIO 输入、消抖、状态翻转 |
| `04-freertos` | **两个独立任务并发运行** | `xTaskCreate`、任务优先级与栈大小、任务间并发 |

## 模块 3：FreeRTOS 多任务（`04-freertos/main/main.c`）

把「按键扫描」和「LED 闪烁」拆成两个互不阻塞的独立任务：

```c
// 任务1：按键扫描，按下时输出 hello world
void key_task(void *arg)
{
    uint8_t key;
    while (1) {
        key = key_scan();
        if (key == 1) printf("hello world\n");
        vTaskDelay(pdMS_TO_TICKS(10));      // 主动让出 CPU
    }
}

// 任务2：LED 以 500ms 周期闪烁
void led_task(void *arg)
{
    while (1) {
        gpio_set_level(GPIO_NUM_1, 0);
        vTaskDelay(pdMS_TO_TICKS(500));
        gpio_set_level(GPIO_NUM_1, 1);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void app_main(void)
{
    led_init();
    key_init();
    xTaskCreate(key_task, "key_task", 4096, NULL, 1, NULL);
    xTaskCreate(led_task, "led_task", 4096, NULL, 1, NULL);
}
```

**要点**：
- 两个 `while(1)` 死循环能"同时"运行，靠的是 FreeRTOS 的任务调度
- 每个任务循环里必须调用 `vTaskDelay()` 主动让出 CPU，否则低优先级任务会饿死
- 任务的栈大小（4096 字节）和优先级（1）需要根据实际负载调整

## 编译

进入任一模块目录：

```bash
cd 04-freertos
idf.py set-target esp32s3
idf.py build
idf.py -p COMx flash monitor
```

## 与其他仓库的关系

这三个模块是后续综合项目的基础组件：

- 综合应用 → [`esp32-qmi8658-attitude-monitor`](https://github.com/)（姿态采集 + LVGL 显示）
- 联网 → [`esp32-wifi-station`](https://github.com/)（WiFi STA + 事件组）

---

## License

MIT
