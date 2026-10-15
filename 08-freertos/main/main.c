#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

// 生产者/消费者：队列传递数据，互斥量保护打印
static QueueHandle_t g_queue;
static SemaphoreHandle_t g_mutex;

void producer(void *arg)
{
    int i = 0;
    while (1) {
        xQueueSend(g_queue, &i, portMAX_DELAY);
        i++;
        vTaskDelay(pdMS_TO_TICKS(300));
    }
}

void consumer(void *arg)
{
    int v;
    while (1) {
        if (xQueueReceive(g_queue, &v, portMAX_DELAY)) {
            xSemaphoreTake(g_mutex, portMAX_DELAY);
            printf("consumed: %d\n", v);
            xSemaphoreGive(g_mutex);
        }
    }
}

void app_main(void)
{
    g_queue = xQueueCreate(5, sizeof(int));
    g_mutex = xSemaphoreCreateMutex();
    xTaskCreate(producer, "producer", 2048, NULL, 5, NULL);
    xTaskCreate(consumer, "consumer", 2048, NULL, 5, NULL);
}
