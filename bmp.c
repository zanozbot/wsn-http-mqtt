#include <stdio.h>

#include "espressif/esp_common.h"
#include "esp/uart.h"

#include "FreeRTOS.h"
#include "task.h"

#include "i2c/i2c.h"
#include "bmp280/bmp280.h"

#define SCL 14
#define SDA 12
#define BUS_I2C     0

bmp280_t bmp280_dev;
float temperature = 0;
float pressure = 0; 

void bmp_init() {
    i2c_init(BUS_I2C, SCL, SDA, I2C_FREQ_100K);
    gpio_enable(SCL, GPIO_OUTPUT);

    // BMP280 configuration
	bmp280_params_t params;
	bmp280_init_default_params(&params);
	params.mode = BMP280_MODE_FORCED;
	bmp280_dev.i2c_dev.bus = BUS_I2C;
	bmp280_dev.i2c_dev.addr = BMP280_I2C_ADDRESS_0;
	bmp280_init(&bmp280_dev, &params);
}

void bmp_task(void *pvParameters) {

    while (1) {
        bmp280_force_measurement(&bmp280_dev);
        // wait for measurement to complete
        while (bmp280_is_measuring(&bmp280_dev)) {
        };
        bmp280_read_float(&bmp280_dev, &temperature, &pressure, NULL);

        vTaskDelay(pdMS_TO_TICKS(50000));
    }

}