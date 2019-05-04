PROGRAM=http_server

EXTRA_CFLAGS=-DLWIP_HTTPD_CGI=1 -DLWIP_HTTPD_SSI=1 -I./fsdata

#Enable debugging
#EXTRA_CFLAGS+=-DLWIP_DEBUG=1 -DHTTPD_DEBUG=LWIP_DBG_ON

EXTRA_COMPONENTS=~/esp-open-rtos/extras/mbedtls ~/esp-open-rtos/extras/httpd ~/esp-open-rtos/extras/paho_mqtt_c ~/esp-open-rtos/extras/i2c ~/esp-open-rtos/extras/rboot-ota

include ~/esp-open-rtos/common.mk

html:
	@echo "Generating fsdata.."
	cd fsdata && ./makefsdata
