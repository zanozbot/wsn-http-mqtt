#include "syn.h"

static const int rx_interval = time_interval;
#define rx_buffer_size	16
static bool rx_buffer[rx_buffer_size] = { 0 };
static int rx_buffer_index = 0;

// check if the message is received
void received_task(void *pvParameters) {
	int i;
	uint8_t pcf_byte;

	while (1) {

		for (i = 0; i <= rx_buffer_size - 8; i++)
			// received message [10101010]
			if ((rx_buffer[i] && rx_buffer[i + 2] && rx_buffer[i + 4] && rx_buffer[i + 6])
					&& !(rx_buffer[i + 1] || rx_buffer[i + 3] || rx_buffer[i + 5] || rx_buffer[i + 7])) {
				// turn on led1
				pcf_byte = led1;
				i2c_slave_write(BUS_I2C, PCF_ADDRESS, NULL, &pcf_byte, 1);
				// set all elements of rx_buffer to 0
				memset(rx_buffer, 0, sizeof rx_buffer);
				rx_buffer_index = 0;
				break;
			}

		vTaskDelay(pdMS_TO_TICKS(rx_buffer_size*rx_interval));

		i2c_slave_read(BUS_I2C, PCF_ADDRESS, NULL, &pcf_byte, 1);
		if ((pcf_byte & button1) == 0) {
			// turn off leds
			pcf_byte = 0xff;
			i2c_slave_write(BUS_I2C, PCF_ADDRESS, NULL, &pcf_byte, 1);
		}
	}
}

void rx_intr_handler(uint8_t gpio_num) {
	static TickType_t current_intr_time, last_intr_time = 0;

	current_intr_time = xTaskGetTickCountFromISR();
	if (current_intr_time - last_intr_time > pdMS_TO_TICKS(200)) {
		//printf("%d\n", gpio_read(gpio_rx));
		rx_buffer[rx_buffer_index] = gpio_read(gpio_rx);
		rx_buffer_index = (rx_buffer_index + 1) % rx_buffer_size;
	}
	last_intr_time = current_intr_time;
}

