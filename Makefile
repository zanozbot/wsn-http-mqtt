PROGRAM=sensor

EXTRA_CFLAGS=-DLWIP_HTTPD_CGI=1 -DLWIP_HTTPD_SSI=1 -I./fsdata

EXTRA_COMPONENTS = ~/esp-open-rtos/extras/paho_mqtt_c ~/esp-open-rtos/extras/mbedtls ~/esp-open-rtos/extras/httpd ~/esp-open-rtos/extras/i2c ~/esp-open-rtos/extras/bmp280

include ~/esp-open-rtos/common.mk

html:
	@echo "Generating fsdata.."
	cd fsdata && ./makefsdata