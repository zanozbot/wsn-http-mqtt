# Processing of temperature using WEMOS D1 mini 

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

This article demonstrates how to set up your own ESP8266 WiFi-enabled micro-controller (in our case WEMOS D1 mini)
to read the temperature, send it to the MQTT broker and run the HTTP server alongside, which shows the real 
time temperature without the need to subscribe to the MQTT topic, using FreeRTOS.

## Variables
Replace the credentials `WIFI_SSID` and `WIFI_PASS` inside 
[wsn-http-mqtt/private_ssid_config.h](https://github.com/zanozbot/wsn-http-mqtt/blob/master/private_ssid_config.h)
with your own access point information to enable the sensor to connect to the internet.

Also change the information `MQTT_HOST`, `MQTT_PORT`, `MQTT_USER` and `MQTT_PASS` inside
[wsn-http-mqtt/mqtt_client.c](https://github.com/zanozbot/wsn-http-mqtt/blob/master/mqtt_client.c)
to enable the connection to the MQTT broker. In this example a free plan on [CloudMQTT](https://www.cloudmqtt.com/)
was used.

## Instalation instructions
Building the project requires a copy of FreeRTOS for ESP8266 WiFi-enabled microcontrollers, which you can find in the repository
[SuperHouse/esp-open-rtos](https://github.com/SuperHouse/esp-open-rtos).

Before building the project check the Makefile to see if all the paths are correctly pointing to your downloaded files.
You can build it by running `make build`.

Flash you build onto the micro-controller using `make flash`.
If you change some pieces of the code and they are not visible after the flashing you can
- run `make clean`, or
- run `make erase_flase` to erase the data of the entire flash chip

and flash it again.
