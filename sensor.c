#include <stdio.h>

#include "espressif/esp_common.h"
#include "esp/uart.h"

#include "FreeRTOS.h"
#include "task.h"

#include "http_server.c"
#include "mqtt_client.c"
#include "bmp.c"

void user_init(void)
{
    uart_set_baud(0, 115200);
    printf("SDK version:%s\n", sdk_system_get_sdk_version());

    http_init();
    mqtt_init();
    bmp_init();
    
    xTaskCreate(&wifi_task, "wifi_task",  256, NULL, 2, NULL);
    xTaskCreate(&beat_task, "beat_task", 256, NULL, 2, NULL);
    xTaskCreate(&mqtt_task, "mqtt_task", 1024, NULL, 2, NULL);
    xTaskCreate(&bmp_task, "bmp_task", 1024, NULL, 2, NULL);
    xTaskCreate(&httpd_task, "httpd_task", 1024, NULL, 2, NULL);
}