/*
 * HTTP server example.
 *
 * This sample code is in the public domain.
 */
#include <espressif/esp_common.h>
#include <esp8266.h>
#include <esp/uart.h>
#include <string.h>
#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <ssid_config.h>
#include <httpd/httpd.h>
#include <aws_iot.c>
#include <bmp280/bmp280.h>

#define LED_PIN 2

enum {
    SSI_UPTIME,
    SSI_FREE_HEAP,
    SSI_TEMPERATURE,
    SSI_LED_STATE
};

float temperature = 0;
float pressure = 0;
bmp280_t bmp280_dev;

#define SCL 14
#define SDA 12
#define BUS_I2C     0

int32_t ssi_handler(int32_t iIndex, char *pcInsert, int32_t iInsertLen)
{
    switch (iIndex) {
        case SSI_UPTIME:
            snprintf(pcInsert, iInsertLen, "%d",
                    xTaskGetTickCount() * portTICK_PERIOD_MS / 1000);
            break;
        case SSI_FREE_HEAP:
            snprintf(pcInsert, iInsertLen, "%d", (int) xPortGetFreeHeapSize());
            break;
        case SSI_LED_STATE:
            snprintf(pcInsert, iInsertLen, (GPIO.OUT & BIT(LED_PIN)) ? "Off" : "On");
            break;
        default:
            snprintf(pcInsert, iInsertLen, "N/A");
            break;
    }

    /* Tell the server how many characters to insert */
    return (strlen(pcInsert));
}

const char *websocket_cgi_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    return "/temperature.html";
}

void websocket_task(void *pvParameter)
{
    struct tcp_pcb *pcb = (struct tcp_pcb *) pvParameter;

    for (;;) {
        if (pcb == NULL || pcb->state != ESTABLISHED) {
            printf("Connection closed, deleting task\n");
            break;
        }

        int uptime = xTaskGetTickCount() * portTICK_PERIOD_MS / 1000;
        int heap = (int) xPortGetFreeHeapSize();
        int led = !gpio_read(LED_PIN);

        /* Generate response in JSON format */
        char response[64];
        int len = snprintf(response, sizeof (response),
                "{\"uptime\" : \"%d\","
                " \"temperature\" : \"%f\","
                " \"heap\" : \"%d\"}", uptime, temperature, heap);
        if (len < sizeof (response))
            websocket_write(pcb, (unsigned char *) response, len, WS_TEXT_MODE);

        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);
}

/**
 * This function is called when websocket frame is received.
 *
 * Note: this function is executed on TCP thread and should return as soon
 * as possible.
 */
void websocket_cb(struct tcp_pcb *pcb, uint8_t *data, u16_t data_len, uint8_t mode)
{
    printf("[websocket_callback]:\n%.*s\n", (int) data_len, (char*) data);

    uint8_t response[2];
    uint16_t val;

    switch (data[0]) {
        case 'A': // ADC
            /* This should be done on a separate thread in 'real' applications */
            val = temperature;
            break;
        case 'D': // Disable LED
            gpio_write(LED_PIN, true);
            val = 0xDEAD;
            break;
        case 'E': // Enable LED
            gpio_write(LED_PIN, false);
            val = 0xBEEF;
            break;
        default:
            printf("Unknown command\n");
            val = 0;
            break;
    }

    response[1] = (uint8_t) val;
    response[0] = val >> 8;

    websocket_write(pcb, response, 2, WS_BIN_MODE);
}

/**
 * This function is called when new websocket is open and
 * creates a new websocket_task if requested URI equals '/stream'.
 */
void websocket_open_cb(struct tcp_pcb *pcb, const char *uri)
{
    printf("WS URI: %s\n", uri);
    if (!strcmp(uri, "/stream")) {
        printf("request for streaming\n");
        xTaskCreate(&websocket_task, "websocket_task", 1024, (void *) pcb, 2, NULL);
    }
}

void httpd_task(void *pvParameters)
{
    tCGI pCGIs[] = {
        {"/temperature", (tCGIHandler) websocket_cgi_handler},
    };

    const char *pcConfigSSITags[] = {
        "uptime", // SSI_UPTIME
        "heap",   // SSI_FREE_HEAP
        "temperature",   // SSI_TEMPERATURE
        "led"     // SSI_LED_STATE
    };

    /* register handlers and start the server */
    http_set_cgi_handlers(pCGIs, sizeof (pCGIs) / sizeof (pCGIs[0]));
    http_set_ssi_handler((tSSIHandler) ssi_handler, pcConfigSSITags,
            sizeof (pcConfigSSITags) / sizeof (pcConfigSSITags[0]));
    websocket_register_callbacks((tWsOpenHandler) websocket_open_cb,
            (tWsHandler) websocket_cb);
    httpd_init();

    for(;;);
}

void bmp_task(void *pvParameters) {

	//char buffer [50];
	//float temperature, pressure;

    while (1) {
        bmp280_force_measurement(&bmp280_dev);
        // wait for measurement to complete
        while (bmp280_is_measuring(&bmp280_dev)) {
        };
        bmp280_read_float(&bmp280_dev, &temperature, &pressure, NULL);

        vTaskDelay(pdMS_TO_TICKS(50000));
    }



	//sprintf(buffer, "pressure:%.1fPa,temp:%.1fC", pressure, temperature);

	//printf("%s\n", buffer);

	// transmit_nrf24(buffer);

}

void user_init(void)
{
    uart_set_baud(0, 115200);
    printf("SDK version:%s\n", sdk_system_get_sdk_version());

    /* turn off LED */
    gpio_enable(LED_PIN, GPIO_OUTPUT);
    gpio_write(LED_PIN, true);

    i2c_init(BUS_I2C, SCL, SDA, I2C_FREQ_100K);
    gpio_enable(SCL, GPIO_OUTPUT);


    // BMP280 configuration
	bmp280_params_t params;
	bmp280_init_default_params(&params);
	params.mode = BMP280_MODE_FORCED;
	bmp280_dev.i2c_dev.bus = BUS_I2C;
	bmp280_dev.i2c_dev.addr = BMP280_I2C_ADDRESS_0;
	bmp280_init(&bmp280_dev, &params);

	xTaskCreate(bmp_task, "BMP task", 512, NULL, 2, NULL);

    /* initialize tasks */
    publish_queue = xQueueCreate(3, 16);
    xTaskCreate(&wifi_task, "wifi_task", 512, NULL, 2, NULL);
    xTaskCreate(&beat_task, "beat_task", 512, NULL, 2, NULL);
    xTaskCreate(&mqtt_task, "mqtt_task", 2048, NULL, 2, NULL);
    xTaskCreate(&httpd_task, "HTTP Daemon", 512, NULL, 2, NULL);
}
