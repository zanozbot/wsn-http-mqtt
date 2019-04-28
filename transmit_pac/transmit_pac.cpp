#include "nrf24.h"
#include <stdio.h>
#include <stdlib.h>

extern "C" {
	#include "bmp280/bmp280.h"
}

#define tx_size		25
static char tx_buffer[tx_size] = "pressure:0.0Pa,temp:0.0C";

bmp280_t bmp280_dev;

// transmit data
void transmit_nrf24() {

	// turn on led1
	write_byte_pcf(led1);

	radio.powerUp();
	radio.stopListening();
	radio.write(&tx_buffer, sizeof(tx_buffer));
	radio.powerDown();

	// turn off leds
	vTaskDelay(pdMS_TO_TICKS(300));
	write_byte_pcf(0xff);
}

// read BMP280 sensor values
extern "C" void read_bmp280() {

	float temperature, pressure;

	bmp280_force_measurement(&bmp280_dev);
	// wait for measurement to complete
	while (bmp280_is_measuring(&bmp280_dev)) {
	};
	bmp280_read_float(&bmp280_dev, &temperature, &pressure, NULL);

	sprintf(tx_buffer, "pressure:%.1fPa,temp:%.1fC", pressure, temperature);

	printf("%s\n", tx_buffer);
	//transmit_nrf24();

}

void bmp_task(void *pvParameters) {
	while(1) {
		read_bmp280();

		// check again after 3s
		vTaskDelay(pdMS_TO_TICKS(3000));
	}
}

extern "C" void user_init(void) {
	setup_nrf();
	radio.openWritingPipe(address);
	radio.powerDown();

	// BMP280 configuration
	bmp280_params_t params;
	bmp280_init_default_params(&params);
	params.mode = BMP280_MODE_FORCED;
	bmp280_dev.i2c_dev.bus = BUS_I2C;
	bmp280_dev.i2c_dev.addr = BMP280_I2C_ADDRESS_0;
	bmp280_init(&bmp280_dev, &params);

	xTaskCreate(bmp_task, "BMP task", 1000, NULL, 2, NULL);
}
